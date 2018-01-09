#include "stdafx.h"
#include "MonsterBar.h"

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

CMonsterBar::CMonsterBar(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_bBlood(false)
, m_fBloodAlpha(0.f)
{

}

CMonsterBar::~CMonsterBar(void)
{

}

HRESULT CMonsterBar::Initialize(void)
{
	for(int i = 0 ; i < HP_END; ++i)
		m_pTexture[i] = NULL;

	m_eTexNum = HP_BLOOD;

	FAILED_CHECK(AddComponent());

	// º¸½º¸÷
	Set_TextureInit(HP_OUTLINE, WINCX / 2.f, WINCY / 11.8502f, WINCX / 4.f, WINCY / 29.8001f);
	Set_TextureInit(HP_BAR, WINCX / 2.f, WINCY / 12.3802f, WINCX / 4.81f, WINCY / 89.7998f);
	Set_TextureInit(HP_BLOOD, WINCX / 2.076f, WINCY / 16.3723f, WINCX / 3.64f, WINCY / 7.10002f);

	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int CMonsterBar::Update(const _float& fTimeDelta)
{
	//Move_Tex();
	//Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);

	m_fTimeDelta = fTimeDelta;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();
	MonsterHP();
	Check_Blood();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CMonsterBar::Render(void)
{
	CMonsters* pMonster = ((CPlayers*)((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer())->Get_TargetMonster();

	if(pMonster == NULL)
	{
		Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_TARGET);
		return;
	}

	pMonster->Set_DMG_Check(0.f);

	m_vTargetPos = pMonster->GetInfo()->m_vPos;
	m_vTargetPos.y += 0.2f;

	Engine::Call_EtcFX(Engine::ETC_TARGET, &m_vTargetPos, true);

	Engine::CStatus* pStatus = const_cast<Engine::CStatus*>(pMonster->GetStatus());

	if(pStatus->m_tRealStat.m_fCurHp <= 0.f)
		return;


	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < HP_END; ++i)
	{
		if(m_pTexture[i] == NULL)
			continue;

		pEffect->SetTechnique("Default_Device");
		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
		pEffect->SetMatrix("g_matView",&m_matView[i]);
		pEffect->SetMatrix("g_matProj",&m_matProj);

		if(i != HP_BLOOD)
			pEffect->SetFloat("g_fAlpha", 1.f);
		else
			pEffect->SetFloat("g_fAlpha", m_fBloodAlpha);

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

void CMonsterBar::Free(void)
{
	CUI::Free();
}

HRESULT CMonsterBar::AddComponent(void)
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
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_HP_Outline");
	m_pTexture[HP_OUTLINE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[HP_OUTLINE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_HP_Outline", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_HP_Bar");
	m_pTexture[HP_BAR] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[HP_BAR], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_HP_Bar", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_HP_Blood");
	m_pTexture[HP_BLOOD] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[HP_BLOOD], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_HP_Blood", pComponent);

	return S_OK;
}

CMonsterBar* CMonsterBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterBar*		pGameObject = new CMonsterBar(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CMonsterBar::Set_TextureInit(eMonsterHP eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void CMonsterBar::Set_TexView(void)
{
	for(int i = 0; i < HP_END; ++i)
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

void CMonsterBar::MonsterHP(void)
{
	CMonsters* pMonster = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer()->Get_TargetMonster();

	if(pMonster == NULL)
		return;

	Engine::CStatus* pStatus = const_cast<Engine::CStatus*>(pMonster->GetStatus());

	if(pStatus->m_tRealStat.m_fCurHp <= 0.f)
		return;

	bool bBoss = pMonster->Get_bBoss_Type();


	if(bBoss)
	{
		m_fOriHpSizeX = WINCX / 4.81f;
		m_fOriHpfX = WINCX / 2.f;

		Set_TextureInit(HP_OUTLINE, WINCX / 2.f, WINCY / 11.8502f, WINCX / 4.f, WINCY / 29.8001f);
		Set_TextureInit(HP_BAR, m_fOriHpfX, WINCY / 12.3802f, m_fOriHpSizeX, WINCY / 89.7998f);
		Set_TextureInit(HP_BLOOD, WINCX / 2.076f, WINCY / 16.3723f, WINCX / 3.64f, WINCY / 7.10002f);
	}
	else
	{
		m_fOriHpSizeX = WINCX / 6.91f;
		m_fOriHpfX = WINCX / 2.f;

		Set_TextureInit(HP_OUTLINE, WINCX / 2.f, WINCY / 12.1702f, WINCX / 5.7f, WINCY / 38.9000f);
		Set_TextureInit(HP_BAR, m_fOriHpfX, WINCY / 12.6902f, m_fOriHpSizeX, WINCY / 127.799f);
		Set_TextureInit(HP_BLOOD, WINCX / 2.056f, WINCY / 15.6123f, WINCX / 5.24f, WINCY / 8.70002f);
	}



	float fHp = pStatus->m_tRealStat.m_fCurHp / pStatus->m_tRealStat.m_fMaxHp;

	// Hp 
	m_fSizeX[HP_BAR] = m_fOriHpSizeX * fHp;
	m_fX[HP_BAR] = m_fOriHpfX - (m_fOriHpSizeX * (1.f - fHp));
}

void CMonsterBar::Check_Blood(void)
{
	CMonsters* pMonster = ((CPlayers*)((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer())->Get_TargetMonster();

	if(pMonster == NULL)
	{
		m_bBlood = false;
		m_fBloodAlpha = 0.f;
		return;
	}

	Engine::CStatus* pStatus = const_cast<Engine::CStatus*>(pMonster->GetStatus());

	if(pStatus->m_tRealStat.m_fCurHp <= 0.f)
	{
		m_bBlood = false;
		m_fBloodAlpha = 0.f;
		return;
	}

	if(0.f < pMonster->Get_DMG_Check() && m_bBlood == false)
	{
		m_bBlood = true;
		m_fBloodAlpha = 1.f;
	}

	if(m_bBlood == true)
	{
		m_fBloodAlpha -= m_fTimeDelta;

		if(m_fBloodAlpha <= 0.f)
			m_bBlood = false;
	}
}

void CMonsterBar::Init_MoveTex(eMonsterHP eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void CMonsterBar::Move_Tex(void)
{
	float fTemp2 = 0.01f;

	if(Engine::GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		m_MoveX -= fTemp2;
	}

	if(Engine::GetDIKeyState(DIK_LEFT) & 0x80)
	{
		m_MoveX += fTemp2;
	}

	fTemp2 = 0.01f;
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

}
