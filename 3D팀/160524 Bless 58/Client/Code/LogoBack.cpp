#include "stdafx.h"
#include "LogoBack.h"

#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "Export_Function.h"



CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pShader(NULL)
, m_fFrame(0.f)
, m_fTime(0.f)
{
	m_htComponent.Ready_Table(19);
	ZeroMemory(&m_tMtrl, sizeof(D3DMATERIAL9));
}

CLogoBack::~CLogoBack(void)
{

}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_fMaxTime = 3.f;

	return S_OK;
}

_int CLogoBack::Update(const _float& fTimeDelta)
{	
	m_fTime += fTimeDelta;

	if(m_fMaxTime <= m_fTime)
	{
		m_fTime = 0.f;
		++m_fFrame;

		if(m_fFrame > 4)
			m_fFrame = 0.f;
	}

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

	for(int i = 0; i < 2; ++i)
	{
		m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture", (_ushort)(m_fFrame));
		m_pFliterTexture->Set_ConstantTable(pEffect, "g_FilterTexture");

		pEffect->SetFloat("g_fTime", 1.f - (m_fTime / m_fMaxTime));

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(i);

		m_pBuffer->Render(&m_pInfo->m_matWorld);

		pEffect->EndPass();
		pEffect->End();
	}

	Engine::Safe_Release(pEffect);
}

HRESULT CLogoBack::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Texture_Logo");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Texture_LogoFilter");
	m_pFliterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Texture_LogoFilter", pComponent);

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Default");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

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


