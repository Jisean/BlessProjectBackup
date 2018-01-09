#include "stdafx.h"
#include "SkyDome.h"

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

CSkyDome::CSkyDome(LPDIRECT3DDEVICE9 pDevice)
: CMapObjects(pDevice)
, m_bTop(true)
{

}

CSkyDome::~CSkyDome(void)
{

}

HRESULT CSkyDome::Initialize(const TCHAR* pMeshKey)
{
	FAILED_CHECK(AddComponent(pMeshKey));

	m_pInfo->m_vPos= _myvec(128.5f, -100.f, 128.5f);
	m_pInfo->m_vScale = _myvec(0.01f, 0.01f, 0.01f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = 1.0622094f;

	return S_OK;
}

_int CSkyDome::Update(const _float& fTimeDelta)
{
	if(m_bStart && !m_bTop)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_X] = D3DXToRadian(180.f);
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = 11.2724f;
		m_pInfo->m_vPos.y += 40.f;
		m_bStart = false;
	}

	_int iResult = Engine::CGameObject::Update(fTimeDelta);
	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_STATIC, this);
	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_VOLUMETRIC, this);

	return iResult;
}

void CSkyDome::Render(void)
{
	if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	CMapObjects::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);
	pEffect->SetTechnique("SkyDome");

	pEffect->AddRef();

	Set_ConstantTable(pEffect);

	pEffect->SetFloat("g_fFogRender", 1.f);

	m_pMesh->RenderMesh_ForShader(pEffect, &m_pInfo->m_matWorld);

	Engine::Safe_Release(pEffect);
}

HRESULT CSkyDome::AddComponent(const TCHAR* pMeshKey)
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
		pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_SkyDome");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_VTF");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	pComponent = m_pVolumetricShader = Engine::Clone_Shader(L"Shader_Volumetric");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_VolumetricShader", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);	

	return S_OK;
}

void CSkyDome::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CSkyDome* CSkyDome::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey/* = NULL*/)
{
	CSkyDome*		pGameObject = new CSkyDome(pDevice);
	if(FAILED(pGameObject->Initialize(pMeshKey)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSkyDome::Free(void)
{
	Engine::CGameObject::Free();
}