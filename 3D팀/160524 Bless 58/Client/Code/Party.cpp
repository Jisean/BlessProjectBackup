#include "stdafx.h"
#include "Party.h"

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

CParty::CParty(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
{

}

CParty::~CParty(void)
{

}

HRESULT CParty::Initialize(void)
{
	for(int i = 0 ; i < PARTY_END; ++i)
		m_pTexture[i] = NULL;

	for(int i = 0; i < 3; ++i)
		m_eOtherPlayer[i] = CPlayers::JOB_END;

	for(int i = 0; i < CPlayers::JOB_END; ++i)
		m_bCenterCrest[i] = false;

	m_eTexNum = CENTER_GUARDIAN;

	FAILED_CHECK(AddComponent());

	// Outline
	Init_Outline();
      
	// Bar    
	Init_Bar();
     
	Set_TextureInit(CENTER_GUARDIAN, WINCX / 2.004f, WINCY / 1.11408f, WINCX / 50.9154f, WINCY / 32.3314f);
	Set_TextureInit(CENTER_MAGICIAN, WINCX / 2.004f, WINCY / 1.11408f, WINCX / 50.9154f, WINCY / 32.3314f);
	Set_TextureInit(CENTER_PALADIN, WINCX / 2.004f, WINCY / 1.11408f, WINCX / 50.9154f, WINCY / 32.3314f);
	Set_TextureInit(CENTER_RANGER, WINCX / 2.004f, WINCY / 1.11408f, WINCX / 50.9154f, WINCY / 32.3314f);

	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CParty::Update(const _float& fTimeDelta)
{
	if(g_bMainQuest)
		return 0;

	if(m_bStart)
	{
		Init_Players();
		m_bStart = false;
	}

	if(g_bMainQuest)
		return 0;

	Set_Player();
	Set_Bar();
	CenterCrest();

	//if(Engine::GetDIKeyState(DIK_L))
	//	const_cast<Engine::CStatus*>(((CPlayers*)m_pMainPlayer)->GetStatus())->m_tRealStat.m_fCurHp -= 10.f;

	/*Move_Tex();
	Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);*/

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	
	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CParty::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < PARTY_END; ++i)
	{
		if(m_pTexture[i] == NULL)
			continue;


		if(m_eOtherPlayer[0] == CPlayers::JOB_END && (i == OUT_1HP || i == BAR_1HP || i == OUT_1MP || i == BAR_1MP || i == OUT_1RING))
			continue;

		if(m_eOtherPlayer[1] == CPlayers::JOB_END && (i == OUT_2HP || i == BAR_2HP || i == OUT_2MP || i == BAR_2MP || i == OUT_2RING))
			continue;

		if(m_eOtherPlayer[2] == CPlayers::JOB_END && (i == OUT_3HP || i == BAR_3HP || i == OUT_3MP || i == BAR_3MP || i == OUT_3RING))
			continue;

		if(!(Check_CenterCrest((eParty)i)))
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

void CParty::Free(void)
{
	CUI::Free();
}

HRESULT CParty::AddComponent(void)
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

	AddOutLineComponent();
	AddBarComponent();

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Crest_Guardian");
	m_pTexture[CREST_GUARDIAN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CREST_GUARDIAN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Crest_Guardian", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Crest_Magician");
	m_pTexture[CREST_MAGICIAN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CREST_MAGICIAN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Crest_Magician", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Crest_Paladin");
	m_pTexture[CREST_PALADIN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CREST_PALADIN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Crest_Paladin", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Crest_Ranger");
	m_pTexture[CREST_RANGER] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CREST_RANGER], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Crest_Ranger", pComponent);


	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Txt_Paladin");
	m_pTexture[NAME_PALADIN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[NAME_PALADIN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Txt_Paladin", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Txt_Magician");
	m_pTexture[NAME_MAGICIAN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[NAME_MAGICIAN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Txt_Magician", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Txt_Guardian");
	m_pTexture[NAME_GUARDIAN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[NAME_GUARDIAN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Txt_Guardian", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Txt_Ranger");
	m_pTexture[NAME_RANGER] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[NAME_RANGER], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Txt_Ranger", pComponent);

	return S_OK;
}

HRESULT CParty::AddOutLineComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	// Main Outline
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_MAINHP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_MAINHP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_OutlineMainHp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_MAINMP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_MAINMP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_OutlineMainMp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Ring");
	m_pTexture[OUT_MAINRING] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_MAINRING], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_RingMain", pComponent);

	// 1st Outline
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Ring");
	m_pTexture[OUT_1RING] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_1RING], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Ring1st", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_1HP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_1HP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Outline1stHp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_1MP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_1MP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Outline1stMP", pComponent);


	// 2nd Outline
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Ring");
	m_pTexture[OUT_2RING] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_2RING], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Ring2nd", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_2HP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_2HP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Outline2ndHp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_2MP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_2MP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Outline2ndMP", pComponent);


	// 3rd Outline
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Ring");
	m_pTexture[OUT_3RING] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_3RING], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Ring3rd", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_3HP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_3HP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Outline3rdHp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_Outline");
	m_pTexture[OUT_3MP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OUT_3MP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_Outline3rdMP", pComponent);

	return S_OK;

}

HRESULT CParty::AddBarComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	// Main Bar
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_HpBar");
	m_pTexture[BAR_MAINHP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_MAINHP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBarMainHp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_MpBar");
	m_pTexture[BAR_MAINMP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_MAINMP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBarMainMp", pComponent);


	// 1st Bar
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_HpBar");
	m_pTexture[BAR_1HP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_MAINHP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBar1Hp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_MpBar");
	m_pTexture[BAR_1MP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_1MP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBar1Mp", pComponent);


	// 2nd Bar
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_HpBar");
	m_pTexture[BAR_2HP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_2HP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBar2Hp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_MpBar");
	m_pTexture[BAR_2MP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_2MP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBar2Mp", pComponent);


	// 3rd Bar
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_HpBar");
	m_pTexture[BAR_3HP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_3HP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBardHp", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Party_MpBar");
	m_pTexture[BAR_3MP] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_3MP], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Party_HpBardMp", pComponent);



	// Center Crest
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_PlayerBar_CenterGuardian");
	m_pTexture[CENTER_GUARDIAN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CENTER_GUARDIAN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_PlayerBar_CenterGuardian", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_PlayerBar_CenterMagician");
	m_pTexture[CENTER_MAGICIAN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CENTER_MAGICIAN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_PlayerBar_CenterMagician", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_PlayerBar_CenterPaladin");
	m_pTexture[CENTER_PALADIN] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CENTER_PALADIN], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_PlayerBar_CenterPaladin", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_PlayerBar_CenterRanger");
	m_pTexture[CENTER_RANGER] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CENTER_RANGER], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_PlayerBar_CenterRanger", pComponent);


	return S_OK;
}

CParty* CParty::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CParty*		pGameObject = new CParty(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CParty::Add_PlayerParty(CPlayers::PLAYERJOB ePlayerJob, int iPos)
{
	m_eOtherPlayer[iPos] = ePlayerJob;
	Set_Crest(ePlayerJob, iPos + 1);
}

void CParty::Init_Outline(void)
{
	// Main OutLine
	Set_TextureInit(OUT_MAINHP, WINCX / 7.22007f, WINCY / 16.7502f, WINCX / 15.1f, WINCY / 148.9000f);
	Set_TextureInit(OUT_MAINMP, WINCX / 7.96009f, WINCY / 13.7502f, WINCX / 16.9f, WINCY / 187.1020f);
	Set_TextureInit(OUT_MAINRING, WINCX / 19.2103f, WINCY / 23.3502f, WINCX / 30.1f, WINCY / 21.4998f);

	// 1st OutLine
	Set_TextureInit(OUT_1RING, WINCX / 19.9603f, WINCY / 6.95014f, WINCX / 36.7999f, WINCY / 27.3998f);
	Set_TextureInit(OUT_1HP, WINCX / 7.96009f, WINCY / 6.25019f, WINCX / 16.9f, WINCY / 187.1020f);
	Set_TextureInit(OUT_1MP, WINCX / 8.83011f, WINCY / 5.95018f, WINCX / 21.5f, WINCY / 225.3040f);

	// 2nd OutLine
	Set_TextureInit(OUT_2RING, WINCX / 19.9603f, WINCY / 4.75014f, WINCX / 36.7999f, WINCY / 27.3998f);
	Set_TextureInit(OUT_2HP, WINCX / 7.96009f, WINCY / 4.45019f, WINCX / 16.9f, WINCY / 187.1020f);
	Set_TextureInit(OUT_2MP, WINCX / 8.83011f, WINCY / 4.29018f, WINCX / 21.5f, WINCY / 225.3040f);

	// 3rd OutLine
	Set_TextureInit(OUT_3RING, WINCX / 19.9603f, WINCY / 3.55014f, WINCX / 36.7999f, WINCY / 27.3998f);
	Set_TextureInit(OUT_3HP, WINCX / 7.96009f, WINCY / 3.38019f, WINCX / 16.9f, WINCY / 187.1020f);
	Set_TextureInit(OUT_3MP, WINCX / 8.83011f, WINCY / 3.28018f, WINCX / 21.5f, WINCY / 225.3040f);

}

void CParty::Init_Bar(void)
{
	m_fOriBarfX[_BAR_MAINHP] = WINCX / 7.24007f;
	m_fOriBarSizeX[_BAR_MAINHP] = WINCX / 15.1f;
	m_fOriBarfX[_BAR_MAINMP] = WINCX / 7.96009f;
	m_fOriBarSizeX[_BAR_MAINMP] = WINCX / 16.9f;
	Set_TextureInit(BAR_MAINHP, m_fOriBarfX[_BAR_MAINHP], WINCY / 16.0702f, m_fOriBarSizeX[_BAR_MAINHP], WINCY / 111.699f);
	Set_TextureInit(BAR_MAINMP, m_fOriBarfX[_BAR_MAINMP], WINCY / 13.5002f, m_fOriBarSizeX[_BAR_MAINMP], WINCY / 157.001f);

	m_fOriBarfX[_BAR_1HP] = WINCX / 7.96009f;
	m_fOriBarSizeX[_BAR_1HP] = WINCX / 16.9f;
	m_fOriBarfX[_BAR_1MP] = WINCX / 8.83011f;
	m_fOriBarSizeX[_BAR_1MP] = WINCX / 21.5f;
	Set_TextureInit(BAR_1HP, m_fOriBarfX[_BAR_1HP], WINCY / 6.20019f, 0, WINCY / 151.4f);
	Set_TextureInit(BAR_1MP, m_fOriBarfX[_BAR_1MP], WINCY / 5.90018f, 0, WINCY / 172.701f);

	m_fOriBarfX[_BAR_2HP] = WINCX / 7.96009f;
	m_fOriBarSizeX[_BAR_2HP] = WINCX / 16.9f;
	m_fOriBarfX[_BAR_2MP] = WINCX / 8.83011f;
	m_fOriBarSizeX[_BAR_2MP] = WINCX / 21.5f;
	Set_TextureInit(BAR_2HP, m_fOriBarfX[_BAR_2HP], WINCY / 4.42019f, 0, WINCY / 151.4f);
	Set_TextureInit(BAR_2MP, m_fOriBarfX[_BAR_2MP], WINCY / 4.26018f, 0, WINCY / 172.701f);

	m_fOriBarfX[_BAR_3HP] = WINCX / 7.96009f;
	m_fOriBarSizeX[_BAR_3HP] = WINCX / 16.9f;
	m_fOriBarfX[_BAR_3MP] = WINCX / 8.83011f;
	m_fOriBarSizeX[_BAR_3MP] = WINCX / 21.5f;
	Set_TextureInit(BAR_3HP, m_fOriBarfX[_BAR_3HP], WINCY / 3.36019f, 0, WINCY / 151.4f);
	Set_TextureInit(BAR_3MP, m_fOriBarfX[_BAR_3MP], WINCY / 3.26618f, 0, WINCY / 172.701f);
}

void CParty::Init_Players(void)
{
	m_pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());

	for(int i = 0; i < CPlayers::JOB_END; ++i)
	{
		m_pPlayer[i] = m_pMainPlayer->Get_PlayerSet(i);
	}

	m_eMainPlayer = m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob();
	Set_Crest(m_eMainPlayer, 0);
	m_eOldMainPlayer = m_eMainPlayer;


	//Add_PlayerParty(직업, int)
	/////////		퀘스트 창에서 바꾸게끔 함수로 해줄 것
	/*int i = 0;
	for(int iJob = 0; iJob < CPlayers::JOB_END; ++iJob)
	{
		if(m_eMainPlayer == iJob)
			continue;

		m_eOtherPlayer[i] = (CPlayers::PLAYERJOB)iJob;
		Set_Crest((CPlayers::PLAYERJOB)iJob, i + 1);
		++i;
	}*/
}

void CParty::Set_Crest(CPlayers::PLAYERJOB ePlayer, int iPos)
{
	eParty eCrest = PARTY_END;
	eParty eTxt = PARTY_END;

	if(ePlayer == CPlayers::JOB_GUARDIAN)
	{
		eCrest = CREST_GUARDIAN;
		eTxt = NAME_GUARDIAN;
	}
	else if(ePlayer == CPlayers::JOB_PALADIN)
	{
		eCrest = CREST_PALADIN;
		eTxt = NAME_PALADIN;
	}
	else if(ePlayer == CPlayers::JOB_MAGICIAN)
	{
		eCrest = CREST_MAGICIAN;
		eTxt = NAME_MAGICIAN;
	}
	else if(ePlayer == CPlayers::JOB_RANGER)
	{
		eCrest = CREST_RANGER;
		eTxt = NAME_RANGER;
	}

	// Main Crest
	if(iPos == 0)
	{
		Set_TextureInit(eCrest, WINCX / 18.6503f, WINCY / 20.4987f, WINCX / 46.1998f, WINCY / 36.5997f);
		Set_TextureInit(eTxt, WINCX / 7.80005f, WINCY / 25.3588f, WINCX / 21.9f, WINCY / 54.4994f);
	}
	else if(iPos == 1)
	{
		Set_TextureInit(eCrest, WINCX / 19.5103f, WINCY / 6.68838f, WINCX / 59.9996f, WINCY / 45.6996f);
		Set_TextureInit(eTxt, WINCX / 8.70007f, WINCY / 6.95877f, WINCX / 26.1f, WINCY / 69.3992f);
	}
	else if(iPos == 2)
	{
		Set_TextureInit(eCrest, WINCX / 19.5103f, WINCY / 4.63833f, WINCX / 59.9996f, WINCY / 45.6996f);
		Set_TextureInit(eTxt, WINCX / 8.70007f, WINCY / 4.79877f, WINCX / 26.1f, WINCY / 69.3992f);

	}
	else if(iPos == 3)
	{
		Set_TextureInit(eCrest, WINCX / 19.5103f, WINCY / 3.48832f, WINCX / 59.9996f, WINCY / 45.6996f);
		Set_TextureInit(eTxt, WINCX / 8.70007f, WINCY / 3.55877f, WINCX / 26.1f, WINCY / 69.3992f);
	}
}

void CParty::Set_TextureInit(eParty eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CParty::Set_TexView(void)
{
	for(int i = 0; i < PARTY_END; ++i)
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


void CParty::Set_Player(void)
{
	m_eMainPlayer = m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob();
	Set_Crest(m_eMainPlayer, 0);

	if(m_eOldMainPlayer == m_eMainPlayer)
		return;

	for(int i = 0 ; i < CPlayers::JOB_END - 1; ++i)
	{
		if(m_eMainPlayer == m_eOtherPlayer[i])
		{
			m_eOtherPlayer[i] = m_eOldMainPlayer;
			Set_Crest(m_eOldMainPlayer, i + 1);
			m_eOldMainPlayer = m_eMainPlayer;
			break;
		}
	}
}

void CParty::Set_Bar(void)
{
	// Main
	Engine::CStatus* pStatus = const_cast<Engine::CStatus*>(m_pMainPlayer->GetStatus());
	calculate_Bar(pStatus, BAR_MAINHP, _BAR_MAINHP, BAR_MAINMP, _BAR_MAINMP);

	// 1st
	if(m_eOtherPlayer[0] == CPlayers::JOB_END)
	{
		return;
	}
	pStatus = const_cast<Engine::CStatus*>(m_pMainPlayer->Get_PlayerSet(m_eOtherPlayer[0])->GetStatus());
	calculate_Bar(pStatus, BAR_1HP, _BAR_1HP, BAR_1MP, _BAR_1MP);

	// 2nd
	if(m_eOtherPlayer[1] == CPlayers::JOB_END)
		return;
	pStatus = const_cast<Engine::CStatus*>(m_pMainPlayer->Get_PlayerSet(m_eOtherPlayer[1])->GetStatus());
	calculate_Bar(pStatus, BAR_2HP, _BAR_2HP, BAR_2MP, _BAR_2MP);

	// 3rd
	if(m_eOtherPlayer[2] == CPlayers::JOB_END)
		return;
	pStatus = const_cast<Engine::CStatus*>(m_pMainPlayer->Get_PlayerSet(m_eOtherPlayer[2])->GetStatus());
	calculate_Bar(pStatus, BAR_3HP, _BAR_3HP, BAR_3MP, _BAR_3MP);
}

void CParty::calculate_Bar(Engine::CStatus* pStatus, eParty HpEnum, eBar HpBar, eParty MpEnum, eBar MpBar)
{
	float fHp = pStatus->m_tRealStat.m_fCurHp / pStatus->m_tRealStat.m_fMaxHp;
	float fMp = pStatus->m_tRealStat.m_fCurMana / pStatus->m_tRealStat.m_fMaxMana;

	m_fSizeX[HpEnum] = m_fOriBarSizeX[HpBar] * fHp;
	m_fX[HpEnum] = m_fOriBarfX[HpBar] - (m_fOriBarSizeX[HpBar] * (1.f - fHp));

	m_fSizeX[MpEnum] = m_fOriBarSizeX[MpBar] * fMp;
	m_fX[MpEnum] = m_fOriBarfX[MpBar] - (m_fOriBarSizeX[MpBar] * (1.f - fMp));
}

void CParty::CenterCrest(void)
{
	for(int i = 0; i < CPlayers::JOB_END; ++i)
		m_bCenterCrest[i] = false;

	if(m_eMainPlayer == CPlayers::JOB_GUARDIAN)
		m_bCenterCrest[CPlayers::JOB_GUARDIAN] = true;
	else if(m_eMainPlayer == CPlayers::JOB_MAGICIAN)
		m_bCenterCrest[CPlayers::JOB_MAGICIAN] = true;
	else if(m_eMainPlayer == CPlayers::JOB_PALADIN)
		m_bCenterCrest[CPlayers::JOB_PALADIN] = true;
	else if(m_eMainPlayer == CPlayers::JOB_RANGER)
		m_bCenterCrest[CPlayers::JOB_RANGER] = true;
}

bool CParty::Check_CenterCrest(eParty eJobName)
{
	if(eJobName == CENTER_GUARDIAN)
	{
		if(m_bCenterCrest[CPlayers::JOB_GUARDIAN])
			return true;
		else
			return false;
	}
	else if(eJobName == CENTER_MAGICIAN)
	{
		if(m_bCenterCrest[CPlayers::JOB_MAGICIAN])
			return true;
		else
			return false;
	}
	else if(eJobName == CENTER_RANGER)
	{
		if(m_bCenterCrest[CPlayers::JOB_RANGER])
			return true;
		else
			return false;
	}
	else if(eJobName == CENTER_PALADIN)
	{
		if(m_bCenterCrest[CPlayers::JOB_PALADIN])
			return true;
		else
			return false;
	}



	return true;
}

void CParty::Init_MoveTex(eParty eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CParty::Move_Tex(void)
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

	fTemp2 = 0.002f;
	if(Engine::GetDIKeyState(DIK_UP) & 0x80)
	{
		m_MoveY += fTemp2;
	}

	if(Engine::GetDIKeyState(DIK_DOWN) & 0x80)
	{
		m_MoveY -= fTemp2;
	}


	float fTemp = 0.01f;
	if(Engine::GetDIKeyState(DIK_Z) & 0x80)
	{
		m_MoveSizeX += fTemp;
	}

	if(Engine::GetDIKeyState(DIK_X) & 0x80)
	{
		m_MoveSizeX -= fTemp;
	}

	if(Engine::GetDIKeyState(DIK_B) & 0x80)
	{
		m_MoveSizeY += fTemp;
	}

	if(Engine::GetDIKeyState(DIK_N) & 0x80)
	{
		m_MoveSizeY -= fTemp;
	}

	cout << m_MoveX << "\t" << m_MoveY << "\t" << m_MoveSizeX << "\t" << m_MoveSizeY << endl;
}