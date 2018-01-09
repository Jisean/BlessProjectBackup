#include "stdafx.h"
#include "NPCs.h"

CNPCs::CNPCs(LPDIRECT3DDEVICE9 pDevice)
: CUnits(pDevice)
, m_fTimeDelta(0.f)
, m_fSlowTime(1.f)
, m_fSpeed(0.f)
, m_iAniIdx(0)
, m_iPrevAniIdx(0)
, m_bBossCam(false)
{
	//m_bTest = false;
	m_bNPC_Collision = false;
	m_bStage2 = false;

	m_fFrameSpeedCtr = 1.f;
	m_bDieStart = false;
	m_bDieState = false;
	m_fDie_VanishTime = 0.f;

	m_dwMyIndex = 0;
	m_pTargetInfo = NULL;
	m_pAttPosition = NULL;

	m_fOriAngle = 0.f;

	m_fAtt_DelayTime = 2.f;
	m_fCautionRange = 0.f;
	m_fCollisionRange = 0.f;
	m_fStart_Chase = 0.f;
	m_fAttackDist = 0.5f;


	m_eUnitType = CUnits::TYPEID_NPC;
	m_bIninInfo = false;
	m_AttackStyle = false;

	m_bTarget_Attack = false;
	m_bAttack = false;
	m_bAttackEnd = false;
	m_bRelocation = false;


	m_bDungeonNPC = false;
	m_pMainPlayer = NULL;
}

CNPCs::~CNPCs(void)
{

}

_int CNPCs::Update(const _float& fTimeDelta)
{
	return 0;
}

float CNPCs::Get_Speed()
{
	return m_fSpeed;
}

