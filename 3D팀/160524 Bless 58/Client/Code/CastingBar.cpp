#include "stdafx.h"
#include "CastingBar.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Stage.h"
#include "Texture.h"
#include "DynamicCamera.h"
#include "KeyMgr.h"
#include "Status.h"
#include "Players.h"
#include "MainPlayer.h"

CCastingBar::CCastingBar(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fMaxTime(0.f)
, m_fCurTime(0.f)
, m_fAlpha(0.f)
{

}

CCastingBar::~CCastingBar(void)
{

}

HRESULT CCastingBar::Initialize(void)
{
	for(int i = 0 ; i < CAST_END; ++i)
		m_pTexture[i] = NULL;

	m_eTexNum = CAST_BAR;

	FAILED_CHECK(AddComponent());
            


	Set_TextureInit(CAST_OUTLINE, WINCX / 2.015f, WINCY / 1.20619f, WINCX / 9.6f, WINCY / 123.2f);

	       
	m_fOriSizeX = WINCX / 10.27f;
	m_fOrifX = WINCX / 2.014f;
	Set_TextureInit(CAST_BAR, m_fOrifX, WINCY / 1.20519f, m_fOriSizeX, WINCY / 187.488f);

	Set_TextureInit(CAST_Slider, m_fX[CAST_BAR] + (m_fSizeX[CAST_BAR]), WINCY / 1.20519f, WINCX / 61.3696f, WINCY / 38.5882f);

	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CCastingBar::Update(const _float& fTimeDelta)
{
	//Move_Tex();
	//Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);

	if(m_fAlpha <= 0.f)
		return 0;

	m_fCurTime -= fTimeDelta;
	float fCasting = 1.f - m_fCurTime / m_fMaxTime;

	// Hp 
	m_fSizeX[CAST_BAR] = m_fOriSizeX * fCasting;

	// 스르륵 사라지게 하기
	if(m_bRender == false)
	{
		//m_fAlpha = 1.f;
		m_fAlpha -= fTimeDelta * 2.f;
	}
	
	// 최대치 조건
	if(m_fOriSizeX <= m_fSizeX[CAST_BAR])
	{
		m_fSizeX[CAST_BAR] = m_fOriSizeX;
		m_bRender = false;
	}
	// 증가
	else
		m_fX[CAST_BAR] = m_fOrifX - (m_fOriSizeX * (1.f - fCasting));

	Set_TextureInit(CAST_Slider, m_fX[CAST_BAR] + (m_fSizeX[CAST_BAR]), WINCY / 1.20519f, WINCX / 61.3696f, WINCY / 38.5882f);




	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CCastingBar::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < CAST_END; ++i)
	{
		if(m_pTexture[i] == NULL)
			continue;

		pEffect->SetTechnique("Default_Device");
		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
		pEffect->SetMatrix("g_matView",&m_matView[i]);
		pEffect->SetMatrix("g_matProj",&m_matProj);

		pEffect->SetFloat("g_fAlpha", m_fAlpha);

		pEffect->SetFloat("g_fRGB", 1.f);
		pEffect->SetFloat("g_fPointPower", 1.f);

		m_pTexture[i]->Set_ConstantTable(pEffect,"g_BaseTexture");

		pEffect->Begin(NULL,0);
		pEffect->BeginPass(0);

		m_pBuffer->Render(&m_pInfo->m_matWorld);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);

	}

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CCastingBar::Free(void)
{
	CUI::Free();
}

HRESULT CCastingBar::AddComponent(void)
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

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader",pComponent);


	// Texture
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Cast_Outline");
	m_pTexture[CAST_OUTLINE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CAST_OUTLINE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Cast_Outline", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Cast_Bar");
	m_pTexture[CAST_BAR] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CAST_BAR], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Cast_Bar", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Cast_Slider");
	m_pTexture[CAST_Slider] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CAST_Slider], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Cast_Slider", pComponent);

	return S_OK;
}

CCastingBar* CCastingBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCastingBar*		pGameObject = new CCastingBar(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CCastingBar::Set_TextureInit(eCasting eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CCastingBar::Set_TexView(void)
{
	for(int i = 0; i < CAST_END; ++i)
	{
		D3DXMatrixIdentity(&m_matView[i]);	

		m_matView[i]._11 = m_fSizeX[i];
		m_matView[i]._22 = m_fSizeY[i];
		m_matView[i]._33 = 1.f;

		// -0.5	-> -400		,	0.5	-> 400
		m_matView[i]._41 = m_fX[i] - (WINCX >> 1);
		m_matView[i]._42 = -m_fY[i] + (WINCY >> 1);
	}
}

void CCastingBar::Set_Render(bool bRender)
{
	m_bRender = bRender;
}

void CCastingBar::Set_CastTime(float fMaxTime)
{
	m_fMaxTime = fMaxTime;
	m_fCurTime = fMaxTime;
	m_bRender = true;
	m_fAlpha = 1.f;
}

void CCastingBar::Init_MoveTex(eCasting eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CCastingBar::Move_Tex(void)
{
	float fTemp2 = 0.001f;

	if(Engine::GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		m_MoveX -= fTemp2;
	}

	if(Engine::GetDIKeyState(DIK_LEFT) & 0x80)
	{
		m_MoveX += fTemp2;
	}

	fTemp2 = 0.001f;
	if(Engine::GetDIKeyState(DIK_UP) & 0x80)
	{
		m_MoveY += fTemp2;
	}

	if(Engine::GetDIKeyState(DIK_DOWN) & 0x80)
	{
		m_MoveY -= fTemp2;
	}


	float fTemp = 0.1f;
	if(Engine::GetDIKeyState(DIK_Z) & 0x80)
	{
		m_MoveSizeX += fTemp;
	}

	if(Engine::GetDIKeyState(DIK_X) & 0x80)
	{
		m_MoveSizeX -= fTemp;
	}

	if(Engine::GetDIKeyState(DIK_C) & 0x80)
	{
		m_MoveSizeY += fTemp;
	}

	if(Engine::GetDIKeyState(DIK_V) & 0x80)
	{
		m_MoveSizeY -= fTemp;
	}

	cout << m_MoveX << "\t" << m_MoveY << "\t" << m_MoveSizeX << "\t" << m_MoveSizeY << endl;
}
