#include "stdafx.h"
#include "QuestNpc.h"

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
#include "SoundMgr.h"
#include "..\Engine\Utility\Code\Light.h"

CQuestNpc::CQuestNpc(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fCurTime(0.f)
, m_fMaxTime(2.f)
, m_fLightPower(0.7f)
{

}

CQuestNpc::~CQuestNpc(void)
{

}

HRESULT CQuestNpc::Initialize(void)
{
	for(int i = 0 ; i < QUEST_END; ++i)
		m_pTexture[i] = NULL;

	//m_eTexNum = QUEST_TXT;

	FAILED_CHECK(AddComponent());


	Set_TextureInit(QUEST_BACK, WINCX / 2.f, WINCY / 2.f, WINCX / 2.f, WINCY / 2.f);
	Set_TextureInit(QUEST_TXT, WINCX / 2.f, WINCY / 1.131f, WINCX / 4.6f, WINCY / 54.5997f);
	Set_TextureInit(QUEST_BUTTON, WINCX / 1.107f, WINCY / 1.046f, WINCX / 11.6f, WINCY / 42.7999f);
	Set_TextureInit(QUEST_ACCEPT, WINCX / 1.108f, WINCY / 1.044f, WINCX / 27.4001f, WINCY / 81.1993f);


	MakeRect(m_rcAccept, QUEST_BUTTON);

	//m_fOriSizeX = WINCX / 10.27f;
	//m_fOrifX = WINCX / 2.038f;
	//Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CQuestNpc::Update(const _float& fTimeDelta)
{
	if(CKeyMgr::GetInstance()->GetKeyDown('L'))
	{
		g_bMainQuest = true;
		Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
		((CDynamicCamera*)pCamera)->Set_MouseFix(false);
	}

	CMainPlayer* m_pMainPlayer = (CMainPlayer*)((CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());

	if(!g_bMainQuest ||
		*m_pMainPlayer->Get_QuestStep() != CMainPlayer::QUEST_BEFORE || 
		*m_pMainPlayer->Get_QuestType() != CMainPlayer::QUESTTYPE_MAIN)
		return 0;

	//Move_Tex();
	//Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);

	m_fCurTime += fTimeDelta;

	if(CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if(PtInRect(&m_rcAccept, ptMouse))
		{
			g_bMainQuest = false;
			Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
			((CDynamicCamera*)pCamera)->Set_MouseFix(true);

			*m_pMainPlayer->Get_QuestStep() = CMainPlayer::QUEST_PROGRESS;
		
			COption* pOption = (COption*)(Engine::Get_ObjList(LAYER_UI, L"Option")->front());
			*(Engine::Set_RendererLightPower()) = m_fLightPower;
			pOption->Set_OldLightPower(m_fLightPower);

			pOption->Adjust_Light();
		}
	}
	else if(CKeyMgr::GetInstance()->GetKeyDown(VK_SPACE))
	{
		g_bMainQuest = false;
		CSoundMgr::GetInstance()->PlayQuestSound(L"FirstQuestAccept.ogg");
		Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
		((CDynamicCamera*)pCamera)->Set_MouseFix(true);

		*m_pMainPlayer->Get_QuestStep() = CMainPlayer::QUEST_PROGRESS;

		COption* pOption = (COption*)(Engine::Get_ObjList(LAYER_UI, L"Option")->front());
		*(Engine::Set_RendererLightPower()) = m_fLightPower;
		pOption->Set_OldLightPower(m_fLightPower);
		pOption->Adjust_Light();

		Engine::CLight* pLight = Engine::Get_Light(L"Directional", 0);
		pLight->Set_LightInfo()->Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	}

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CQuestNpc::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < QUEST_END; ++i)
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

		if(i == QUEST_TXT)
		{
			pEffect->SetTechnique("Txt");
			float fTemp = m_fCurTime / m_fMaxTime;

			pEffect->SetFloat("g_fColor", fTemp);
			m_pGradationTex->Set_ConstantTable(pEffect, "g_FilterTexture");
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

void CQuestNpc::Free(void)
{
	CUI::Free();
}

HRESULT CQuestNpc::AddComponent(void)
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
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_Main_QuestBack");
	m_pTexture[QUEST_BACK] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUEST_BACK], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_Main_QuestBack", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_Main_QuestTxt");
	m_pTexture[QUEST_TXT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUEST_TXT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_Main_QuestTxt", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_Main_Button");
	m_pTexture[QUEST_BUTTON] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUEST_BUTTON], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_Main_Button", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_Main_Accept");
	m_pTexture[QUEST_ACCEPT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[QUEST_ACCEPT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_Main_Accept", pComponent);


	//Gradation
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_Main_TxtGradation");
	m_pGradationTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pGradationTex, E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Quest_Main_TxtGradation", pComponent);


	return S_OK;
}

CQuestNpc* CQuestNpc::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CQuestNpc*		pGameObject = new CQuestNpc(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CQuestNpc::Set_TextureInit(eQuest eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CQuestNpc::Set_TexView(void)
{
	for(int i = 0; i < QUEST_END; ++i)
	{
		D3DXMatrixIdentity(&m_matView[i]);	

		m_matView[i]._11 = m_fSizeX[i];
		m_matView[i]._22 = m_fSizeY[i];
		m_matView[i]._33 = 1.f;

		m_matView[i]._41 = m_fX[i] - (WINCX >> 1);
		m_matView[i]._42 = -m_fY[i] + (WINCY >> 1);
	}
}

void CQuestNpc::MakeRect(RECT& rc, eQuest eTexNum, int iAdd /*= 0*/)
{
	rc.left = long(m_fX[eTexNum] - (m_fSizeX[eTexNum] + iAdd) * 0.5f);
	rc.top =  long(m_fY[eTexNum] - (m_fSizeY[eTexNum] + iAdd) * 0.5f);
	rc.right = long(m_fX[eTexNum] + (m_fSizeX[eTexNum] + iAdd) * 0.5f);
	rc.bottom = long(m_fY[eTexNum] + (m_fSizeY[eTexNum] + iAdd) * 0.5f);
}

void CQuestNpc::Init_MoveTex(eQuest eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CQuestNpc::Move_Tex(void)
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

	//cout << m_MoveX << "\t" << m_MoveY << "\t" << m_MoveSizeX << "\t" << m_MoveSizeY << endl;
}
