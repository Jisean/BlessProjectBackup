#include "stdafx.h"
#include "DynamicObject.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Back.h"
#include "CollisionMgr.h"
#include "ObjectTool.h"
#include "MainFrm.h"
#include "MyForm.h"

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_bCollision(false)
{

}

CDynamicObject::~CDynamicObject(void)
{

}

HRESULT CDynamicObject::Initialize(const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos)
{
	_tcscpy_s(m_tcKey,pMeshKey);

	FAILED_CHECK(AddComponent(pMeshKey));

	m_pInfo->m_vPos.x = vMousePos.x;
	m_pInfo->m_vPos.y = vMousePos.y;
	m_pInfo->m_vPos.z = vMousePos.z;

	TCHAR temp[13];
	ZeroMemory(&temp,13);
	_tcsncpy_s(temp, pMeshKey, 12);
	temp[12] = '\0';

	BOOL b = _tcscmp(temp,L"Ghost_Knight");

	if(!b)
	{
		m_pInfo->m_vScale.x = 0.02f;
		m_pInfo->m_vScale.y = 0.02f;
		m_pInfo->m_vScale.z = 0.02f;
	}
	else
	{
		m_pInfo->m_vScale.x = 0.01f;
		m_pInfo->m_vScale.y = 0.01f;
		m_pInfo->m_vScale.z = 0.01f;
	}


	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Power = 20.f;

	//ZeroMemory(&temp,5);

	return S_OK;
}

int CDynamicObject::Update(const float& fTimeDelta)
{
	int iResult = Engine::CGameObject::Update(Engine::Get_Time());
	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);


	return iResult;
}

void CDynamicObject::Render(void)
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

	CObjectTool* pObjTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab2;

	if(pObjTool->m_WireFrame.GetCheck() == TRUE)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pMesh->RenderMesh_ForShader(pEffect,&m_pInfo->m_matWorld);

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	else
		m_pMesh->RenderMesh_ForShader(pEffect,&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CDynamicObject::AddComponent(const TCHAR* pMeshKey)
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


void CDynamicObject::Set_ContantTable(LPD3DXEFFECT	pEffect)
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

CDynamicObject* CDynamicObject::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos)
{
	CDynamicObject*		pGameObject = new CDynamicObject(pDevice);
	if(FAILED(pGameObject->Initialize(pMeshKey,vMousePos)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CDynamicObject::Free(void)
{
	Engine::CGameObject::Free();
}