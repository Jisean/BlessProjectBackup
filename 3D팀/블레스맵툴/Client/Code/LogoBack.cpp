#include "stdafx.h"
#include "LogoBack.h"

#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "Export_Function.h"



CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pShader(NULL)
{
	ZeroMemory(&m_tMtrl, sizeof(D3DMATERIAL9));
}

CLogoBack::~CLogoBack(void)
{

}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	/*m_Mtrl.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	m_Mtrl.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);*/

	return S_OK;
}

_int CLogoBack::Update(const _float& fTimeDelta)
{	
	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_PRIORITY, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CLogoBack::Render(void)
{
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pBuffer->Render(&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

/*
	m_pTexture->Render(0,0);
	*/
}

HRESULT CLogoBack::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_LOGO, L"Texture_Logo");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Default");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogoBack*		pGameObject = new CLogoBack(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CLogoBack::Free(void)
{
	Engine::CGameObject::Free();
}


