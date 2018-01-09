#include "stdafx.h"
#include "Aim.h"

#include "UI.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Stage.h"
#include "Texture.h"

CAim::CAim(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_bAct_Ranger(false)
{

}

CAim::~CAim(void)
{

}

HRESULT CAim::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fX = 512.f;
	m_fY = 332.f;
	m_fSizeX = 160.f;
	m_fSizeY = 120.f;

	return S_OK;
}

_int CAim::Update(const _float& fTimeDelta)
{
	if(false == m_bAct_Ranger)
		return 0;

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


	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CAim::Render(void)
{
	if(false == m_bAct_Ranger)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	D3DXMATRIX		matOldView, matOldProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);      //소스의 블랜딩값은 소스의 알파값을 블렌딩한다
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//데스트블렌딩 값은 소스의 알파값의 반대값을 블렌딩한다.
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);   //디폴트값 = 텍스쳐
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);    // 1과 2에 각각 텍스쳐와 디퓨즈를 넣고
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // 2개를 모듈레이트 한다 (합성)

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);


	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void CAim::Free(void)
{
	CUI::Free();
}

HRESULT CAim::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Aim_UI");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);
	return S_OK;
}

CAim* CAim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAim*		pGameObject = new CAim(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

bool CAim::Get_ActRanger()
{
	return m_bAct_Ranger;
}
void CAim::Set_ActRanger(bool bCheck)
{
	m_bAct_Ranger = bCheck;
}
