#include "stdafx.h"
#include "ATObject.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Back.h"
#include "CollisionMgr.h"

CATObject::CATObject(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_bCollision(false)
{

}

CATObject::~CATObject(void)
{

}

HRESULT CATObject::Initialize(const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos)
{
	FAILED_CHECK(AddComponent(pMeshKey));

	m_pInfo->m_vPos.x = vMousePos.x;
	m_pInfo->m_vPos.y = vMousePos.y;
	m_pInfo->m_vPos.z = vMousePos.z;

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Power = 20.f;

	return S_OK;
}

int CATObject::Update(const float& fTimeDelta)
{
	int iResult = Engine::CGameObject::Update(Engine::Get_Time());
	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);


	return iResult;
}

void CATObject::Render(void)
{
	if(m_bCollision)
		((Engine::CMesh*)m_pMesh)->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 255, 0, 0));
	else
		((Engine::CMesh*)m_pMesh)->SetBoundingBoxColor(D3DCOLOR_ARGB(255, 0, 255, 0));
	

	/*m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( 125, 255, 255, 255 ) );

	m_pMesh->RenderMesh_ForShader(&m_pInfo->m_matWorld);*/


	LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	Set_ContantTable(pEffect); 

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMesh->RenderMesh_ForShader(pEffect,&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CATObject::AddComponent(const TCHAR* pMeshKey)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_TOOL, pMeshKey);
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Mesh");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}


void CATObject::Set_ContantTable(LPD3DXEFFECT	pEffect)
{
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	D3DXMATRIX			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", (D3DXVECTOR4*)&m_tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (D3DXVECTOR4*)&m_tMtrl.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (D3DXVECTOR4*)&m_tMtrl.Specular);

	D3DXMATRIX			matViewInv;

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (D3DXVECTOR4*)&matViewInv.m[3][0]);	
	pEffect->SetFloat("g_fPower", m_tMtrl.Power);


	Engine::Safe_Release(pEffect);
}

CATObject* CATObject::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos)
{
	CATObject*		pGameObject = new CATObject(pDevice);
	if(FAILED(pGameObject->Initialize(pMeshKey,vMousePos)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CATObject::Free(void)
{
	Engine::CGameObject::Free();
}