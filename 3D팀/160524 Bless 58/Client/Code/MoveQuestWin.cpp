#include "stdafx.h"
#include "MoveQuestWin.h"

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
#include "QuestComplete1.h"
#include "MoveQStart.h"

CMoveQuestWin::CMoveQuestWin(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fCurTime(0.f)
, m_fMaxTime(5.f)
{

}

CMoveQuestWin::~CMoveQuestWin(void)
{

}

HRESULT CMoveQuestWin::Initialize(void)
{
	for(int i = 0 ; i < QUESTWIN_END; ++i)
		m_pTexture[i] = NULL;

	m_eTexNum = QUESTWIN_MAINTXT;

	FAILED_CHECK(AddComponent());
	          

	Set_TextureInit(QUESTWIN_MAINTXTOUTLINE, WINCX / 1.11f, WINCY / 4.26f, WINCX / 12.f, WINCY / 49.7998f);
	Set_TextureInit(QUESTWIN_MAINTXT, WINCX / 1.122f, WINCY / 4.21f, WINCX / 11.76f, WINCY / 44.9878f);
	Set_TextureInit(QUESTWIN_MAINMARK, WINCX / 1.21f, WINCY / 4.26f, WINCX / 41.5f, WINCY / 29.8001f);

	Set_TextureInit(QUESTWIN_MAINQUEST1, WINCX / 1.126f, WINCY / 3.679f, WINCX / 12.76f, WINCY / 44.8878f);

	//m_fOriSizeX = WINCX / 10.27f;
	//m_fOrifX = WINCX / 2.038f;
	
	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CMoveQuestWin::Update(const _float& fTimeDelta)
{
	if(((CMoveQStart*)(Engine::Get_ObjList(LAYER_UI, L"MoveQStart")->front()))->Get_NewQuest() == false)
		return 0;

	//Move_Tex();
	//Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);


	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CMoveQuestWin::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < QUESTWIN_END; ++i)
	{
		if(m_pTexture[i] == NULL)
			continue;

		pEffect->SetTechnique("Default_Device");
		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
		pEffect->SetMatrix("g_matView",&m_matView[i]);
		pEffect->SetMatrix("g_matProj",&m_matProj);

		pEffect->SetFloat("g_fAlpha", 1.f);

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

void CMoveQuestWin::Free(void)
{
	CUI::Free();
}

HRESULT CMoveQuestWin::AddComponent(void)
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
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_QuestWindowTxt");
	m_pTexture[QUESTWIN_MAINTXTOUTLINE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINTXTOUTLINE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_SubWidow_QuestWindowTxt", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_SubWidow_SubTxt");
	m_pTexture[QUESTWIN_MAINTXT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINTXT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_SubWidow_SubTxt", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_QuestMark");
	m_pTexture[QUESTWIN_MAINMARK] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINMARK], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_SubWidow_QuestMark", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_SubWidow_SubQuest");
	m_pTexture[QUESTWIN_MAINQUEST1] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST1], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_SubWidow_SubQuest", pComponent);

	return S_OK;
}

CMoveQuestWin* CMoveQuestWin::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMoveQuestWin*		pGameObject = new CMoveQuestWin(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CMoveQuestWin::Set_TextureInit(eQuest eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CMoveQuestWin::Set_TexView(void)
{
	for(int i = 0; i < QUESTWIN_END; ++i)
	{
		D3DXMatrixIdentity(&m_matView[i]);	

		m_matView[i]._11 = m_fSizeX[i];
		m_matView[i]._22 = m_fSizeY[i];
		m_matView[i]._33 = 1.f;

		m_matView[i]._41 = m_fX[i] - (WINCX >> 1);
		m_matView[i]._42 = -m_fY[i] + (WINCY >> 1);
	}
}

void CMoveQuestWin::Init_MoveTex(eQuest eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CMoveQuestWin::Move_Tex(void)
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
