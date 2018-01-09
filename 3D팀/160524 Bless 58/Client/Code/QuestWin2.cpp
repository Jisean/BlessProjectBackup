#include "stdafx.h"
#include "QuestWin2.h"

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
#include "Ghost_Knight.h"
#include "SoundMgr.h"
#include "BossQStart.h"
#include "AIMgr.h"
#include "NPCs.h"
#include "SaveQMark.h"

CQuestWin2::CQuestWin2(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fCurTime(0.f)
, m_fMaxTime(5.f)
, m_bStart(false)
, m_sNum(0)
, m_fSaveQAlpha(1.f)
{

}

CQuestWin2::~CQuestWin2(void)
{

}

HRESULT CQuestWin2::Initialize(void)
{
	for(int i = 0 ; i < QUESTWIN_END; ++i)
		m_pTexture[i] = NULL;

	m_eTexNum = QUESTWIN_SAVEQCOMPLETE;

	FAILED_CHECK(AddComponent());

	                
	Set_TextureInit(QUESTWIN_MAINTXTOUTLINE, WINCX / 1.11f, WINCY / 4.26f, WINCX / 12.f, WINCY / 49.7998f);
	Set_TextureInit(QUESTWIN_MAINTXT, WINCX / 1.139f, WINCY / 4.205f, WINCX / 11.76f, WINCY / 44.9878f);
	Set_TextureInit(QUESTWIN_MAINMARK, WINCX / 1.21f, WINCY / 4.26f, WINCX / 41.5f, WINCY / 29.8001f);


	Set_TextureInit(QUESTWIN_SAVEQCOMPLETE, WINCX / 1.112f, WINCY / 3.679f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_SAVEQUESTOUTLINE, WINCX / 1.112f, WINCY / 3.679f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_SAVEQUESTNUM, WINCX / 1.112f, WINCY / 3.687f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_SAVET, WINCX / 2.f, WINCY / 4.87002f, WINCX / 11.2f, WINCY / 46.0381f);

	Set_TextureInit(QUESTWIN_MAINQUEST1, WINCX / 1.126f, WINCY / 3.26803f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_MAINQUEST2, WINCX / 1.148f, WINCY / 3.02704f, WINCX / 12.76f, WINCY / 44.8878f);

	//m_fOriSizeX = WINCX / 10.27f;
	//m_fOrifX = WINCX / 2.038f;

	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CQuestWin2::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if(m_bStart == false)
	{
		CSoundMgr::GetInstance()->PlayQuestSound(L"MiddleQuestEnd.ogg");
		CMainPlayer* m_pMainPlayer = (CMainPlayer*)((CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());

		m_bStart = true;
	}

	if(((CBossQStart*)(Engine::Get_ObjList(LAYER_UI, L"BossQStart")->front()))->Get_NewQuest() == false)
		return 0;

	Set_QuestNpc();

	//CGhost_Knight* pBoss = ((CGhost_Knight*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ghost_Knight")->front()));

	//if(((CQuestComplete1*)(Engine::Get_ObjList(LAYER_UI, L"MoveQComplete")->front()))->Get_NewQuest() == false ||
	//	pBoss->Get_bDieState() == true)
	//	return 0;

	//if(CKeyMgr::GetInstance()->GetKeyDown('X'))
	//	const_cast<Engine::CStatus*>(pBoss->GetStatus())->m_tRealStat.m_fCurHp = 0.f;


	/*Move_Tex();
	Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);*/


	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CQuestWin2::Render(void)
{
	/*if(CKeyMgr::GetInstance()->GetKeyDown('C'))
		++m_sNum;*/

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < QUESTWIN_END; ++i)
	{
		if(m_pTexture[i] == NULL)
			continue;

		if(m_sNum == 5 && (i == QUESTWIN_SAVEQCOMPLETE))
			pEffect->SetTechnique("Quest");
		else
			pEffect->SetTechnique("Default_Device");
		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
		pEffect->SetMatrix("g_matView",&m_matView[i]);
		pEffect->SetMatrix("g_matProj",&m_matProj);

		if(i == QUESTWIN_SAVET)
			pEffect->SetFloat("g_fAlpha", CompleteAlpha(m_fTimeDelta, m_sNum));
		else
			pEffect->SetFloat("g_fAlpha", 1.f);

		pEffect->SetFloat("g_fRGB", 1.f);
		pEffect->SetFloat("g_fPointPower", 1.f);

		m_pTexture[i]->Set_ConstantTable(pEffect,"g_BaseTexture");

		if(m_sNum < 5)
		{
			if(i == QUESTWIN_SAVEQUESTNUM)
				m_pTexture[i]->Set_ConstantTable(pEffect,"g_BaseTexture", m_sNum);

			if(i == QUESTWIN_SAVEQCOMPLETE)
				pEffect->SetFloat("g_fAlpha", 0.f);
		}
		else if(m_sNum == 5)
		{
			if(i == QUESTWIN_SAVEQUESTOUTLINE || i == QUESTWIN_SAVEQUESTNUM)
				pEffect->SetFloat("g_fAlpha", 0.f);

			if(i == QUESTWIN_SAVEQCOMPLETE)
				pEffect->SetFloat("g_fAlpha", 1.f);
		}

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

void CQuestWin2::Free(void)
{
	CUI::Free();
}

HRESULT CQuestWin2::AddComponent(void)
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

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_BossWidow_MainTxt");
	m_pTexture[QUESTWIN_MAINTXT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINTXT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_BossWidow_MainTxt", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_QuestMark");
	m_pTexture[QUESTWIN_MAINMARK] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINMARK], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_SubWidow_QuestMark", pComponent);



	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_SaveQuest_Complete");
	m_pTexture[QUESTWIN_SAVEQCOMPLETE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_SAVEQCOMPLETE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_SaveQuest_Complete", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_SaveQuest_OutLine");
	m_pTexture[QUESTWIN_SAVEQUESTOUTLINE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_SAVEQUESTOUTLINE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_SaveQuest_OutLine", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_SaveQuest_Num");
	m_pTexture[QUESTWIN_SAVEQUESTNUM] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_SAVEQUESTNUM], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_SaveQuest_Num", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_SaveQuest_CompleteTxt");
	m_pTexture[QUESTWIN_SAVET] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_SAVET], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_SaveQuest_CompleteTxt", pComponent);




	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_BossWidow_Quest1");
	m_pTexture[QUESTWIN_MAINQUEST1] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST1], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_BossWidow_Quest1", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_BossWidow_Quest2");
	m_pTexture[QUESTWIN_MAINQUEST2] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST2], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_BossWidow_Quest2", pComponent);

	return S_OK;
}

CQuestWin2* CQuestWin2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CQuestWin2*		pGameObject = new CQuestWin2(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CQuestWin2::Set_TextureInit(eQuest eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CQuestWin2::Set_TexView(void)
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

float CQuestWin2::CompleteAlpha(float fTimeDelta, int iComplete)
{
	if(iComplete == 5)
		return m_fSaveQAlpha -= fTimeDelta * 0.5f;
	else
		return 0.f;
}

void CQuestWin2::Set_QuestNpc(void)
{
	m_sNum = 0;

	vector<CNPCs*>::iterator iternpc = CAIMgr::GetInstance()->Get_NPCInfo()->begin();
	vector<CNPCs*>::iterator iternpc_end = CAIMgr::GetInstance()->Get_NPCInfo()->end();

	CSaveQMark* pMark = (CSaveQMark*)(Engine::Get_ObjList(LAYER_UI, L"SaveQMark")->front());

	if(pMark == NULL)
		return;

	int i = 0;
	for(iternpc; iternpc < iternpc_end; ++iternpc, ++i)
	{
		if((*iternpc)->Get_bDungeonNPC())
		{
			++m_sNum;
			pMark->Set_Alpha((CSaveQMark::eQuest)i, 0.f);
		}
	}
}

void CQuestWin2::Init_MoveTex(eQuest eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CQuestWin2::Move_Tex(void)
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
