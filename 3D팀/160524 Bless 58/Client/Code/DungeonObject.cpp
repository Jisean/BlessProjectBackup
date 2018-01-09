#include "stdafx.h"
#include "DungeonObject.h"

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
#include "Stage2.h"
#include "DynamicCamera.h"

CDungeonObject::CDungeonObject(LPDIRECT3DDEVICE9 pDevice)
: CMapObjects(pDevice)
{

}

CDungeonObject::~CDungeonObject(void)
{

}

HRESULT CDungeonObject::Initialize(const TCHAR* pMeshKey, int iIndex, int iInstanceIndex)
{
	FAILED_CHECK(AddComponent(pMeshKey));

	return S_OK;
}

_int CDungeonObject::Update(const _float& fTimeDelta)
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
		if(pCam->Get_BossCam())
			m_pCollision->Collision_Mesh_Cam(pCam, m_pInfo, m_pMesh);
	}
	
	if(m_bCulled)
	{
		if(20 == m_iCullingCnt)
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

	return iResult;
}

void CDungeonObject::Render(void)
{
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

		m_pMesh->RenderMesh_ForShader(pEffect, &m_pInfo->m_matWorld);

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CDungeonObject::AddComponent(const TCHAR* pMeshKey)
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
	//_tcscpy_s(const_cast<TCHAR*>(m_cKeyName), _tcslen(pMeshKey)+1, pMeshKey);

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

void CDungeonObject::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CDungeonObject* CDungeonObject::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey/* = NULL*/, int iIndex/* = 0*/, int iInstanceIndex/* = 0*/)
{
	CDungeonObject*		pGameObject = new CDungeonObject(pDevice);
	if(FAILED(pGameObject->Initialize(pMeshKey, iIndex, iInstanceIndex)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CDungeonObject::Free(void)
{
	Engine::CGameObject::Free();
}