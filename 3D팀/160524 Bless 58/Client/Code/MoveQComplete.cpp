#include "stdafx.h"
#include "MoveQComplete.h"

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
#include "Option.h"
#include "QuestNpc.h"

CMoveQComplete::CMoveQComplete(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fCurTime(0.f)
, m_fMaxTime(3.f)
, m_bNewQuest(false)
{

}

CMoveQComplete::~CMoveQComplete(void)
{

}

HRESULT CMoveQComplete::Initialize(void)
{
	for(int i = 0 ; i < COMPLETE_END; ++i)
		m_pTexture[i] = NULL;

	m_eTexNum = COMPLETE_TXT;

	FAILED_CHECK(AddComponent());

	Set_TextureInit(COMPLETE_BACK, WINCX / 2.f, WINCY / 4.06885f, WINCX / 2.8f, WINCY / 20.6f);
	Set_TextureInit(COMPLETE_MARK, WINCX / 3.08392f, WINCY / 4.06885f, WINCX / 20.6f, WINCY / 13.4f);
	Set_TextureInit(COMPLETE_TXT, WINCX / 2.f, WINCY / 4.06885f, WINCX / 5.3f, WINCY / 20.6f);

	//m_fOriSizeX = WINCX / 10.27f;
	//m_fOrifX = WINCX / 2.038f;
	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CMoveQComplete::Update(const _float& fTimeDelta)
{/*
	if(m_bNewQuest)
		return 0;*/
	
	//Move_Tex();
	//Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);

	if(fTimeDelta < 1.f)
		m_fCurTime += fTimeDelta;


	if(m_fCurTime > m_fMaxTime)
		m_bNewQuest = true;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CMoveQComplete::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < COMPLETE_END; ++i)
	{
		if(m_pTexture[i] == NULL)
			continue;

		pEffect->SetTechnique("Default_Device");
		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
		pEffect->SetMatrix("g_matView",&m_matView[i]);
		pEffect->SetMatrix("g_matProj",&m_matProj);

		pEffect->SetFloat("g_fAlpha", 1.f - (m_fCurTime / m_fMaxTime));



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

void CMoveQComplete::Free(void)
{
	CUI::Free();
}

HRESULT CMoveQComplete::AddComponent(void)
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
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_Complete_Back");
	m_pTexture[COMPLETE_BACK] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[COMPLETE_BACK], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_Complete_Back", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_EndMark");
	m_pTexture[COMPLETE_MARK] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[COMPLETE_MARK], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_EndMark", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_BossWidow_MoveCompleteTxt");
	m_pTexture[COMPLETE_TXT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[COMPLETE_TXT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_BossWidow_MoveCompleteTxt", pComponent);


	return S_OK;
}

CMoveQComplete* CMoveQComplete::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMoveQComplete*		pGameObject = new CMoveQComplete(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CMoveQComplete::Set_TextureInit(eComplete eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CMoveQComplete::Set_TexView(void)
{
	for(int i = 0; i < COMPLETE_END; ++i)
	{
		D3DXMatrixIdentity(&m_matView[i]);	

		m_matView[i]._11 = m_fSizeX[i];
		m_matView[i]._22 = m_fSizeY[i];
		m_matView[i]._33 = 1.f;

		m_matView[i]._41 = m_fX[i] - (WINCX >> 1);
		m_matView[i]._42 = -m_fY[i] + (WINCY >> 1);
	}
}

void CMoveQComplete::Init_MoveTex(eComplete eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CMoveQComplete::Move_Tex(void)
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
