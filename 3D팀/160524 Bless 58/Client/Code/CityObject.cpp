#include "stdafx.h"
#include "CityObject.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "CollisionMgr.h"
#include "Calculator.h"
#include "Material.h"
#include "KeyMgr.h"
#include "Collision.h"

//HTable								g_htInstanceInfo;		
//HTable								g_htWorldMatrix;
//vector<const TCHAR*>				g_vObjKeyName;
//bool								g_bhInitTable = false;
//bool								g_bHtableDelete = false;

CCityObject::CCityObject(LPDIRECT3DDEVICE9 pDevice)
: CMapObjects(pDevice)
{
	/*if(!g_bhInitTable)
	{
		g_vObjKeyName.reserve(1051);
		g_htInstanceInfo.Ready_Table(1051);
		g_htWorldMatrix.Ready_Table(1051);
		g_bhInitTable = true;
	}*/
}

CCityObject::~CCityObject(void)
{

}

HRESULT CCityObject::Initialize(const TCHAR* pMeshKey, int iIndex, int iInstanceIndex, bool bSelectCollison)
{
	m_bSelectCollision = bSelectCollison;
	FAILED_CHECK(AddComponent(pMeshKey));

	m_fSpecRange = 6.7999959f;
	m_fSpecPower = 0.54000044f;

	return S_OK;
}

_int CCityObject::Update(const _float& fTimeDelta)
{
	_int iResult = 0;
	if(m_bStart)
	{
		m_bStart = false;
		m_pPlayerInfo = (Engine::CTransform*)(Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform"));

		iResult = Engine::CGameObject::Update(fTimeDelta);
		Compute_ViewZ(&m_pInfo->m_vPos);
		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_STATIC, this);
	}

	if(m_pCollision->Collision_Distance(m_pInfo->m_vPos, m_pPlayerInfo->m_vPos, 10.f))
	{
		CDynamicCamera* pCam = (CDynamicCamera*)(Engine::Get_Scene()->GetObjList(LAYER_UI, L"DynamicCamera")->front());
		m_pCollision->Collision_Mesh_Cam(pCam, m_pInfo, m_pMesh);
	}

	if(m_bCulled)
	{
		if(10 == m_iCullingCnt)
		{
			iResult = CMapObjects::Update(fTimeDelta);
			m_iCullingCnt = 0;
		}
		else
			++m_iCullingCnt;

		iResult = Engine::CGameObject::Update(fTimeDelta);
		Compute_ViewZ(&m_pInfo->m_vPos);
		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_STATIC, this);
	}
	else
		iResult = CMapObjects::Update(fTimeDelta);



	//if(CKeyMgr::GetInstance()->GetStateKey('B'))
	//	m_fSpecPower += 0.01f;
	//if(CKeyMgr::GetInstance()->GetStateKey('N'))
	//	m_fSpecPower -= 0.01f;

	//if(CKeyMgr::GetInstance()->GetStateKey('X'))
	//	m_fSpecRange += 0.1f;
	//if(CKeyMgr::GetInstance()->GetStateKey('C'))
	//	m_fSpecRange -= 0.1f;

	//if(CKeyMgr::GetInstance()->GetKeyDown('Z'))
	//	__debugbreak();

	//cout << m_fSpecPower << "\t" << m_fSpecRange << endl;

	return iResult;
}

void CCityObject::Render(void)
{
	if(((CStage*)Engine::Get_Scene())->Get_CullingSite() && TRUE == m_bSelectCollision)
		m_bCulled = false;

	if(m_bCulled || m_eShadowRender == CGameObject::SHADOW_SHADOW)
	{
	#ifdef _DEBUG
		if(m_bCollision)
			((Engine::CMesh*)m_pMesh)->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 255, 0, 0));
		else
			((Engine::CMesh*)m_pMesh)->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 0, 255, 0));
	#endif

		LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
		if(NULL == pEffect)
			return;

		CMapObjects::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

		pEffect->AddRef();

		Set_ConstantTable(pEffect);

		pEffect->SetFloat("g_fSpecMapping", 1.f);
		pEffect->SetFloat("g_fSpecRange", m_fSpecRange);	// 범위 설정 값이 높을 수록 범위가 축소된다.
		pEffect->SetFloat("g_fSpecPow", m_fSpecPower);		// 파워 설정 값이 높을 수록 빛의 세기가 세진다.


		m_pMesh->RenderMesh_ForShader(pEffect, &m_pInfo->m_matWorld);

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CCityObject::AddComponent(const TCHAR* pMeshKey)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STATIC, pMeshKey);
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_VTF");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);

	// For.Collision Component
	pComponent = m_pCollision = CCollision::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Collision", pComponent);

	return S_OK;
}

void CCityObject::Collision_AABB(void)
{
	D3DXVECTOR3		vDestMin, vDestMax;

	const Engine::CComponent*	pPlayerInfo = Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent*	pPlayerMesh = Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Mesh");
	((Engine::CMesh*)pPlayerMesh)->GetMinMax(&vDestMin, &vDestMax);

	D3DXMATRIX		matAABB;
	D3DXMatrixIdentity(&matAABB);
	memcpy(&matAABB.m[3][0], &((Engine::CTransform*)pPlayerInfo)->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

	D3DXVec3TransformCoord(&vDestMin, &vDestMin, &matAABB);
	D3DXVec3TransformCoord(&vDestMax, &vDestMax, &matAABB);


	D3DXVECTOR3			vSourMin, vSourMax;
	m_pMesh->GetMinMax(&vSourMin, &vSourMax);
	D3DXMatrixIdentity(&matAABB);
	memcpy(&matAABB.m[3][0], &m_pInfo->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));
	D3DXVec3TransformCoord(&vSourMin, &vSourMin, &matAABB);
	D3DXVec3TransformCoord(&vSourMax, &vSourMax, &matAABB);

	if(Engine::Collision_AABB(&vDestMin, &vDestMax, &vSourMin, &vSourMax))
		m_bCollision = true;
	else
		m_bCollision = false;
}

void CCityObject::Collision_OBB(void)
{
	D3DXVECTOR3		vDestMin, vDestMax;
	D3DXMATRIX		matDestWorld;

	const Engine::CComponent*	pPlayerInfo = Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent*	pPlayerMesh = Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Mesh");
	((Engine::CMesh*)pPlayerMesh)->GetMinMax(&vDestMin, &vDestMax);
	matDestWorld = ((Engine::CTransform*)pPlayerInfo)->m_matWorld;

	D3DXVECTOR3			vSourMin, vSourMax;
	m_pMesh->GetMinMax(&vSourMin, &vSourMax);

	if(Engine::Collision_OBB(&vDestMin, &vDestMax, &matDestWorld
		, &vSourMin, &vSourMax, &m_pInfo->m_matWorld))
		m_bCollision = true;
	else
		m_bCollision = false;
}

void CCityObject::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CCityObject* CCityObject::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey/* = NULL*/, int iIndex/* = 0*/, int iInstanceIndex/* = 0*/, bool bSelectCollision)
{
	CCityObject*		pGameObject = new CCityObject(pDevice);
	if(FAILED(pGameObject->Initialize(pMeshKey, iIndex, iInstanceIndex, bSelectCollision)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CCityObject::Free(void)
{
	//if(!g_bHtableDelete)
	//{
	//	g_bHtableDelete = true;
	//	size_t iTableSize =	g_htInstanceInfo.Get_TableSize();
	//	for(size_t i = 0; i < iTableSize; ++i)
	//	{
	//		vector<Engine::INSTANSDATA*>* pInstanceInfo = (vector<Engine::INSTANSDATA*>*)g_htInstanceInfo.Erase_Slot(i);

	//		if(NULL != pInstanceInfo)
	//		{
	//			_uint iVecSize = pInstanceInfo->size();
	//			vector<Engine::INSTANSDATA*>::iterator iter = pInstanceInfo->begin();
	//			vector<Engine::INSTANSDATA*>::iterator iter_end = pInstanceInfo->end();
	//			for(iter; iter != iter_end; ++iter)
	//			{
	//				Engine::Safe_Delete_Array((*iter)->pAttributeTable);
	//				Engine::Safe_Delete(*iter);
	//			}
	//			Engine::Safe_Delete(pInstanceInfo);
	//		}
	//	}
	//	g_htInstanceInfo.Erase_Table();


	//	size_t iTableSize2 = g_htWorldMatrix.Get_TableSize();
	//	for(size_t i = 0; i < iTableSize2; ++i)
	//	{
	//		vector<D3DXMATRIX*>* pWorldMatrix = (vector<D3DXMATRIX*>*)g_htWorldMatrix.Erase_Slot(i);

	//		if(NULL != pWorldMatrix)
	//		{
	//			_uint iVecSize = pWorldMatrix->size();
	//			Engine::Safe_Delete(pWorldMatrix);
	//		}
	//	}
	//	g_htWorldMatrix.Erase_Table();
	//}
	//// 딜리트어레이??
	//Engine::Safe_Delete(m_cKeyName);
	Engine::CGameObject::Free();
}