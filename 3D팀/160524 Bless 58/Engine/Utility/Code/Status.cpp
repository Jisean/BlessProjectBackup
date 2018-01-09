#include "Status.h"

Engine::CStatus::CStatus(_BaseStat& tBaseStat)
: m_fGetDamaged(0.f)
, m_fSpendMana(0.f)
, m_fTotalDamage(0.f)
, m_tBaseStat(tBaseStat)
, m_fHealing(0.f)
, m_fManaRegen(0.f)
, m_fResist(1.f)
, m_fAtkBuff(1.f)
{
	ZeroMemory(&m_tRealStat, sizeof(_RealStat));
}

Engine::CStatus::~CStatus(void)
{

}

HRESULT Engine::CStatus::Initialize()
{
	m_tRealStat.m_fMaxHp		= m_tBaseStat.m_fHealth * STMAG_HEALTH;
	m_tRealStat.m_fCurHp		= m_tRealStat.m_fMaxHp;
	m_tRealStat.m_fMaxMana		= m_tBaseStat.m_fWis * STMAG_WIS;
	m_tRealStat.m_fCurMana		= m_tRealStat.m_fMaxMana;

	m_tRealStat.m_fMeleeDamage	= m_tBaseStat.m_fStr * STMAG_STR;
	m_tRealStat.m_fMagicDamage	= m_tBaseStat.m_fInt * STMAG_INT;

	m_tRealStat.m_fCrit			= m_tBaseStat.m_fDex * STMAG_DEX;
	m_tRealStat.m_fAccurate		= m_tBaseStat.m_fDex * STMAG_DEX;

	m_tRealStat.m_fMeleeResist	= m_tBaseStat.m_fHealth * STMAG_HEALTH;
	m_tRealStat.m_fMagicResist	= m_tBaseStat.m_fWis * STMAG_WIS;

	m_tRealStat.m_fExp			= m_tBaseStat.m_fLevel * 128.f;
	m_fMaxExp = powf(2.f, (m_tBaseStat.m_fLevel - 1.f)) * 100.f;

	return S_OK;
}

void Engine::CStatus::Update(void)
{
	if(0 != m_fGetDamaged)
	{
		m_tRealStat.m_fCurHp -= m_fGetDamaged;
		if(m_tRealStat.m_fCurHp <= 0)
			m_tRealStat.m_fCurHp = 0.f;

		m_fGetDamaged = 0.f;
	}
	if(0 != m_fHealing)
	{
		m_tRealStat.m_fCurHp += m_fHealing;
		m_fHealing = 0.f;
	}
	if(0 != m_fSpendMana)
	{
		m_tRealStat.m_fCurMana -= m_fSpendMana;
		if(m_tRealStat.m_fCurMana <= 0)
			m_tRealStat.m_fCurMana = 0.f;

		m_fSpendMana = 0.f;
	}
	if(0 != m_fManaRegen)
	{
		m_tRealStat.m_fCurMana += m_fManaRegen;
		m_fManaRegen = 0.f;
	}

	if(m_tRealStat.m_fCurHp >= m_tRealStat.m_fMaxHp)
		m_tRealStat.m_fCurHp = m_tRealStat.m_fMaxHp;
	if(m_tRealStat.m_fCurMana >= m_tRealStat.m_fMaxMana)
		m_tRealStat.m_fCurMana = m_tRealStat.m_fMaxMana;
}

Engine::CStatus* Engine::CStatus::Create(_BaseStat& tBaseStat)
{
	CStatus* pStat = new CStatus(tBaseStat);
	
	if(FAILED(pStat->Initialize()))
		Engine::Safe_Delete(pStat);
	
	return pStat;
}

void Engine::CStatus::Free(void)
{

}

void Engine::CStatus::Damaged(const float& fDamage, DAMAGETYPE eDmgType)
{
	// 맞을때 몹-플레이어 둘다
	float fGetDamage = 0.f;
	switch(eDmgType)
	{
	case DMG_MELEE:
		fGetDamage = fDamage - m_tRealStat.m_fMeleeResist * 0.05f;
		m_fGetDamaged = fGetDamage <= 20.f ? 20.f : fGetDamage;
		break;

	case DMG_MAGIC:
		fGetDamage = fDamage - m_tRealStat.m_fMagicResist * 0.05f;
		m_fGetDamaged = fGetDamage <= 20.f ? 20.f : fGetDamage;
		break;
	}
}

void Engine::CStatus::SpendMana(const float& fMana)
{
	// 스킬 쓸때
	m_fSpendMana = fMana;
}

void Engine::CStatus::GetExp(const float& fExp)
{
	// 몹 죽을때 플레이어한테 보낸다.
	if(m_fCurExp + fExp >= m_fMaxExp)
	{
		m_fCurExp = m_fCurExp + fExp - m_fMaxExp;
		LevelUp();
	}
	else
		m_fCurExp += fExp;
}

void Engine::CStatus::LevelUp(void)
{
	m_tBaseStat.m_fLevel		+= 1.f;

	m_tBaseStat.m_fStr			+= (float)LVUP_STR;
	m_tBaseStat.m_fInt			+= (float)LVUP_INT;
	m_tBaseStat.m_fHealth		+= (float)LVUP_HEALTH;
	m_tBaseStat.m_fDex			+= (float)LVUP_DEX;
	m_tBaseStat.m_fWis			+= (float)LVUP_WIS;
	m_tBaseStat.m_fExStat		+= (float)LVUP_EXS;

	m_tRealStat.m_fMaxHp		= m_tBaseStat.m_fHealth * STMAG_HEALTH;
	m_tRealStat.m_fCurHp		= m_tRealStat.m_fMaxHp;
	m_tRealStat.m_fMaxMana		= m_tBaseStat.m_fWis * STMAG_WIS;
	m_tRealStat.m_fCurMana		= m_tRealStat.m_fMaxMana;
								  
	m_tRealStat.m_fMeleeDamage	= m_tBaseStat.m_fStr * STMAG_STR;
	m_tRealStat.m_fMagicDamage	= m_tBaseStat.m_fInt * STMAG_INT;
								  
	m_tRealStat.m_fCrit			= m_tBaseStat.m_fDex * STMAG_DEX;
	m_tRealStat.m_fAccurate		= m_tBaseStat.m_fDex * STMAG_DEX;
								  
	m_tRealStat.m_fMeleeResist	= m_tBaseStat.m_fHealth * STMAG_HEALTH;
	m_tRealStat.m_fMagicResist	= m_tBaseStat.m_fWis * STMAG_WIS;
	
	m_fMaxExp = powf(2.f, (m_tBaseStat.m_fLevel - 1.f)) * 100.f;
}

bool Engine::CStatus::Get_Percent(float fPercent)
{
	if(m_tRealStat.m_fCurHp < m_tRealStat.m_fMaxHp * (fPercent * 0.01f))	// hp 가 60 퍼센트 미만 일때.
		return true;

	return false;
}

void Engine::CStatus::Healing(const float& fHeal)
{
	m_fHealing += m_tRealStat.m_fMaxHp * fHeal * 0.01f;
}

void Engine::CStatus::ManaRegen(const float& fMGen)
{
	m_fManaRegen += m_tRealStat.m_fMaxMana * fMGen * 0.01f;
}

void Engine::CStatus::ResistUp(const float& fResist)
{
	m_tRealStat.m_fMeleeResist = fResist;
}

void Engine::CStatus::AtkBuff(const float& fAtkBuff)
{
	if(fAtkBuff < 1.f)
		m_fAtkBuff = 1.f;
	else
		m_fAtkBuff = fAtkBuff;
}
