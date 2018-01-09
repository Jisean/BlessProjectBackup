#include "stdafx.h"
#include "Monsters.h"
#include "Export_Function.h"
#include "Transform.h"

CMonsters::CMonsters(LPDIRECT3DDEVICE9 pDevice)
: CUnits(pDevice)
{
	m_pSkillRange = NULL;
	m_pSkillRange_1 = NULL;

	m_dwIndex = 0;
	m_pTargetInfo = NULL;
	m_pChangeTargetInfo = NULL;
	m_bInitInfo = false;
	 
	m_bBoss_Type = false;
	m_bSummon_type = false;

	m_bTarget_Attack = false;

	m_AttackStyle = false;

	m_bAttack = false;
	m_bAttackEnd = false;

	m_bSkillEnd = false;
	m_bOnSkill_Range = false;
	m_bRelocation = false;
	m_bDMG_Timing = false;

	m_bHitState = false;
	m_bSkillState = false;
	m_bDownState = false;
	m_bStunState = false;
	m_bDieState = false;

	m_bDieStart = false;

	m_bCall_Skill_State = false;

	m_fOriAngle = 0.f;

	m_fSpeed = 0.f;

	m_iAniIdx = 0;
	m_iPrevAniIdx = 0;
	m_iEnforce_Sk_Idx = 0;

	m_fCautionRange = 0.f;
	m_fCollisionRange = 0.f;
	m_fStart_Chase = 0.f;

	m_fFrameTime = 0.f;
	m_fAtt_DelayTime = 2.f;
	m_fNext_DelayTime = 0.f;

	m_fAttackDist = 0.5f;

	m_pAttPosition = NULL;


	m_fGetHit_DMG = 0.f;
	m_fDie_VanishTime = 0.f;

	m_fFrameSpeedCtr = 1.f;
	m_bMonster_Wake = false;

	m_ePlayer_ACT_Type = ACT_DEFAULT;	
	m_eUnitType = CUnits::TYPEID_MONSTER;

	// 어그로 누적치
	m_vecAggro_Accumulate.push_back(0.f);   // 가디언
	m_vecAggro_Accumulate.push_back(0.f);	// 팔라딘
	m_vecAggro_Accumulate.push_back(0.f);	// 매지션
	m_vecAggro_Accumulate.push_back(0.f);	// 궁수

	m_fAggroTime = 0.f;

	m_bTarget_Change = false;
	m_fMAX_AggroValue = 250.f;
	m_fTargetFix_Time = 3.f;
	
	m_bSearch = false;


	m_fSkillTime = 0.f;
	m_dwMyIndex = (DWORD)0.0;
	// 스킬 쿨타임 다 찼을 때 해당 스킬 쿨타임의 인덱스를 저장 
	// 캔슬 할때 누군인지 판별하기 위함
	m_iCoolTime_FullIndex = 7;   // 초기값은 7
//public
	m_bTest = false;
}

CMonsters::~CMonsters(void)
{
	m_vecAggro_Accumulate.clear();
}

_int CMonsters::Update(const _float& fTimeDelta)
{
	return 0;
}

void CMonsters::Set_DMG_Check(_float fDMG, CUnits* pUnit /*= NULL*/, Engine::CStatus::DAMAGETYPE eDamageType /*= Engine::CStatus::DMG_MELEE*/)
{ 
	if(pUnit != NULL)
	{
		if(pUnit->Get_UnitType() == CUnits::TYPEID_PLAYER)
			m_pChangeTargetInfo = pUnit;
	}

	m_fGetHit_DMG = fDMG;
	m_eDamageType = eDamageType;
}

void CMonsters::Skill_State(const _float& fTimeDelta)
{

}

void CMonsters::Full_SkillCoolTime_Reset(int iIndex)
{
	m_vecSK_Cooltime[iIndex] = 0.f;
}

void CMonsters::Sound(DWORD dwMyIndex)
{

}

