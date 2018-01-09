#include "stdafx.h"
#include "Players.h"
#include "Export_Function.h"
#include "Transform.h"
#include "DynamicMesh.h"
#include "KeyMgr.h"
#include "Stage.h"
#include "DynamicCamera.h"
#include "DynamicMesh.h"
#include "WyvernBrown.h"
#include "Calculator.h"
#include "Paladin.h"
#include "Ranger.h"
#include "Monsters.h"
#include "Effect_Manager.h"
#include "DmgView.h"
#include "SoundMgr.h"
#include "MainPlayer.h"
#include "Magician.h"
#include "CastingBar.h"
#include "MonsterDmg.h"

CPlayers::CPlayers(LPDIRECT3DDEVICE9 pDevice)
: CUnits(pDevice)
, m_eTrackID(STAND_NORMAL)
, m_eKey_StateID(STAKEY_DEFAULT)
, m_eAI_StateID(AI_DEFAULT)
, m_ePrev_TrackID(TRACK_DEFAULT)
, m_fSpeed(130.f)
, m_fTimeDelta(0.f)
, m_fAcc(0.f)
, m_fSlowTime(1.f)
, m_pWyvern(NULL)
, m_fRange(0.f)
, m_bRun_Jump(false)
, m_iAtkCnt(0)
, m_bSkill_Input(false)
, m_bCombo_Input(false)
, m_fPeriod(0.3f)
, m_pDynamicCamera(NULL)
, m_eTypeJob(JOB_END)
, m_pmatSaddle(NULL)
, m_fSpecRange(1.f)
, m_fSpecPower(1.f)
, m_dwIndex(0)
, m_vFormation(_vec3(0,0,0))
, m_fFomationDis(0.f)
, m_fTargetDis(0.f)
, m_pTargetMonster(NULL)
, m_fCurrentDis(0.f)
, m_fRadian(0.f)
, m_pMainPlayer(NULL)
, m_bMainPlayer(false)
, m_bHold(false)
, m_vTargetDir(_vec3(0,0,0))
, m_fTargetAngle(0.f)
, m_eSkill_StateID(SKILL_DEFAULT)
, m_ePrev_SKstateID(SKILL_DEFAULT)
, m_bTrack(false)
, m_fHeight(0.f)
, m_eOriTrack(TRACK_DEFAULT)
, m_bCancel(false)
, m_dwTestIndex(0)
, m_bConstance(false)
, m_fSoundTargetDistance(0.f)
, m_fSoundVolume(0.f)
, m_pSoundTargetPlayer(NULL)
, m_vQuestEnd_Start_Pos(_vec3(0,0,0))	 
, m_vQuestEnd_End_Pos(_vec3(0,0,0))	
, m_bQuestEnd_First(false)
, m_bQuestEnd_Second(false)
, m_bQuestEnd_End(false)
, m_bEventEnd(false)
, m_fFirstDis(0.f)
, m_vEvasionPos(_vec3(0,0,0))
, m_fEvasion_Length(0.f)
, m_bEvasion_Check(false)
, m_bBossCam(false)
{
	m_bActStateSet.reset();
	m_bAtkComboSet.reset();
	m_bKeyStateset.reset();
	m_bSkillSet.reset();
	m_bAIset.reset();
	m_bEventRootSet.reset();

	for(int i = 0; i < 7; ++i)
	{
		m_fSkillCooltime[i] = 0.f;
		m_fSkillCooltime_Max[i] = 0.f;
	}

	for(int i = 0; i < SKILL_END; ++i)
	{
		m_fSkillAngle_Effect[i] = 0.f;
		m_vEffectPos[i] = _vec3(0,0,0);
	}
	for(int i = 0; i < 3; ++i)
		m_fEventTime[i] = 0.f;
}

CPlayers::~CPlayers(void)
{

}
_int CPlayers::Update(const _float& fTimeDelta)
{
	return 0;
}

bool CPlayers::KeyInput_Move(const _float& fTimeDelta, CPlayers* pPlayer)
{
	float		fTime = fTimeDelta;
	bool		bKeyMove = false;

	CDynamicCamera* pCamera = ((CDynamicCamera*)Engine::Get_ObjList(LAYER_UI,L"DynamicCamera")->front());
	if(pCamera == NULL)
		return false;

	if(false == pCamera->Get_MouseFix())
	{	
		if(Engine::GetDIKeyState(DIK_W) & 0x80)
		{
			//m_pInfo->m_vPos += m_pInfo->m_vDir * m_fSpeed * fTime;
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(m_pInfo->m_vDir * m_fSpeed * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(m_pInfo->m_vDir * m_fSpeed * fTime), pPlayer);
			m_eTrackID = RUN_F;
			bKeyMove = true;
		}
		if(Engine::GetDIKeyState(DIK_S) & 0x80)
		{
			//m_pInfo->m_vPos -= m_pInfo->m_vDir * m_fSpeed * fTime;
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(-m_pInfo->m_vDir * m_fSpeed * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(-m_pInfo->m_vDir * m_fSpeed * fTime), pPlayer);
			m_eTrackID = RUN_B;
			bKeyMove = true;
		}
		if(Engine::GetDIKeyState(DIK_A) & 0x80)
		{
			m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f * fTime);

			bKeyMove = true;
		}

		if(Engine::GetDIKeyState(DIK_D) & 0x80)
		{
			m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f * fTime);
			bKeyMove = true;		
			
		}
	}
	else
	{
		if(Engine::GetDIKeyState(DIK_W) & 0x80)
		{
			//m_pInfo->m_vPos += m_pInfo->m_vDir * m_fSpeed * fTimeDelta;
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(m_pInfo->m_vDir * m_fSpeed * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(m_pInfo->m_vDir * m_fSpeed * fTime), pPlayer);
			m_eTrackID = RUN_F;
			bKeyMove = true;
		}
		if(Engine::GetDIKeyState(DIK_S) & 0x80)
		{
			//m_pInfo->m_vPos -= m_pInfo->m_vDir * (m_fSpeed - 25.f ) * fTimeDelta ;
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(-m_pInfo->m_vDir * (m_fSpeed - 25.f ) * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(-m_pInfo->m_vDir * (m_fSpeed - 25.f ) * fTime), pPlayer);
			m_eTrackID = RUN_B;
			bKeyMove = true;
		}
		if(Engine::GetDIKeyState(DIK_A) & 0x80)
		{
			D3DXVECTOR3 vtemp;
			memcpy(&vtemp, &m_pInfo->m_matWorld._31, sizeof(D3DXVECTOR3));

			//m_pInfo->m_vPos += vtemp * m_fSpeed * fTimeDelta;
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vtemp * m_fSpeed * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(vtemp * m_fSpeed * fTime), pPlayer);
			m_eTrackID = RUN_L;
			bKeyMove = true;
		}
		if(Engine::GetDIKeyState(DIK_D) & 0x80)
		{
			D3DXVECTOR3 vtemp;
			memcpy(&vtemp, &m_pInfo->m_matWorld._31, sizeof(D3DXVECTOR3));

			//m_pInfo->m_vPos -= (vtemp * m_fSpeed * fTimeDelta);
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(-vtemp * m_fSpeed * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(-vtemp * m_fSpeed * fTime), pPlayer);

			m_eTrackID = RUN_R;
			bKeyMove = true;
		}

		if(Engine::GetDIKeyState(DIK_W) & 0x80 && 
			Engine::GetDIKeyState(DIK_D) & 0x80)
		{
			D3DXVECTOR3 vtemp;
			memcpy(&vtemp, &m_pInfo->m_matWorld._31, sizeof(D3DXVECTOR3));

			D3DXVECTOR3 vDir = m_pInfo->m_vDir - vtemp;
			D3DXVec3Normalize(&vDir,&vDir);
			
			//m_pInfo->m_vPos -= vDir * 0.7f * fTimeDelta;	
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(-vDir * 0.7f * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(-vDir * 0.7f * fTime), pPlayer);

			m_eTrackID = RUN_FR;
			bKeyMove = true;
		}
		if(Engine::GetDIKeyState(DIK_W) & 0x80 && 
			Engine::GetDIKeyState(DIK_A) & 0x80)
		{
			D3DXVECTOR3 vtemp;
			memcpy(&vtemp, &m_pInfo->m_matWorld._31, sizeof(D3DXVECTOR3));

			D3DXVECTOR3 vDir = m_pInfo->m_vDir + vtemp;
			D3DXVec3Normalize(&vDir,&vDir);
		
			//m_pInfo->m_vPos -= vDir * 0.7f * fTimeDelta;
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(-vDir * 0.7f * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(-vDir * 0.7f * fTime), pPlayer);

			m_eTrackID = RUN_FL;
			bKeyMove = true;
		}
		if(Engine::GetDIKeyState(DIK_S) & 0x80 &&
			Engine::GetDIKeyState(DIK_A) & 0x80)
		{
			D3DXVECTOR3 vtemp;
			memcpy(&vtemp, &m_pInfo->m_matWorld._31, sizeof(D3DXVECTOR3));

			D3DXVECTOR3 vDir = m_pInfo->m_vDir - vtemp;
			D3DXVec3Normalize(&vDir,&vDir);

			//m_pInfo->m_vPos -= -vDir * fTimeDelta * 0.8f;	
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vDir * 0.8f * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(vDir * 0.8f * fTime), pPlayer);

			m_eTrackID = RUN_BL;
			bKeyMove = true;

		}
		if(Engine::GetDIKeyState(DIK_S) & 0x80 && 
			Engine::GetDIKeyState(DIK_D) & 0x80)
		{
			D3DXVECTOR3 vtemp;
			memcpy(&vtemp, &m_pInfo->m_matWorld._31, sizeof(D3DXVECTOR3));

			D3DXVECTOR3 vDir = m_pInfo->m_vDir + vtemp;
			D3DXVec3Normalize(&vDir,&vDir);	
			
			//m_pInfo->m_vPos -= -vDir  * fTimeDelta * 0.8f;
			//pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vDir * 0.8f * fTime),pPlayer->m_dwIndex, m_fHeight);
			MoveOnNaviMesh(&(vDir * 0.8f * fTime), pPlayer);
			
			m_eTrackID = RUN_BR;
			bKeyMove = true;
		}

	}

	if(true == bKeyMove)
	{
		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();

		pPlayer->m_bKeyStateset.set(STAKEY_MOVE);
		return true;
	}
	else
	{
		pPlayer->m_bKeyStateset.reset(STAKEY_MOVE);
		m_pDynamicCamera->Set_Interpol(false);
	}
	return false;
}

void CPlayers::KeyInput_Combo(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(pPlayer->m_bActStateSet.any())
		return;

	bool	bKeyCombo = false;

	
	if(RangerAttack_Link(pPlayer))
		bKeyCombo = true;
	if(MagicianAttack_Link(pPlayer))
		bKeyCombo = true;
	if(PaladinAttack_Link(pPlayer))
		bKeyCombo = true;
	if(GuardianAttack_Link(pPlayer))
		bKeyCombo = true;
	
	
	if(true == bKeyCombo)
	{
		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();

		if(pPlayer->m_bKeyStateset.test(STAKEY_MOVE))
			pPlayer->m_bKeyStateset.reset(STAKEY_MOVE);

		pPlayer->m_bKeyStateset.set(STAKEY_COMBO);
	}
	else
	{
		if(pPlayer->m_bAtkComboSet.count() == 0)
			pPlayer->m_bKeyStateset.reset(STAKEY_COMBO);
	}
}

void CPlayers::KeyInput_Skill(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(	pPlayer->m_bActStateSet.any())
		return;

	bool bKeySkill = false;

	if(CKeyMgr::GetInstance()->GetKeyDown('1'))
	{
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_1))
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}
		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_1))
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}
		if(pPlayer->Get_PlayerJob() == JOB_RANGER)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_1) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(4.6f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_1) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}

		AngleSetting();

		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();
			
		m_eTrackID = SK_0;
		pPlayer->Set_SkillBitset(SKILL_1);
		bKeySkill = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyDown('2'))
	{
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_2))
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(1.05488f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_2))
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}
		if(pPlayer->Get_PlayerJob() == JOB_RANGER)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_2) /*|| pPlayer->m_pTargetMonster == NULL*/)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(1.f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_2) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}

		AngleSetting();

		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();

		m_eTrackID = SK_1;
		pPlayer->Set_SkillBitset(SKILL_2);
		bKeySkill = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyDown('3'))
	{
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_3) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}
		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_3))
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(0.45f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_RANGER)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_3) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(1.f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_3) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}

		AngleSetting();

		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();

		m_eTrackID = SK_2;
		pPlayer->Set_SkillBitset(SKILL_3);
		bKeySkill = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyDown('4'))
	{
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_4) || pPlayer->m_pTargetMonster == NULL || pPlayer->m_fCurrentDis > 0.5f)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}
		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_4))
				return;

			Engine::Call_MonsterFX(Engine::SKULL_SHIELD, &m_pInfo->m_vPos, false);

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(0.6f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_RANGER)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_4) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(0.4f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_4) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(3.8f);
		}

		AngleSetting();

		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();

		m_eTrackID = SK_3;
		pPlayer->Set_SkillBitset(SKILL_4);
		bKeySkill = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyDown('5'))
	{
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_5) || pPlayer->m_pTargetMonster == NULL || pPlayer->m_fCurrentDis > 0.5f)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_CastTime(1.f);
		}
		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_5) || pPlayer->m_pTargetMonster == NULL || pPlayer->m_fCurrentDis > 0.5f)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}
		if(pPlayer->Get_PlayerJob() == JOB_RANGER)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_5) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}
		if(pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_5) || pPlayer->m_pTargetMonster == NULL)
				return;

			((CCastingBar*)(Engine::Get_ObjList(LAYER_UI, L"CastingBar")->front()))->Set_Render(false);
		}



		AngleSetting();		

		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();

		m_eTrackID = SK_4;
		pPlayer->Set_SkillBitset(SKILL_5);
		bKeySkill = true;
	}

	if(true == bKeySkill)
	{
		if(pPlayer->m_bAtkComboSet.any())
			pPlayer->m_bAtkComboSet.reset();

		pPlayer->m_bKeyStateset.set(STAKEY_SKILL);
	}
	else
		pPlayer->m_bKeyStateset.reset(STAKEY_SKILL);
}

void CPlayers::KeyInput_Static(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(pPlayer->m_bAtkComboSet.any())
		return;

	bool bKeyStatic = false;

	if(CKeyMgr::GetInstance()->GetKeyDown(VK_LSHIFT))
	{
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_INNATE))
				return;

			Engine::Call_GuardianFX(Engine::G_INNATE, &pPlayer->m_vEffectPos[SKILL_INNATE], true,NULL,&pPlayer->m_fSkillAngle_Effect[SKILL_INNATE]);
		}
		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_INNATE))
				return;

			Engine::Call_PaladinFX(Engine::P_INNATE, &pPlayer->m_vEffectPos[SKILL_INNATE], true,NULL,&pPlayer->m_fSkillAngle_Effect[SKILL_INNATE]);
		}
		if(pPlayer->Get_PlayerJob() == JOB_RANGER)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_INNATE))
				return;
		}
		if(pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_INNATE))
				return;
		}

		if(pPlayer->m_bActStateSet.any())
			return;

		AngleSetting();

		m_eTrackID = HIDDEN_SKILL;
		pPlayer->Set_ActBitset(ACT_EVASION);
		m_bActStateSet.set(ACT_EVASION);

		bKeyStatic = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyUp(VK_LSHIFT))
	{
		m_eTrackID = STAND_NORMAL;
		pPlayer->Reset_ActBitset(ACT_EVASION);
		m_bActStateSet.reset(ACT_EVASION);
		pPlayer->m_fSlowTime = 1.f;

		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			Engine::CEffect_Manager::GetInstance()->Off_PaladinFX(Engine::P_INNATE);
		}
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			Engine::CEffect_Manager::GetInstance()->Off_GuardianFX(Engine::G_INNATE);
		}

		bKeyStatic = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyDown('E'))
	{
		if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_DASH))
				return;
		}
		if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_DASH))
				return;
		}
		if(pPlayer->Get_PlayerJob() == JOB_RANGER)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_DASH))
				return;
		}
		if(pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
		{
			if(pPlayer->m_bCoolTimeSet.test(SKILL_DASH))
				return;
		}

		AngleSetting();
		
		Engine::Set_RadialRender(true, 0.5f);
		m_eTrackID = DASH;
		pPlayer->Set_ActBitset(ACT_DASH);
		m_bActStateSet.set(ACT_DASH);

		bKeyStatic = true;
	}

	if(bKeyStatic)
	{
		if(pPlayer->m_bSkillSet.any())
			pPlayer->m_bSkillSet.reset();

		pPlayer->m_bKeyStateset.set(STAKEY_STATIC);
	}
	else
		pPlayer->m_bKeyStateset.reset(STAKEY_STATIC);

}

void CPlayers::KeyInput_UI_Set(const _float& fTimeDelta)
{

}

void CPlayers::KeyCheck_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(pPlayer->m_bActStateSet.test(ACT_MOUNT))
	{
		if(false == KeyInput_Move_To_Wyvern(fTimeDelta,pPlayer) &&
			false == KeyInput_Jump_To_Wyvern(fTimeDelta,pPlayer))
		{
			if(true == pPlayer->Get_Wyvern()->Get_Fly())
			{
				if(pPlayer->m_pWyvern->Get_TrackID() != CWyverns::WY_RUN_JUMP_LOOP)
				{
					m_eTrackID = MOUNT_FLYING_IDLE;
					pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_FLYING_IDLE);
				}
			}
			else
			{
				if(false == pPlayer->m_pWyvern->Get_Jump())
				{
					m_eTrackID = MOUNT_IDLE;
					pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_IDLE);
				}

			}

		}
	}

	if(pPlayer->Get_Wyvern()->Get_TrackID() == CWyverns::WY_RUN_JUMP_LOOP)
		Engine::Set_RadialRender(true);
	else
		Engine::Set_RadialRender(false);
}

bool CPlayers::RangerAttack_Link(CPlayers* pPlayer)
{
	if(pPlayer->Get_TargetMonster() == NULL)
		return false;

	if(pPlayer->m_eTypeJob == JOB_RANGER)
	{
		if(CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
		{
			if(pPlayer->m_bAtkComboSet.any())
				pPlayer->m_bAtkComboSet.reset();

			m_eTrackID = ATK_0;
			pPlayer->m_bAtkComboSet.set(m_eTrackID);

			AngleSetting();
			return true;
		}
		if(CKeyMgr::GetInstance()->GetKeyDown(VK_RBUTTON))
		{
			if(pPlayer->m_bAtkComboSet.any())
				pPlayer->m_bAtkComboSet.reset();

			m_eTrackID = ATK_1;
			pPlayer->m_bAtkComboSet.set(m_eTrackID);

			AngleSetting();
			return true;
		}
		if(CKeyMgr::GetInstance()->GetKeyDown('F'))
		{
			if(pPlayer->m_bAtkComboSet.any())
				pPlayer->m_bAtkComboSet.reset();

			m_eTrackID = ATK_2;
			pPlayer->m_bAtkComboSet.set(m_eTrackID);

			AngleSetting();
			return true;
		}

		
	}

	return false;
}

bool CPlayers::MagicianAttack_Link(CPlayers* pPlayer)
{
	if(pPlayer->Get_TargetMonster() == NULL)
		return false;

	if(pPlayer->m_eTypeJob == JOB_MAGICIAN)
	{
		if(CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
		{
			if(m_iAtkCnt >= ATTACK_END)
				m_iAtkCnt = 0;

			float fTrackTime = (float)((Engine::CDynamicMesh*)pPlayer->GetComponent(L"Mesh"))->GetTrackPosition();	

			if(pPlayer->m_bAtkComboSet.any())
			{
				if(0 < m_iAtkCnt && 0.6f < fTrackTime)
				{
					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);
						
					if(0 < m_iAtkCnt && pPlayer->m_bAtkComboSet.test((ATK_COMBO)(m_iAtkCnt - 1)))
						pPlayer->m_bAtkComboSet.reset((ATK_COMBO)(m_iAtkCnt - 1));

					++m_iAtkCnt;
				}
				if (0 == m_iAtkCnt && 0.95f < fTrackTime)
				{
					AngleSetting();

					if(1.f != pPlayer->m_fSlowTime)
						pPlayer->m_fSlowTime = 1.f;

					if(pPlayer->m_bAtkComboSet.any())
						pPlayer->m_bAtkComboSet.reset();

					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					if(0 < m_iAtkCnt && pPlayer->m_bAtkComboSet.test((ATK_COMBO)(m_iAtkCnt - 1)))
						pPlayer->m_bAtkComboSet.reset((ATK_COMBO)(m_iAtkCnt - 1));

					++m_iAtkCnt;
				}
			}
			else
			{
				if(m_iAtkCnt != 0)
					m_iAtkCnt = 0;

				if(0 == m_iAtkCnt && !pPlayer->m_bAtkComboSet.any())
				{
					AngleSetting();

					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					++m_iAtkCnt;
				}
			}

			return true;
		}
	}

	return false;
}

bool CPlayers::GuardianAttack_Link(CPlayers* pPlayer)
{
	if(pPlayer->m_eTypeJob == JOB_GUARDIAN)
	{
		if(CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
		{
			if(m_iAtkCnt >= ATTACK_END)
				m_iAtkCnt = 0;

			float fTrackTime = (float)((Engine::CDynamicMesh*)pPlayer->GetComponent(L"Mesh"))->GetTrackPosition();	

			if(pPlayer->m_bAtkComboSet.any())
			{
				if(0 < m_iAtkCnt && 0.5f < fTrackTime)
				{
					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					if(0 < m_iAtkCnt && pPlayer->m_bAtkComboSet.test((ATK_COMBO)(m_iAtkCnt - 1)))
						pPlayer->m_bAtkComboSet.reset((ATK_COMBO)(m_iAtkCnt - 1));

					++m_iAtkCnt;
				}
				if (0 == m_iAtkCnt && 0.85f < fTrackTime)
				{
					AngleSetting();

					if(1.f != pPlayer->m_fSlowTime)
						pPlayer->m_fSlowTime = 1.f;

					if(pPlayer->m_bAtkComboSet.any())
						pPlayer->m_bAtkComboSet.reset();

					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					if(0 < m_iAtkCnt && pPlayer->m_bAtkComboSet.test((ATK_COMBO)(m_iAtkCnt - 1)))
						pPlayer->m_bAtkComboSet.reset((ATK_COMBO)(m_iAtkCnt - 1));

					++m_iAtkCnt;
				}
			}
			else
			{
				if(m_iAtkCnt != 0)
					m_iAtkCnt = 0;

				if(0 == m_iAtkCnt && !pPlayer->m_bAtkComboSet.any())
				{
					AngleSetting();

					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					++m_iAtkCnt;
				}
			}

			return true;
		}
	}

	return false;
}

bool CPlayers::PaladinAttack_Link(CPlayers* pPlayer)
{
	if(pPlayer->m_eTypeJob == JOB_PALADIN)
	{
		if(CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
		{
			if(m_iAtkCnt >= ATTACK_END)
				m_iAtkCnt = 0;

			float fTrackTime = (float)((Engine::CDynamicMesh*)pPlayer->GetComponent(L"Mesh"))->GetTrackPosition();	

			if(pPlayer->m_bAtkComboSet.any())
			{
				if(0 < m_iAtkCnt && 0.5f < fTrackTime)
				{
					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					if(0 < m_iAtkCnt && pPlayer->m_bAtkComboSet.test((ATK_COMBO)(m_iAtkCnt - 1)))
						pPlayer->m_bAtkComboSet.reset((ATK_COMBO)(m_iAtkCnt - 1));

					++m_iAtkCnt;
				}
				if (0 == m_iAtkCnt && 0.85f < fTrackTime)
				{
					AngleSetting();

					if(1.f != pPlayer->m_fSlowTime)
						pPlayer->m_fSlowTime = 1.f;

					if(pPlayer->m_bAtkComboSet.any())
						pPlayer->m_bAtkComboSet.reset();

					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					if(0 < m_iAtkCnt && pPlayer->m_bAtkComboSet.test((ATK_COMBO)(m_iAtkCnt - 1)))
						pPlayer->m_bAtkComboSet.reset((ATK_COMBO)(m_iAtkCnt - 1));

					++m_iAtkCnt;
				}
			}
			else
			{
				if(m_iAtkCnt != 0)
					m_iAtkCnt = 0;

				if(0 == m_iAtkCnt && !pPlayer->m_bAtkComboSet.any())
				{
					AngleSetting();

					m_eTrackID = (TRACKID)m_iAtkCnt;
					pPlayer->m_bAtkComboSet.set((ATK_COMBO)m_iAtkCnt);

					++m_iAtkCnt;
				}
			}

			return true;
		}
	}

	return false;
}

void CPlayers::KeyInput_Static_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(CKeyMgr::GetInstance()->GetKeyDown('R') )
	{
		if(pPlayer->m_bActStateSet.test(ACT_MOUNT))
		{
			Engine::Call_EtcFX(Engine::ETC_SMOKE, &const_cast<Engine::CTransform*>(pPlayer->Get_Wyvern()->GetInfo())->m_vPos, false);
			pPlayer->Set_TrackID(STAND_NORMAL);
			pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_IDLE);
			pPlayer->m_pWyvern->Set_ActMount(false);
			pPlayer->m_pWyvern->Set_Fly(false);
			pPlayer->m_pWyvern->Set_Jump(false);
			pPlayer->m_pWyvern->Set_JumpAcc_Init(0.f);
			pPlayer->Reset_ActBitset(ACT_MOUNT);
			m_bActStateSet.reset(ACT_MOUNT);

			m_pDynamicCamera->RenewalCamera();
		}
		else
		{
			if(pPlayer->m_bActStateSet.any() ||
				pPlayer->m_bAtkComboSet.any())
				return;

			Engine::Call_EtcFX(Engine::ETC_SMOKE, &const_cast<Engine::CTransform*>(pPlayer->GetInfo())->m_vPos, false);

			pPlayer->Set_TrackID(MOUNT_IDLE);
			pPlayer->m_pWyvern->Set_WyvernInfo(m_pInfo->m_vPos, m_pInfo->m_fAngle[Engine::ANGLE_Y]);
			pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_IDLE);
			pPlayer->m_pWyvern->Set_ActMount(true);
			pPlayer->m_pWyvern->Set_TargetPlayer(pPlayer);
			pPlayer->Set_ActBitset(ACT_MOUNT);
			m_bActStateSet.set(ACT_MOUNT);

			m_pDynamicCamera->RenewalCamera();
		}

	}
}

bool CPlayers::KeyInput_Move_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(pPlayer->Get_Wyvern()->Get_TrackID() == CWyverns::WY_RUN_JUMP_UP)
		return false;

	bool bMove_Wyvern = false;

	CDynamicCamera* pCamera = ((CDynamicCamera*)Engine::Get_ObjList(LAYER_UI,L"DynamicCamera")->front());
	if(pCamera == NULL)
		return false;

	if(false == pPlayer->m_pWyvern->Get_Jump())
	{
		if(Engine::GetDIKeyState(DIK_W))
		{
			if(false == m_bRun_Jump)
				m_bRun_Jump = true;

			if(false == bMove_Wyvern)
			{
				m_eTrackID = MOUNT_RUN_F;
				float fSpeed = pPlayer->m_pWyvern->Get_Speed();
				_vec3 WyvernDir = ((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vDir; 
				((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vPos += WyvernDir * fSpeed * fTimeDelta;
				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_RUN_F);
				bMove_Wyvern = true;
			}
		}
		else
			m_bRun_Jump = false;

		if(Engine::GetDIKeyState(DIK_S))
		{
			if(false == m_bRun_Jump)
				m_bRun_Jump = true;

			if(false == bMove_Wyvern)
			{
				m_eTrackID = MOUNT_RUN_B;	
				float fSpeed = pPlayer->m_pWyvern->Get_Speed();
				_vec3 WyvernDir = ((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vDir; 
				((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vPos -= WyvernDir * (fSpeed / 1.5f) * fTimeDelta;
				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_RUN_B);
				bMove_Wyvern = true;
			}

		}
		else
			m_bRun_Jump = false;

		if(Engine::GetDIKeyState(DIK_A) & 0x80)
		{
			if(false == bMove_Wyvern)
			{
				m_eTrackID = MOUNT_ROT_L;
				((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f * fTimeDelta);
				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_IDLE_ROT_L_LOOP);
				bMove_Wyvern = true;
			}
		}

		if(Engine::GetDIKeyState(DIK_D) & 0x80)
		{
			if(false == bMove_Wyvern)
			{
				m_eTrackID = MOUNT_ROT_R;	
				((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f * fTimeDelta);
				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_IDLE_ROT_R_LOOP);	
				bMove_Wyvern = true;
			}
		}
	}
	else
	{
		if(Engine::GetDIKeyState(DIK_W) & 0x80)
		{
			if(pPlayer->m_pWyvern->Get_TrackID() != CWyverns::WY_RUN_JUMP_LOOP)
			{
				m_eTrackID = MOUNT_FLYING_F_FLUTTER;
				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_FLYING_F_FLUTTER);
				bMove_Wyvern = true;
			}

		}
		if(Engine::GetDIKeyState(DIK_S) & 0x80)
		{
			if(pPlayer->m_pWyvern->Get_TrackID() != CWyverns::WY_RUN_JUMP_LOOP)
			{
				if(false == pPlayer->m_pWyvern->Get_Decelerate() && false == pPlayer->m_pWyvern->Get_Axel())
				{
					m_eTrackID = MOUNT_FLYING_B;
					pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_FLYING_B);
					bMove_Wyvern = true;
				}

			}
		}
		if(Engine::GetDIKeyState(DIK_A) & 0x80)
		{
			m_eTrackID = MOUNT_ROT_L;
			((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f * fTimeDelta);
			bMove_Wyvern = true;
		}
		if(Engine::GetDIKeyState(DIK_D) & 0x80)
		{
			m_eTrackID = MOUNT_ROT_R;	
			((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f * fTimeDelta);
			bMove_Wyvern = true;
		}

		if(Engine::GetDIKeyState(DIK_W) & 0x80 &&
			Engine::GetDIKeyState(DIK_LSHIFT) & 0x80)
		{
			((CDynamicCamera*)Engine::Get_ObjList(LAYER_UI,L"DynamicCamera")->front())->Set_MouseFix(true);

			pPlayer->m_pWyvern->Set_Axel(true);

			if(pPlayer->m_pWyvern->Get_fAcc() >= 20.f)
			{
				m_eTrackID = MOUNT_FLYING_F_GLIDE;
			}
			else if (pPlayer->m_pWyvern->Get_TrackID() != CWyverns::WY_RUN_JUMP_LOOP)
			{

				m_eTrackID = MOUNT_FLYING_BOOST_FLUTTER;
			}

			bMove_Wyvern = true;
		}
		else
		{
			if(true == pPlayer->m_pWyvern->Get_Axel())
			{
				if(pPlayer->m_pWyvern->Get_TrackID() != CWyverns::WY_RUN_JUMP_LOOP)
					m_eTrackID = MOUNT_FLYING_F_FLUTTER;
				else
					pPlayer->m_pWyvern->Set_Decelerate(true);	
			}
			pPlayer->m_pWyvern->Set_Axel(false);
		}
		if(Engine::GetDIKeyState(DIK_W) & 0x80 &&
			Engine::GetDIKeyState(DIK_E) & 0x80)
		{
			_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");

			if(fHeight + 2.f <= ((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vPos.y)
			{
				((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vPos.y -= 2.f * fTimeDelta;
				pPlayer->m_pWyvern->Set_Down(true);
			}
			else
			{
				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_JUMP_LAND);
			}
		}
		if(Engine::GetDIKeyState(DIK_W) & 0x80 &&
			Engine::GetDIKeyState(DIK_SPACE) & 0x80)
		{
			((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vPos.y += 2.f * fTimeDelta;
		}
	}


	if(true == bMove_Wyvern)
		return true;

	return false;
}

bool CPlayers::KeyInput_Jump_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(pPlayer->m_bAtkComboSet.any())
		return false;

	bool bJump_Wyvern = false;

	if(CKeyMgr::GetInstance()->GetKeyDown(VK_SPACE))
	{
		if(false == pPlayer->m_pWyvern->Get_Fly())
		{
			pPlayer->m_pWyvern->Set_Jump(true);
			pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_RUN_JUMP_UP);
			bJump_Wyvern = true;
		}

	}
	if(CKeyMgr::GetInstance()->GetStateKey('E'))
	{
		if(true == pPlayer->m_pWyvern->Get_Fly())
		{
			float fSpeed = pPlayer->m_pWyvern->Get_Speed();
			float fJumpAcc = pPlayer->m_pWyvern->Get_JumpAcc();
			_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");

			if(fHeight + 2.f <= ((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vPos.y +
				((0.98f ) * fJumpAcc + fJumpAcc * 0.2f - 0.2f) * fTimeDelta)
			{
				((Engine::CTransform*)pPlayer->m_pWyvern->GetComponent(L"Transform"))->m_vPos.y += 
					((0.98f ) * fJumpAcc + fJumpAcc * 0.2f - 0.2f) * fTimeDelta;

				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_FLYING_IDLE);
				pPlayer->m_pWyvern->Set_Down(true);
			}
			else
			{
				pPlayer->m_pWyvern->Set_TrackID(CWyverns::WY_JUMP_LAND);
			}

			bJump_Wyvern = true;
		}
	}

	if(true == bJump_Wyvern)
		return true;

	return false;
}

void CPlayers::KeyCheck(const _float& fTimeDelta, CPlayers* pPlayer)
{
	if(g_bPause)
		return;

	if(true == pPlayer->m_bConstance)
	{
		m_eTrackID = SK_CONSTANCE;
		return;
	}

	if(m_bActStateSet.test(ACT_STUN) ||
		m_bActStateSet.test(ACT_DMG_HIT) ||
		m_bActStateSet.test(ACT_HIT_UP))
			return;

	if(NULL == m_pDynamicCamera)
		m_pDynamicCamera = ((CDynamicCamera*)Engine::Get_ObjList(LAYER_UI,L"DynamicCamera")->front());

	KeyInput_Static(fTimeDelta,pPlayer);
	KeyInput_Skill(fTimeDelta,pPlayer);
	KeyInput_Combo(fTimeDelta,pPlayer);

	if (!pPlayer->m_bActStateSet.any() &&
		!pPlayer->m_bAtkComboSet.any() &&
		!pPlayer->m_bSkillSet.any())
	{
		if(false == KeyInput_Move(fTimeDelta, pPlayer))
		{		
			m_eTrackID = STAND_NORMAL;
		}
	}

}

void CPlayers::SetMainPlayer(void)
{
	m_bMainPlayer = true;
}
void CPlayers::ResetMainPlayer(void)
{
	m_bMainPlayer = false;
}

void CPlayers::AngleSetting()
{
	if(NULL != m_pDynamicCamera)
	{
		m_pDynamicCamera->Set_Free(true);

		_float fTemp = m_pInfo->m_fAngle[Engine::ANGLE_Y];

		m_pInfo->m_fAngle[Engine::ANGLE_Y] += m_pDynamicCamera->Get_fAngle_Yaw();
		if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
		
		m_pDynamicCamera->Set_fAngle_Yaw(0.f);
		m_pInfo->Update();
	}
}

void CPlayers::TargetSetting(CPlayers* pPlayer /*= NULL*/)
{
	if(NULL == pPlayer)
		return;

	_myvec vTargetDir = _vec3(0,0,0);
	_myvec vNormal = _vec3(0,0,0);
	_myvec vPlayerDir = _vec3(((Engine::CTransform*)pPlayer->GetComponent(L"Transform"))->m_vDir.x,0.f,((Engine::CTransform*)pPlayer->GetComponent(L"Transform"))->m_vDir.z);
	_float fDis = 0.f;

	vector<CMonsters*>* pvecMonsters = pPlayer->Get_vecMonsters();

	/*if(CKeyMgr::GetInstance()->GetKeyDown(VK_INSERT))
		__debugbreak();*/

	for(UINT i = 0; i < pvecMonsters->size(); ++i)
	{
		if(((CMonsters*)((*pvecMonsters)[i]))->Get_bDieState())
		{
			if(pPlayer->Get_TargetMonster() == ((CMonsters*)((*pvecMonsters)[i])))
				pPlayer->Set_TagetMonster(NULL);

			if(pPlayer->Get_MainPlayer() && pPlayer->Get_TargetMonster() != NULL)
			{
				pPlayer->Set_TagetMonster(NULL);
			}
			continue;
		}

		if(NULL != pPlayer->Get_TargetMonster())
		{
			if(pPlayer->Get_TargetMonster()->Get_bDieState())
			{
				pPlayer->Set_TagetMonster(NULL);
				continue;
			}
			_vec3 vTargetPos(((Engine::CTransform*)pPlayer->m_pTargetMonster->GetComponent(L"Transform"))->m_vPos.x ,0.f,
				((Engine::CTransform*)pPlayer->m_pTargetMonster->GetComponent(L"Transform"))->m_vPos.z);
			
			_vec3 vPlayerPos(((Engine::CTransform*)pPlayer->GetComponent(L"Transform"))->m_vPos.x , 0.f,
				((Engine::CTransform*)pPlayer->GetComponent(L"Transform"))->m_vPos.z);

			vTargetDir = vTargetPos - vPlayerPos;
			pPlayer->m_fCurrentDis = vTargetDir.Length();
		
			vTargetDir.Normalize();
			vPlayerDir.Normalize();

			_float fAngle = D3DXVec3Dot(&vTargetDir,&vPlayerDir);
			_float fRadian = acosf(fAngle);

			/// 플레이어에게 타겟의 거리 , 각도 , 방향 넣어준다

			pPlayer->m_vTargetDir = vTargetDir;	/// 디렉션 y 는 제외

			if(pPlayer->m_fCurrentDis < pPlayer->m_fTargetDis)
			{
				if(true == pPlayer->m_bMainPlayer)
				{
					/// 현재 플레이어가 메인플레이어 일때 시야각으로 검사
					if(pPlayer->Get_PlayerJob() == JOB_RANGER || pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
					{
						if(fAngle < 1.f && fAngle >= 0.7f)
							break;
						else
						{
							pPlayer->Set_TagetMonster(NULL);
							continue;
						}
					}
					else
					{
						if(fAngle < 1.f && fAngle >= 0.95f)
							break;

						else
						{
							pPlayer->Set_TagetMonster(NULL);
							continue;
						}
					}
				}
				else			/// 메인플레이어가 아니라면 range 검사만 한다
					break;	
			}
			else
			{
				pPlayer->Set_TagetMonster(NULL);
				continue;
			}
		}
		else
		{
			_vec3 vMonsterPos(((Engine::CTransform*)(((CMonsters*)((*pvecMonsters)[i]))->GetComponent(L"Transform")))->m_vPos.x , 0.f,
				((Engine::CTransform*)(((CMonsters*)((*pvecMonsters)[i]))->GetComponent(L"Transform")))->m_vPos.z);

			_vec3 vPlayerPos(((Engine::CTransform*)pPlayer->GetComponent(L"Transform"))->m_vPos.x , 0.f ,
				((Engine::CTransform*)pPlayer->GetComponent(L"Transform"))->m_vPos.z);

			vTargetDir = vMonsterPos - vPlayerPos;
			fDis = vTargetDir.Length();
			vTargetDir.Normalize();
			vPlayerDir.Normalize();
			_float fTargetDis = 0.f;
			fTargetDis = pPlayer->m_fTargetDis;

			_float fAngle = vTargetDir.Dot(&vPlayerDir);

			if(fDis < fTargetDis)
			{
				if(false == pPlayer->m_bMainPlayer)
				{
					pPlayer->Set_TagetMonster(((CMonsters*)((*pvecMonsters)[i])));
					break;
				}
				else
				{
					if(pPlayer->Get_PlayerJob() == JOB_RANGER || pPlayer->Get_PlayerJob() == JOB_MAGICIAN)
					{
						if(pPlayer->Get_MainPlayer())
						{
							if(fAngle < 1.f && fAngle >= 0.7f)
							{
								pPlayer->Set_TagetMonster(((CMonsters*)((*pvecMonsters)[i])));
								break;
							}
						}
						else
						{
							pPlayer->Set_TagetMonster(((CMonsters*)((*pvecMonsters)[i])));
							break;
						}
					}
					else
					{
						if(fAngle < 1.f && fAngle >= 0.95f)
						{
							pPlayer->Set_TagetMonster(((CMonsters*)((*pvecMonsters)[i])));
							break;
						}
					}
					
				}
				
			}
			else
				continue;
			
		}
	}
}

void CPlayers::RangeSkill_State(SKILL_STATE eSkillType,_vec3 vPos /*= _vec3(0,0,0)*/,_float fDMG/* = 20.f*/,_float fRange /*= 20.f*/,CPlayers* pPlayer /*= NULL*/)
{
	if(NULL == pPlayer)
		return;

	_vec3 vPoint(0,0,0); 
	vPoint = vPos;
		
	PLAYERJOB ePlayerJob = pPlayer->Get_PlayerJob();
	HRESULT hr = NULL;

	if(ePlayerJob == JOB_END)
		return;

	if(ePlayerJob == JOB_GUARDIAN)
	{
		switch(eSkillType)
		{
		case SKILL_1:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_3:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_5:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_2:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		}

		if(hr == S_OK)
			return;
	}
	if(ePlayerJob == JOB_PALADIN)
	{
		switch(eSkillType)
		{
		case SKILL_1:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_3:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_4:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_5:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_DASH:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		}
		if(hr == S_OK)
			return;
	}
	if(ePlayerJob == JOB_RANGER)
	{
		if(eSkillType == SKILL_1)
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
		if(hr == S_OK)
			return;
	}
	if(ePlayerJob == JOB_MAGICIAN)
	{
		switch(eSkillType)
		{
		case SKILL_1:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_2:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_3:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_4:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		case SKILL_5:
			hr = TargetRange_Collision(fDMG,fRange,vPoint,pPlayer,pPlayer->Get_vecMonsters());
			break;
		}
		if(hr == S_OK)
			return;
	}

}

HRESULT CPlayers::TargetRange_Collision(_float fDMG,_float fRange,_vec3 vPos ,CPlayers* pPlayer, vector<CMonsters*>* pvecMonster /*= NULL*/)
{
	if(pvecMonster == NULL || pPlayer == NULL)
		return E_FAIL;


	for(_uint i = 0; i < pvecMonster->size(); ++i)
	{
		if((((CMonsters*)((*pvecMonster)[i])))->Get_bDieState())
				continue;

		_vec3 vMonsterPos(((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.x , 0.f,
			((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.z);
		
		_vec3 vFontPos =((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos;
		_myvec vDir = vPos - vMonsterPos;
		_float fDis = vDir.Length();

		if(fDis < fRange)
		{
			if(pPlayer->Get_PlayerJob() == JOB_GUARDIAN)
			{
				if(pPlayer->Get_TrackID() == SK_1)
				{
					((CMonsters*)((*pvecMonster)[i]))->Set_bDMG_Timing(true);
					_vec3 vEffectPos = _vec3(((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.x,
											((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.y + 0.3f,
											((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.z);

					Engine::Call_EtcFX(Engine::ETC_HITSLASH,&vEffectPos,false);
				}
				if(pPlayer->Get_TrackID() == SK_2)
				{
					((CMonsters*)((*pvecMonster)[i]))->Set_bStunState(true);
					_vec3 vEffectPos = _vec3(((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.x,
						((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.y + 0.3f,
						((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.z);
					Engine::Call_EtcFX(Engine::ETC_HITCIRCLE,&vEffectPos,false);
				}
				if(pPlayer->Get_TrackID() == SK_4)
				{
					((CMonsters*)((*pvecMonster)[i]))->Set_bDownState(true);
					_vec3 vEffectPos = _vec3(((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.x,
						((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.y + 0.3f,
						((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos.z);
					Engine::Call_EtcFX(Engine::ETC_HITSLASH,&vEffectPos,false);
				}
				if(pPlayer->Get_TrackID() == SK_0)
				{
					(*((CMonsters*)((*pvecMonster)[i]))->Get_Aggro_Accumulate())[0] = 250.f;
					Engine::Call_GuardianFX(Engine::G_SKILL1,&((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos,true);
				}
			}
			if(pPlayer->Get_PlayerJob() == JOB_PALADIN)
			{
				if(pPlayer->Get_TrackID() == DASH)
					Engine::Call_PaladinFX(Engine::P_DASH_TARGET,&((Engine::CTransform*)(((CMonsters*)((*pvecMonster)[i]))->GetComponent(L"Transform")))->m_vPos,false);
			}
			((CMonsters*)((*pvecMonster)[i]))->Set_DMG_Check(fDMG,(CUnits*)pPlayer,Engine::CStatus::DMG_MELEE);
			const_cast<Engine::CStatus*>((((CMonsters*)(*pvecMonster)[i]))->GetStatus())->Damaged((((CMonsters*)(*pvecMonster)[i]))->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
			Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &vFontPos,fDMG));

			CPlayers* pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer();
			if(pMainPlayer == NULL)
				continue;

			if(pMainPlayer->Get_PlayerJob() == JOB_RANGER)
			{
				if(pMainPlayer->Get_TrackID() == SK_0)
					CSoundMgr::GetInstance()->PlayRangerSound3(L"RangerEffectHit1.ogg",0.5f);
			}
		}
		else
			continue;
	}
	return S_OK;
}

void CPlayers::SetFormation(_vec3 vPos)
{
	m_vFormation = vPos;
}

void CPlayers::Set_RangeSkill_Check(_vec3 vPos, bool bCheck, _float fLength)
{
	m_bEvasion_Check = bCheck;
	m_vEvasionPos = vPos;
	m_fEvasion_Length = fLength;
}

bool CPlayers::EvasionRange()
{
	if(m_bEvasion_Check)
	{
		_myvec vEvasionDir;
		vEvasionDir = m_vEvasionPos - m_pInfo->m_vPos;
		_float fDis = vEvasionDir.Length();
		vEvasionDir.Normalize();
		_float fRadian = acosf(vEvasionDir.Dot(&m_vLook));

		if(m_pInfo->m_vPos.z > m_vEvasionPos.z)
			fRadian = -fRadian;
		else
			fRadian = fRadian;

		if(fDis < m_fEvasion_Length + 0.5f)
		{
			MoveOnNaviMesh(&(-vEvasionDir * (0.7f) * m_fTimeDelta));
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fRadian;
			m_eTrackID = RUN_B;
			return true;
		}
		else
		{
			m_bEvasion_Check = false;
			return false;
		}
	}
	else
		return false;

	return false;
}

void CPlayers::SKill_CoolTime()
{
	if(m_bCoolTimeSet.test(SKILL_1))
	{
		if(m_fSkillCooltime[SKILL_1] > m_fSkillCooltime_Max[SKILL_1])
		{
			m_bCoolTimeSet.reset(SKILL_1);
			m_fSkillCooltime[SKILL_1] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_1] += m_fTimeDelta;
	}
	if(m_bCoolTimeSet.test(SKILL_2))
	{
		if(m_fSkillCooltime[SKILL_2] > m_fSkillCooltime_Max[SKILL_2])
		{
			m_bCoolTimeSet.reset(SKILL_2);
			m_fSkillCooltime[SKILL_2] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_2] += m_fTimeDelta;
	}
	if(m_bCoolTimeSet.test(SKILL_3))
	{
		if(m_fSkillCooltime[SKILL_3] > m_fSkillCooltime_Max[SKILL_3])
		{
			m_bCoolTimeSet.reset(SKILL_3);
			m_fSkillCooltime[SKILL_3] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_3] += m_fTimeDelta;
	}
	if(m_bCoolTimeSet.test(SKILL_4))
	{
		if(m_fSkillCooltime[SKILL_4] > m_fSkillCooltime_Max[SKILL_4])
		{
			m_bCoolTimeSet.reset(SKILL_4);
			m_fSkillCooltime[SKILL_4] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_4] += m_fTimeDelta;
	}
	if(m_bCoolTimeSet.test(SKILL_5))
	{
		if(m_fSkillCooltime[SKILL_5] > m_fSkillCooltime_Max[SKILL_5])
		{
			m_bCoolTimeSet.reset(SKILL_5);
			m_fSkillCooltime[SKILL_5] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_5] += m_fTimeDelta;

	}
	if(m_bCoolTimeSet.test(SKILL_DASH))
	{
		if(m_fSkillCooltime[SKILL_DASH] > m_fSkillCooltime_Max[SKILL_DASH])
		{
			m_bCoolTimeSet.reset(SKILL_DASH);
			m_fSkillCooltime[SKILL_DASH] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_DASH] += m_fTimeDelta;

	}
	if(m_bCoolTimeSet.test(SKILL_INNATE))
	{
		if(m_fSkillCooltime[SKILL_INNATE] > m_fSkillCooltime_Max[SKILL_INNATE])
		{
			m_bCoolTimeSet.reset(SKILL_INNATE);
			m_fSkillCooltime[SKILL_INNATE] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_INNATE] += m_fTimeDelta;
	}
	if(m_bCoolTimeSet.test(SKILL_DEFAULT))
	{
		if(m_fSkillCooltime[SKILL_DEFAULT] > m_fSkillCooltime_Max[SKILL_DEFAULT])
		{
			m_bCoolTimeSet.reset(SKILL_DEFAULT);
			m_fSkillCooltime[SKILL_DEFAULT] = 0.f;
		}
		else
			m_fSkillCooltime[SKILL_DEFAULT] += m_fTimeDelta;
	}


}

void CPlayers::Set_DamageType(ACT_STATE state, float fDMG , Engine::CStatus::DAMAGETYPE damageType)
{
	if(fDMG > 0.f)
	{
		if(m_eTypeJob == JOB_GUARDIAN)
		{
			if(m_eTrackID == HIDDEN_SKILL)
			{
				m_fAcc -= 2.f * m_fTimeDelta;
				MoveOnNaviMesh(&(m_pInfo->m_vDir * m_fTimeDelta * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f)));
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"MonsterDamage",CMonsterDmg::Create(m_pDevice, &m_pInfo->m_vPos,0.f));
			}
			else
			{
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"MonsterDamage",CMonsterDmg::Create(m_pDevice, &m_pInfo->m_vPos,fDMG));
				m_pStatus->Damaged(fDMG,damageType);
			}
		}
		if(m_eTypeJob == JOB_PALADIN)
		{
			if(m_eTrackID == HIDDEN_SKILL)
			{
				m_fAcc -= 2.f * m_fTimeDelta;
				MoveOnNaviMesh(&(m_pInfo->m_vDir * m_fTimeDelta * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f)));
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"MonsterDamage",CMonsterDmg::Create(m_pDevice, &m_pInfo->m_vPos,0.f));
			}
			else
			{
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"MonsterDamage",CMonsterDmg::Create(m_pDevice, &m_pInfo->m_vPos,fDMG));
				m_pStatus->Damaged(fDMG,damageType);
			}
		}
		if(m_eTypeJob == JOB_RANGER)
		{
			Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"MonsterDamage",CMonsterDmg::Create(m_pDevice, &m_pInfo->m_vPos,fDMG));
			m_pStatus->Damaged(fDMG,damageType);
		}
		if(m_eTypeJob == JOB_MAGICIAN)
		{
			Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"MonsterDamage",CMonsterDmg::Create(m_pDevice, &m_pInfo->m_vPos,fDMG));
			m_pStatus->Damaged(fDMG,damageType);
		}
		/// 타입 넘겨라 ACT_STATE 이넘문은 Players 에 있다
		if(state != ACT_DEFAULT)	/// 디폴트 주면 상태 안줌
		{
			if(!m_bActStateSet.test(state))			
				m_bActStateSet.set(state);
		}
	}
}

//void CPlayers::Set_ActBitset(ACT_STATE state , _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bActStateSet.set();
//	else
//		m_bActStateSet.set(state);
//	bCheck == true ? m_bActStateSet.set() : m_bActStateSet.set(state);
//}
//
//void CPlayers::Reset_ActBitset(ACT_STATE state , _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bActStateSet.reset();
//	else
//		m_bActStateSet.reset(state);
//}
//
//void CPlayers::Set_AtkBitset(ATK_COMBO state , _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bAtkComboSet.set();
//	else
//		m_bAtkComboSet.set(state);
//}
//
//void CPlayers::Reset_AtkBitset(ATK_COMBO state , _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bAtkComboSet.reset();
//	else
//		m_bAtkComboSet.reset(state);
//}
//
//void CPlayers::Set_KeyBitset(KEY_STATEID state , _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bKeyStateset.set();
//	else
//		m_bKeyStateset.set(state);
//}
//
//void CPlayers::Reset_KeyBitset(KEY_STATEID state , _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bKeyStateset.reset();
//	else
//		m_bKeyStateset.reset(state);
//}
//
//void CPlayers::Set_SkillBitset(SKILL_STATE state, _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bSkillSet.set();
//	else
//		m_bSkillSet.set(state);
//}
//
//void CPlayers::Reset_SkillBitset(SKILL_STATE state, _bool bCheck /*= false*/)
//{
//	if(bCheck)
//		m_bSkillSet.reset();
//	else
//		m_bSkillSet.reset(state);
//}
//
//void CPlayers::Set_SpecRange(float fRange)
//{
//	if(m_fSpecRange >= 0.f)
//		m_fSpecRange += fRange;
//	else
//		m_fSpecRange = 0.f;
//}

void CPlayers::Free(void)
{
	CUnits::Free();
	Engine::CGameObject::Free();

	m_pWyvern = NULL;
	m_pDynamicCamera = NULL;
	m_pMainPlayer = NULL;
	m_pTargetMonster = NULL;
	m_vecMonsters.clear();
}

void CPlayers::MoveOnNaviMesh(D3DXVECTOR3* pDir, CPlayers* pPlayer/* = NULL*/)
{
	if(pPlayer)
		pPlayer->m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos, pDir, pPlayer->m_dwIndex, pPlayer->m_fHeight);
	else
		m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos, pDir, m_dwIndex, m_fHeight);
}