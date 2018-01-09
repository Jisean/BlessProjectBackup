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
	float	m_fLevel;	// ����
	float	m_fStr;		// ��
	float	m_fInt;		// ����
	float	m_fHealth;	// ü��
	float	m_fDex;		// ��ø
	float	m_fWis;		// ����
	float	m_fExStat;	// Ư������
};

struct _RealStat
{
	float	m_fMaxHp;		// ����
	float	m_fCurHp;		// ������
	float	m_fMaxMana;		// �ƽ�����
	float	m_fCurMana;		// ���縶��
	float	m_fMeleeDamage;	// ������
	float	m_fMagicDamage;	// ������
	float	m_fAccurate;	// ����
	float	m_fCrit;		// ũ��Ƽ��
	float	m_fMeleeResist;	// ��
	float	m_fMagicResist;	// �������
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

public:	// �⺻ �ɷ�ġ
	_BaseStat			m_tBaseStat;
public: // ���� �ɷ�ġ
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