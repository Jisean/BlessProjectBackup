/*!
 * \file Monsters.h
 * \date 2016/05/20 14:57
 *
 * \author 권오훈
 * Contact: user@company.com
 *
 * \brief 몬스터 부모
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Monsters_h__
#define Monsters_h__

#include "Units.h"
#include "Status.h"
#include "Collision.h"

class CSkill_Range;
class CSkill_Range_1;
class CPlayers;
class CMonsters : public CUnits
{
protected:
	explicit CMonsters(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMonsters(void);

public:
	//enum ACT_STATE{
	enum PLACT_STATE{ ACT_DASH,ACT_JUMP, ACT_MOUNT, ACT_STUN, ACT_CANCLE, ACT_EVASION, ACT_DOWN, ACT_DMG_HIT,ACT_HIT_UP,ACT_DIE,ACT_DEFAULT,ACT_END};	// 상태 이상 열거체

public:
	//========Get========//
	CSkill_Range_1*  Get_Skill_Range_1(void) { return m_pSkillRange_1; };
	DWORD		Get_DwIndex(void) { return m_dwIndex; };

	CCollision*  Get_collsion(void) { return m_pCollision; };
	CUnits*	Get_TargetInfo(void) { return m_pTargetInfo; };
	CUnits*	Get_ChangeTargetInfo(void) { return m_pChangeTargetInfo; };

	vector<ATTACK_DMG_TIMIMG*>* Get_Att_DMG_Timing(void) { return &m_vecAttack_DMG_Timing; };
	vector<ATTACK_DMG_TIMIMG*>* Get_Skill_DMG_Timing(void) { return &m_vecSkill_DMG_Timing; };

	vector<float>*		Get_Aggro_Accumulate(void) { return &m_vecAggro_Accumulate; };
	float				Get_AggroTime(void)	{ return m_fAggroTime; };
	float				Get_MAX_Aggro_Value(void)	{ return m_fMAX_AggroValue; };
	float				Get_TargetFix_Time(void)	{ return m_fTargetFix_Time; };
	bool				Get_bAggro_Target_Change(void) { return m_bTarget_Change; }; 
	bool				Get_bSearch(void) { return m_bSearch; }; 

	_vec3		Get_OriPos(void) { return	m_vOriPos; };
	float		Get_OriAngle(void) { return m_fOriAngle; };

	float		Get_Speed(void) { return m_fSpeed; };


	bool		Get_bBoss_Type(void) { return m_bBoss_Type; };
	bool		Get_bSummon_Type(void) { return m_bSummon_type; };

	bool		Get_Target_Attack(void) { return m_bTarget_Attack; };
	
	bool		Get_bAttackStyle(void) { return m_AttackStyle; };

	bool		Get_AttackEnd(void) { return m_bAttackEnd; };
	bool		Get_SkillEnd(void) { return m_bSkillEnd; };
	bool		Get_bAttack(void) { return m_bAttack; };
	
	bool		Get_bRelocation(void) { return m_bRelocation; };
	
	// 상태 이상
	bool		Get_bHitState(void) { return m_bHitState; };
	bool		Get_bSkillState(void) { return m_bSkillState; };
	bool		Get_bDownState(void) { return m_bDownState; };
	bool		Get_bStunState(void) { return m_bStunState; };
	bool		Get_bDieState(void) { return m_bDieState; };
	bool		Get_bDeadState(void) { return m_bDead; };
	bool		Get_bDMG_Timing(void) { return m_bDMG_Timing; };
 
	// 스킬 상태
	bool		Get_bCall_Skill_State(void) { return m_bCall_Skill_State; };
	bool		Get_bMonster_Wake(void) { return m_bMonster_Wake; };

	_uint		Get_AniIdx(void) { return m_iAniIdx; };
	_uint		Get_PrevAniIdx(void) { return m_iPrevAniIdx; };

	_float		Get_CautionRange(void) { return m_fCautionRange; };
	_float		Get_CollisionRange(void) { return m_fCollisionRange; };
	
	_float		Get_Start_Chase(void) { return m_fStart_Chase; };
	_float		Get_Att_DelayTime(void) { return m_fAtt_DelayTime; };
	_float		Get_fNext_DelayTime(void) { return m_fNext_DelayTime; };

	_float		Get_AttackDist(void) { return m_fAttackDist; };
	_float		Get_DMG_Check(void) { return m_fGetHit_DMG; };

	int		Get_CoolTime_FullIndex(void) { return m_iCoolTime_FullIndex; };
	// 데미지 타입
	Engine::CStatus::DAMAGETYPE  Get_eDamageType (void) { return m_eDamageType; };

	Engine::ATTPOSITION*		Get_AttackPos(void) { return m_pAttPosition; };

	PLACT_STATE		Get_Player_ACT_Type(void) { return m_ePlayer_ACT_Type; };
	
	//========Set========//
	void		Set_DwIndex(DWORD dIndex) { m_dwIndex = dIndex; };

	void		Set_TargetInfo(CUnits* pTarget) { m_pTargetInfo = pTarget; };
	void		Set_ChangeTargetInfo(CUnits* pTarget) { m_pChangeTargetInfo = pTarget; };
	
	void		Set_AggroTime(float fTime) { m_fAggroTime = fTime; };
	void		Set_bAggro_Target_Change(bool bType) { m_bTarget_Change = bType; };
	void		Set_bSearch(bool bType) { m_bSearch = bType; };

	void		Set_bSummon_Type(bool bType) { m_bSummon_type = bType; };

	void		Set_Target_Attack(bool bTarget) { m_bTarget_Attack = bTarget; };
	void		Set_AttackEnd(bool AttackEnd) { m_bAttackEnd = AttackEnd; };
	void		Set_SkillEnd(bool SkillEnd) { m_bSkillEnd = SkillEnd; };
	void		Set_bAttack(bool bCheck) { m_bAttack = bCheck; };
	
	void		Set_bRelocation(bool bCheck) { m_bRelocation = bCheck; };
	
	// 상태 이상
	void		Set_bHitState(bool bState) { m_bHitState = bState; };
	void		Set_bSkillState(bool bState) { m_bSkillState = bState; };
	void		Set_bDownState(bool bState) { m_bDownState = bState; };
	void		Set_bStunState(bool bState) { m_bStunState = bState; };
	void		Set_bDieState(bool bState) { m_bDieState = bState; };
	void		Set_bDeadState(bool bState) { m_bDead = bState; };
	void		Set_bDMG_Timing(bool bTiming) { m_bDMG_Timing = bTiming; };
	
	void		Set_bDieStart(bool	bType) { m_bDieStart = bType; };
	// 스킬 상태
	void		Set_bCall_Skill_State(bool bState) { m_bCall_Skill_State = bState; };
	void		Set_bMonster_Wake(bool bState) { m_bMonster_Wake = bState; };

	void		Set_AniIdx(_uint iAniIdx) { m_iAniIdx = iAniIdx; };
	void		Set_PrevAniIdx(_uint iAniIdx) { m_iPrevAniIdx = iAniIdx; };

	void		Set_Start_Chase(_float fTime) { m_fStart_Chase = fTime; };
	void		Set_Att_DelayTime(_float fTime) { m_fAtt_DelayTime = fTime; };
	void		Set_fNext_DelayTime(_float fTime) { m_fNext_DelayTime = fTime; };

	void		Set_AttackPos(Engine::ATTPOSITION* pInfo) { m_pAttPosition = pInfo; };
	void		Set_DMG_Check(_float fDMG, CUnits* pUnit = NULL, Engine::CStatus::DAMAGETYPE eDamageType = Engine::CStatus::DMG_MELEE);

	void		Set_FrameSpeedCtr(float fSpeedCtr) { m_fFrameSpeedCtr = fSpeedCtr; };

	void		Set_Player_ACT_Type(PLACT_STATE eType) { m_ePlayer_ACT_Type = eType; };
	
	void		Set_bSkill_Range(bool bSkill_Range) { m_bOnSkill_Range = bSkill_Range; };
	void		Set_SkillTime(float fTime) { m_fSkillTime = fTime; };
	void		Set_MyIndex(DWORD dwIndex) { m_dwMyIndex = dwIndex; };
	//======General======//
	virtual void		Skill_State(const _float& fTimeDelta);
	void				Full_SkillCoolTime_Reset(int iIndex);
	virtual void		Sound(DWORD dwMyIndex);



	// Test
public:
	bool			m_bTest;
protected:
	DWORD							m_dwIndex;
	DWORD							m_dwMyIndex;


protected:
	CSkill_Range*					m_pSkillRange;
	CSkill_Range_1*					m_pSkillRange_1;


	vector<CPlayers*>				m_vecPlayersInfo;
	vector<ATTACK_DMG_TIMIMG*>		m_vecAttack_DMG_Timing;
	vector<ATTACK_DMG_TIMIMG*>		m_vecSkill_DMG_Timing;
	vector<float>					m_vecSK_Cooltime;
	CUnits*							m_pTargetInfo;
	CUnits*							m_pChangeTargetInfo;

	// 어그로 누적치 설정
	vector<float>					m_vecAggro_Accumulate;
	float							m_fAggroTime;
	float							m_fMAX_AggroValue;
	float							m_fTargetFix_Time;
	bool							m_bTarget_Change;

	bool							m_bSearch;


	_vec3							m_vOriPos;

	_float							m_fSpeed;

	bool							m_bInitInfo;
	bool							m_bTarget_Attack;
	bool							m_bAttackEnd;
	bool							m_bSkillEnd;

	// 보스타입 지정
	bool							m_bBoss_Type;				// Boss == True

	bool							m_bSummon_type;				// false == 일반
	// true  == 소환물


	// 공격 스타일 지정
	bool							m_AttackStyle;			// false 근거리   true 원거리

	// 상태 지정
	bool							m_bAttack;

	bool							m_bRelocation;
	bool							m_bDMG_Timing;

	bool							m_bHitState;
	bool							m_bSkillState;
	bool							m_bDownState;
	bool							m_bStunState;
	bool							m_bDieState;

	bool							m_bDieStart;

	// 스킬 상태
	bool							m_bCall_Skill_State;
	bool							m_bMonster_Wake;			// 소환물 상태
	// 장판 스킬 범위 이펙트
	bool							m_bOnSkill_Range;

	_uint							m_iAniIdx;
	_uint							m_iPrevAniIdx;
	_uint							m_iEnforce_Sk_Idx;

	float							m_fOriAngle;

	_float							m_fCautionRange;			// 경계범위
	_float							m_fCollisionRange;			// 충돌범위

	_float							m_fFrameTime;
	_float							m_fStart_Chase;		
	_float							m_fAtt_DelayTime;
	_float							m_fNext_DelayTime;

	_float							m_fAttackDist;

	_float							m_fGetHit_DMG;

	_float							m_fDie_VanishTime;
	_float							m_fFrameSpeedCtr;

	Engine::ATTPOSITION*			m_pAttPosition;
	Engine::CStatus::DAMAGETYPE     m_eDamageType;

	PLACT_STATE						m_ePlayer_ACT_Type;


	// 스킬 일반 몬스터 쿨타임
	float							m_fSkillTime;
	int								m_iCoolTime_FullIndex;
public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};

};


#endif // Monsters_h__
