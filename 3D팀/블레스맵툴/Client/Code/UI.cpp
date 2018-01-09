#include "stdafx.h"
#include "UI.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Stage.h"
#include "Texture.h"

CUI::CUI(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_fX(0.f)
, m_fY(0.f)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
{

}

CUI::~CUI(void)
{

}

HRESULT CUI::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fX = 700.f;
	m_fY = 500.f;
	m_fSizeX = 150.f;
	m_fSizeY = 100.f;
	return S_OK;
}

_int CUI::Update(const _float& fTimeDelta)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	D3DXMatrixIdentity(&m_matView);
	m_matView._11 = m_fSizeX;
	m_matView._22 = m_fSizeY;
	m_matView._33 = 1.f;

	// -0.5	-> -400		,	0.5	-> 400
	m_matView._41 = m_fX - (WINCX >> 1);
	m_matView._42 = -m_fY + (WINCY >> 1);

	RECT	rcUI = {long(m_fX - m_fSizeX * 0.5f)
	, long(m_fY - m_fSizeY * 0.5f)
	, long(m_fX + m_fSizeX * 0.5f)
	, long(m_fY + m_fSizeY * 0.5f)};

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if(PtInRect(&rcUI, ptMouse))
	{
		m_fSizeX = 300.f;
		m_fSizeY = 200.f;
	}
	else
	{
		m_fSizeX = 150.f;
		m_fSizeY = 100.f;
	}

	return Engine::CGameObject::Update(fTimeDelta);
}

void CUI::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX		matOldView, matOldProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

HRESULT CUI::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_UI");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	return S_OK;
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUI*		pGameObject = new CUI(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CUI::Free(void)
{
	Engine::CGameObject::Free();
}
