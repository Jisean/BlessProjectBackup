#include "stdafx.h"
#include "Stone.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "CollisionMgr.h"

CStone::CStone(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_bCollision(false)
{

}

CStone::~CStone(void)
{

}

HRESULT CStone::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos.x = float(rand() % VTXCNTX);
	m_pInfo->m_vPos.z = float(rand() % VTXCNTX);

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Power = 20.f;

	return S_OK;
}

_int CStone::Update(const _float& fTimeDelta)
{
	//Collision_AABB();
	Collision_OBB();
	_int iResult = Engine::CGameObject::Update(Engine::Get_Time());
	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);


	return iResult;
}

void CStone::Render(void)
{
	/*if(m_bCollision)
		((Engine::CMesh*)m_pMesh)->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 255, 0, 0));
	else
		((Engine::CMesh*)m_pMesh)->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 0, 255, 0));*/
	

	/*m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( 125, 255, 255, 255 ) );

	m_pMesh->RenderMesh_ForShader(&m_pInfo->m_matWorld);*/


	LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	Set_ContantTable(pEffect); 

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMesh->RenderMesh_ForShader(pEffect, &m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CStone::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Stone");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Mesh");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

void CStone::Collision_AABB(void)
{
	D3DXVECTOR3		vDestMin, vDestMax;

	const Engine::CComponent*	pPlayerInfo = Engine::Get_Component(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent*	pPlayerMesh = Engine::Get_Component(CStage::LAYER_GAMELOGIC, L"Player", L"Mesh");
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

void CStone::Collision_OBB(void)
{
	D3DXVECTOR3		vDestMin, vDestMax;
	D3DXMATRIX		matDestWorld;

	const Engine::CComponent*	pPlayerInfo = Engine::Get_Component(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent*	pPlayerMesh = Engine::Get_Component(CStage::LAYER_GAMELOGIC, L"Player", L"Mesh");
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

void CStone::Set_ContantTable(LPD3DXEFFECT	pEffect)
{
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;
	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&m_tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&m_tMtrl.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&m_tMtrl.Specular);

	_matrix			matViewInv;

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matViewInv.m[3][0]);	
	pEffect->SetFloat("g_fPower", m_tMtrl.Power);


	Engine::Safe_Release(pEffect);
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStone*		pGameObject = new CStone(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CStone::Free(void)
{
	Engine::CGameObject::Free();
}