#include "stdafx.h"
#include "Sun.h"

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

CSun::CSun(LPDIRECT3DDEVICE9 pDevice)
: CMapObjects(pDevice)
{

}

CSun::~CSun(void)
{

}

HRESULT CSun::Initialize(const TCHAR* pMeshKey)
{
	FAILED_CHECK(AddComponent(pMeshKey));

	//m_pInfo->m_vPos = _vec3(106.f, 1.f, 117.f);
	
	m_pInfo->m_vPos = _vec3(-76.f, 384.f, 3.f);
	m_pInfo->m_vScale = _vec3(0.6f, 0.6f, 0.6f);

	return S_OK;
}

_int CSun::Update(const _float& fTimeDelta)
{
	float		fTime = fTimeDelta;

	//if(Engine::GetDIKeyState(DIK_J) & 0x80)
	//{
	//	m_pInfo->m_vPos += m_pInfo->m_vDir * 200.f * fTime;
	//}

	//if(Engine::GetDIKeyState(DIK_N) & 0x80)
	//{
	//	m_pInfo->m_vPos -= m_pInfo->m_vDir * 200.f * fTime;
	//}

	


	_int iResult = Engine::CGameObject::Update(fTimeDelta);
	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_PRIORITY, this);



	D3DXMATRIX		matBill, matView;
	D3DXMatrixIdentity(&matBill);
	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, 0, &matBill);
	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;


	return iResult;
}

void CSun::Render(void)
{
	if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW ||
		m_eShadowRender == Engine::CGameObject::SHADOW_OBJECT ||
		m_eShadowRender == Engine::CGameObject::SHADOW_NONE_S)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	//m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CMapObjects::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

	pEffect->AddRef();

	Set_ConstantTable(pEffect);

	m_pMesh->RenderMesh_ForShader(pEffect, &m_pInfo->m_matWorld);

	//m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	Engine::Safe_Release(pEffect);
}

HRESULT CSun::AddComponent(const TCHAR* pMeshKey)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	if(pMeshKey)
		pComponent = Engine::Clone_Resource(RESOURCE_STAGE, pMeshKey);
	else
		pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Sun");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Mesh");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);	

	return S_OK;
}

void CSun::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CSun* CSun::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey/* = NULL*/)
{
	CSun*		pGameObject = new CSun(pDevice);
	if(FAILED(pGameObject->Initialize(pMeshKey)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSun::Free(void)
{
	Engine::CGameObject::Free();
}