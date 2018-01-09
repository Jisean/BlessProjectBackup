/*!
 * \file Status.h
 * \date 2016/03/30 10:28
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Status_h__
#define Status_h__

#include "Component.h"

BEGIN(Engine)

enum STATUSMAG
{
	STMAG_STR = 6,
	STMAG_INT = 6,
	STMAG_HEALTH = 15,
	STMAG_DEX = 6,
	STMAG_WIS = 10,
	STMAG_EXS = 5
};

enum LEVELUP
{
	LVUP_STR = 12,
	LVUP_INT = 12,
	LVUP_HEALTH = 25,
	LVUP_DEX = 9,
	LVUP_WIS = 9,
	LVUP_EXS = 6
};

struct _BaseStat
{
	float	m_fLevel;	// 레벨
	float	m_fStr;		// 힘
	float	m_fInt;		// 지능
	float	m_fHealth;	// 체력
	float	m_fDex;		// 민첩
	float	m_fWis;		// 지혜
	float	m_fExStat;	// 특수스탯
};

struct _RealStat
{
	float	m_fMaxHp;		// 만피
	float	m_fCurHp;		// 현재피
	float	m_fMaxMana;		// 맥스마나
	float	m_fCurMana;		// 현재마나
	float	m_fMeleeDamage;	// 물리뎀
	float	m_fMagicDamage;	// 마법뎀
	float	m_fAccurate;	// 적중
	float	m_fCrit;		// 크리티컬
	float	m_fMeleeResist;	// 방어도
	float	m_fMagicResist;	// 마법방어
	float	m_fExp;
};

class ENGINE_DLL CStatus
	: public CComponent
{
public:
	enum DAMAGETYPE { DMG_MELEE, DMG_MAGIC };
private:
	explicit CStatus(_BaseStat& tBaseStat);
public:
	virtual ~CStatus(void);

public:	// 기본 능력치
	_BaseStat			m_tBaseStat;
public: // 실제 능력치
	_RealStat			m_tRealStat;
public:
	float				m_fGetDamaged;
	float				m_fSpendMana;
	float				m_fHealing;
	float				m_fManaRegen;
	float				m_fTotalDamage;
	float				m_fMaxExp;
	float				m_fCurExp;
	float				m_fResist;
	float				m_fAtkBuff;
public:
	virtual void Update(void);
	HRESULT	Initialize(void);
	void	Damaged(const float& fDamage, DAMAGETYPE eDmgType);
	void	SpendMana(const float& fMana);
	void	GetExp(const float& fExp);
	void	LevelUp(void);
	void	Healing(const float& fHeal);
	void	ManaRegen(const float& fMGen);
	void	ResistUp(const float& fResist);
	void	AtkBuff(const float& fAtkBuff);
public:
	////////////////////////////////////////////////////////////////////////// GET
	bool	Get_Percent(float fPercent);
public:
	static CStatus* Create(_BaseStat& tBaseStat);
	virtual void Free(void);
};

END

#endif // Status_h__