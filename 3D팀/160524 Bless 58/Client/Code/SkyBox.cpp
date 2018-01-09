#include "stdafx.h"
#include "SkyBox.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Stage.h"
#include "Texture.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
{
	m_htComponent.Ready_Table(19);
}

CSkyBox::~CSkyBox(void)
{

}

HRESULT CSkyBox::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(30.f, 30.f, 30.f);
	return S_OK;
}

_int CSkyBox::Update(const _float& fTimeDelta)
{
	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(D3DXVECTOR3));

	m_pInfo->m_vPos = vCamPos;

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_PRIORITY, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CSkyBox::Render(void)
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

	m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture", 3);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pBuffer->Render(&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	

}

HRESULT CSkyBox::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_SkyBox");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Cube");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);


	return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkyBox*		pGameObject = new CSkyBox(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSkyBox::Free(void)
{
	Engine::CGameObject::Free();
}
