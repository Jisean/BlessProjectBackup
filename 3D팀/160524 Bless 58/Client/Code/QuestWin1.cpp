#include "stdafx.h"
#include "QuestWin1.h"

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
#include "Party.h"
#include "QuestMark.h"
#include "SoundMgr.h"
#include "MainQStart.h"

CQuestWin1::CQuestWin1(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fMaxMonster(10.f)
, m_fCurMonster(0.f)
{

}

CQuestWin1::~CQuestWin1(void)
{

}

HRESULT CQuestWin1::Initialize(void)
{
	for(int i = 0 ; i < QUESTWIN_END; ++i)
	{
		m_pTexture[i] = NULL;
		m_bQuestCK[i] = false;
	}

	for(int i = 0; i < QUESTCK_END; ++i)
	{
		m_bQuestGroup[i] = false;
		m_fCompleteQuestAlpha[i] = 0.f;
	}

	m_eTexNum = QUESTWIN_MAINTXT;

	FAILED_CHECK(AddComponent());   
   
	Set_TextureInit(QUESTWIN_MAINTXTOUTLINE, WINCX / 1.11f, WINCY / 4.26f, WINCX / 12.f, WINCY / 49.7998f);
	Set_TextureInit(QUESTWIN_MAINTXT, WINCX / 1.13f, WINCY / 4.21f, WINCX / 12.16f, WINCY / 39.1879f);
	Set_TextureInit(QUESTWIN_MAINMARK, WINCX / 1.21f, WINCY / 4.26f, WINCX / 41.5f, WINCY / 29.8001f);

	Set_TextureInit(QUESTWIN_MAINQUEST1, WINCX / 1.126f, WINCY / 3.679f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_MAINQUEST1S, WINCX / 1.034f, WINCY / 3.685f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_MAINQUEST2, WINCX / 1.126f, WINCY / 3.3f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_MAINQUEST2S, WINCX / 1.034f, WINCY / 3.294f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_MAINQUEST3, WINCX / 1.126f, WINCY / 3.f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_MAINQUEST3S, WINCX / 1.034f, WINCY / 2.994f, WINCX / 12.76f, WINCY / 44.8878f);

	Set_TextureInit(QUESTWIN_SUBQUEST, WINCX / 1.118f, WINCY / 2.75396f, WINCX / 12.76f, WINCY / 44.8878f);
	Set_TextureInit(QUESTWIN_SUBQUESTS, WINCX / 1.02f, WINCY / 2.74796f, WINCX / 12.76f, WINCY / 44.8878f);

	Set_TextureInit(QUESTWIN_MAINQUEST1T, WINCX / 2.f, WINCY / 4.87002f, WINCX / 11.2f, WINCY / 46.0381f);
	Set_TextureInit(QUESTWIN_MAINQUEST2T, WINCX / 2.f, WINCY / 4.87002f, WINCX / 11.2f, WINCY / 46.0381f);
	Set_TextureInit(QUESTWIN_MAINQUEST3T, WINCX / 2.f, WINCY / 4.87002f, WINCX / 11.2f, WINCY / 46.0381f);
	Set_TextureInit(QUESTWIN_SUBQESTT, WINCX / 2.f, WINCY / 4.87002f, WINCX / 11.2f, WINCY / 46.0381f);

	//m_fCurMonster = 10.f;

	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CQuestWin1::Update(const _float& fTimeDelta)
{
	//if(CKeyMgr::GetInstance()->GetKeyDown('L'))
	//{
	//	g_bMainQuest = true;
	//	Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
	//	((CDynamicCamera*)pCamera)->Set_MouseFix(false);
	//}

	CMainPlayer* m_pMainPlayer = (CMainPlayer*)((CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());

	if(	*m_pMainPlayer->Get_QuestStep() != CMainPlayer::QUEST_PROGRESS || 
		*m_pMainPlayer->Get_QuestType() != CMainPlayer::QUESTTYPE_MAIN)
	{
		return 0;
	}

	if(((CMainQStart*)(Engine::Get_ObjList(LAYER_UI, L"MainQStart")->front()))->Get_NewQuest() == false)
		return 0;


	if(CKeyMgr::GetInstance()->GetKeyDown(VK_F11))
		Cheat_QeustComplete();

	// 퀘스트 완료 체크용
	if(CKeyMgr::GetInstance()->GetKeyDown(VK_F10))
		m_bQuestGroup[QUESTCK_SUB] = true;

	//Move_Tex();
	//Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);


	QuestCheck();
	CompleteAlpha(fTimeDelta);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CQuestWin1::Render(void)
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

		if(m_bQuestCK[i])
			pEffect->SetTechnique("Quest");
		else
		{
			pEffect->SetTechnique("Default_Device");

			if(i == QUESTWIN_MAINQUEST1S || i == QUESTWIN_MAINQUEST2S || i == QUESTWIN_MAINQUEST3S || i == QUESTWIN_SUBQUESTS)
				continue;
		}
		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
		pEffect->SetMatrix("g_matView",&m_matView[i]);
		pEffect->SetMatrix("g_matProj",&m_matProj);
																										
		if(i == QUESTWIN_MAINQUEST1T || i == QUESTWIN_MAINQUEST2T || i == QUESTWIN_MAINQUEST3T || i == QUESTWIN_SUBQESTT)
			pEffect->SetFloat("g_fAlpha", Get_CompleteQuestAlpha((eQuest)i));
		else
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

void CQuestWin1::Free(void)
{
	CUI::Free();
}

HRESULT CQuestWin1::AddComponent(void)
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
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_QuestWindowTxt", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_MainTxt");
	m_pTexture[QUESTWIN_MAINTXT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINTXT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_MainTxt", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_QuestMark");
	m_pTexture[QUESTWIN_MAINMARK] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINMARK], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_QuestMark", pComponent);


	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_MainPaladin");
	m_pTexture[QUESTWIN_MAINQUEST1] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST1], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_MainPaladin", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompletePaladin");
	m_pTexture[QUESTWIN_MAINQUEST1T] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST1T], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompletePaladin", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompleteTxt");
	m_pTexture[QUESTWIN_MAINQUEST1S] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST1S], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompleteTxt1", pComponent);


	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_MainMagician");
	m_pTexture[QUESTWIN_MAINQUEST2] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST2], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_MainMagician", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompleteMagician");
	m_pTexture[QUESTWIN_MAINQUEST2T] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST2T], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompleteMagician", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompleteTxt");
	m_pTexture[QUESTWIN_MAINQUEST2S] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST2S], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompleteTxt2", pComponent);


	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_MainRanger");
	m_pTexture[QUESTWIN_MAINQUEST3] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST3], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_MainRanger", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompleteRanger");
	m_pTexture[QUESTWIN_MAINQUEST3T] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST3T], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompleteRanger", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompleteTxt");
	m_pTexture[QUESTWIN_MAINQUEST3S] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_MAINQUEST3S], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompleteTxt3", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_SubHunt");
	m_pTexture[QUESTWIN_SUBQUEST] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_SUBQUEST], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_SubHunt", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompleteTxt");
	m_pTexture[QUESTWIN_SUBQUESTS] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_SUBQUESTS], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompleteTxtSub", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_CompleteHunt");
	m_pTexture[QUESTWIN_SUBQESTT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUESTWIN_SUBQESTT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_MainWidow_CompleteHunt", pComponent);

	return S_OK;
}

CQuestWin1* CQuestWin1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CQuestWin1*		pGameObject = new CQuestWin1(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CQuestWin1::Set_MainQuestCheck(CPlayers::PLAYERJOB eJob, bool bMonsterHunt/* = false*/)
{
	int iCnt = 0;
	float fAlpha = 1.f;
	if(m_bQuestGroup[QUESTCK_MAINQ1] == true)
		++iCnt;
	if(m_bQuestGroup[QUESTCK_MAINQ2] == true)
		++iCnt;
	if(m_bQuestGroup[QUESTCK_MAINQ3] == true)
		++iCnt;
	if(m_bQuestGroup[QUESTCK_SUB] == true)
		++iCnt;

	if(iCnt == 3)
		fAlpha = 0.f;

	CQuestMark* pMark = (CQuestMark*)(Engine::Get_ObjList(LAYER_UI, L"QuestMark")->front());
	CParty* pParty = (CParty*)(Engine::Get_ObjList(LAYER_UI, L"Party")->front());
	CMainPlayer* pMainPlayer = (CMainPlayer*)((CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());
	if(bMonsterHunt)
	{
		m_bQuestGroup[QUESTCK_SUB] = true;
		m_fCompleteQuestAlpha[QUESTCK_SUB] = fAlpha;
	}
	else if(eJob == CPlayers::JOB_PALADIN)
	{
		m_bQuestGroup[QUESTCK_MAINQ1] = true;
		pParty->Add_PlayerParty(CPlayers::JOB_PALADIN, QuestPosCnt());
		pMark->Set_Alpha(CQuestMark::QUEST_1, 0.f);
		m_fCompleteQuestAlpha[QUESTCK_MAINQ1] = fAlpha;

		//_myvec vPos = pMainPlayer->Get_PlayerSet(CPlayers::JOB_PALADIN)->GetInfo()->m_vPos;
		m_vQuestEffect = ((CPlayers*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"Paladin")->front()))->GetInfo()->m_vPos;
		Engine::Call_EtcFX(Engine::ETC_QUEST, &m_vQuestEffect, true);
		
	}
	else if(eJob == CPlayers::JOB_MAGICIAN)
	{
		m_bQuestGroup[QUESTCK_MAINQ2] = true;
		pParty->Add_PlayerParty(CPlayers::JOB_MAGICIAN, QuestPosCnt());
		pMark->Set_Alpha(CQuestMark::QUEST_2, 0.f);
		m_fCompleteQuestAlpha[QUESTCK_MAINQ2] = fAlpha;

		m_vQuestEffect = ((CPlayers*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"Magician")->front()))->GetInfo()->m_vPos;
		Engine::Call_EtcFX(Engine::ETC_QUEST, &m_vQuestEffect, true);

	}
	else if(eJob == CPlayers::JOB_RANGER)
	{
		m_bQuestGroup[QUESTCK_MAINQ3] = true;
		pParty->Add_PlayerParty(CPlayers::JOB_RANGER, QuestPosCnt());
		pMark->Set_Alpha(CQuestMark::QUEST_3, 0.f);
		m_fCompleteQuestAlpha[QUESTCK_MAINQ3] = fAlpha;

		m_vQuestEffect = ((CPlayers*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ranger")->front()))->GetInfo()->m_vPos;
		Engine::Call_EtcFX(Engine::ETC_QUEST, &m_vQuestEffect, true);
	}
}

bool CQuestWin1::Get_MainQuestCheck(CPlayers::PLAYERJOB eJob, bool bMonsterHunt/* = false*/)
{
	if(bMonsterHunt)
		return m_bQuestGroup[QUESTCK_SUB];
	else if(eJob == CPlayers::JOB_PALADIN)
		return m_bQuestGroup[QUESTCK_MAINQ1];
	else if(eJob == CPlayers::JOB_MAGICIAN)
		return m_bQuestGroup[QUESTCK_MAINQ2];
	else if(eJob == CPlayers::JOB_RANGER)
		return m_bQuestGroup[QUESTCK_MAINQ3];

	return false;
}

void CQuestWin1::Set_TextureInit(eQuest eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CQuestWin1::Set_TexView(void)
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

void CQuestWin1::QuestCheck(void)
{
	if(m_bQuestGroup[QUESTCK_MAINQ1])
	{
		m_bQuestCK[QUESTWIN_MAINQUEST1] = true;
		m_bQuestCK[QUESTWIN_MAINQUEST1S] = true;
	}
	if(m_bQuestGroup[QUESTCK_MAINQ2])
	{
		m_bQuestCK[QUESTWIN_MAINQUEST2] = true;
		m_bQuestCK[QUESTWIN_MAINQUEST2S] = true;
	}
	if(m_bQuestGroup[QUESTCK_MAINQ3])
	{
		m_bQuestCK[QUESTWIN_MAINQUEST3] = true;
		m_bQuestCK[QUESTWIN_MAINQUEST3S] = true;
	}
	if(m_bQuestGroup[QUESTCK_SUB])
	{
		m_bQuestCK[QUESTWIN_SUBQUEST] = true;
		m_bQuestCK[QUESTWIN_SUBQUESTS] = true;
	}


	if(m_bQuestGroup[QUESTCK_MAINQ1] && m_bQuestGroup[QUESTCK_MAINQ2] && 
		m_bQuestGroup[QUESTCK_MAINQ3] && m_bQuestGroup[QUESTCK_SUB])
	{
		CMainPlayer* m_pMainPlayer = (CMainPlayer*)((CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());

		*m_pMainPlayer->Get_QuestStep() = CMainPlayer::QUEST_COMPLETE;
	}
}

int CQuestWin1::QuestPosCnt(void)
{
	int iCnt = -1;
	if(m_bQuestGroup[QUESTCK_MAINQ1])
	{
		CSoundMgr::GetInstance()->PlayQuestSound(L"MiddleQuestEnd.ogg");
		++iCnt;
	}
	if(m_bQuestGroup[QUESTCK_MAINQ2])
	{
		CSoundMgr::GetInstance()->PlayQuestSound(L"MiddleQuestEnd.ogg");
		++iCnt;
	}
	if(m_bQuestGroup[QUESTCK_MAINQ3])
	{
		CSoundMgr::GetInstance()->PlayQuestSound(L"MiddleQuestEnd.ogg");
		++iCnt;
	}

	return iCnt;
}

void CQuestWin1::CompleteAlpha(float fTimeDelta)
{
	for(int i = 0; i < QUESTCK_END; ++i)
	{
		if(m_fCompleteQuestAlpha[i] > 0.f)
			m_fCompleteQuestAlpha[i] -= fTimeDelta * 0.5f;
	}
}

float CQuestWin1::Get_CompleteQuestAlpha(eQuest eQuestName)
{
	if(eQuestName == QUESTWIN_MAINQUEST1T)
		return m_fCompleteQuestAlpha[0];
	else if(eQuestName == QUESTWIN_MAINQUEST2T)
		return m_fCompleteQuestAlpha[1];
	else if(eQuestName == QUESTWIN_MAINQUEST3T)
		return m_fCompleteQuestAlpha[2];
	else if(eQuestName == QUESTWIN_SUBQESTT)
		return m_fCompleteQuestAlpha[3];

	return 0.f;
}

void CQuestWin1::Cheat_QeustComplete(void)
{
	CParty* pParty = (CParty*)(Engine::Get_ObjList(LAYER_UI, L"Party")->front());

	m_bQuestGroup[QUESTCK_MAINQ1] = true;
	pParty->Add_PlayerParty(CPlayers::JOB_PALADIN, QuestPosCnt());
		
	m_bQuestGroup[QUESTCK_MAINQ2] = true;
	pParty->Add_PlayerParty(CPlayers::JOB_MAGICIAN, QuestPosCnt());
		
	m_bQuestGroup[QUESTCK_MAINQ3] = true;
	pParty->Add_PlayerParty(CPlayers::JOB_RANGER, QuestPosCnt());
	
}

void CQuestWin1::Init_MoveTex(eQuest eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CQuestWin1::Move_Tex(void)
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
