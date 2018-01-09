/*!
 * \file NPCs.h
 * \date 2016/05/20 14:59
 *
 * \author 권오훈
 * Contact: user@company.com
 *
 * \brief NPC 부모
 *
 * TODO: long description
 *
 * \note
*/

#ifndef NPCs_h__
#define NPCs_h__

#include "Units.h"

class CPlayers;

class CNPCs : public CUnits
{
protected:
	explicit CNPCs(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CNPCs(void);

public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};
public:
	bool							m_bNPC_Collision;		// stage2 NPC  충돌처리용 bool 변수
	bool							m_bStage2;								
protected:
	_float							m_fFrameSpeedCtr;
	_float							m_fGetHit_DMG;

	bool							m_bDieStart;
	bool							m_bDieState;
	
	_float							m_fDie_VanishTime;

	DWORD							m_dwMyIndex;


	DWORD							m_dwIndex;
	CUnits*							m_pTargetInfo;

	float							m_fTimeDelta;
	float							m_fSlowTime;
	float							m_fSpeed;
	float							m_fOriAngle;

	_float							m_fCautionRange;			// 경계범위
	_float							m_fCollisionRange;			// 충돌범위

	_float							m_fStart_Chase;		
	_float							m_fAtt_DelayTime;
	
	_float							m_fAttackDist;


	_uint							m_iAniIdx;
	_uint							m_iPrevAniIdx;

	_vec3							m_vOriPos;
	
	bool							m_bIninInfo;
	// 공격 스타일 지정
	bool							m_AttackStyle;			// false 근거리   true 원거리

	bool							m_bTarget_Attack;
	bool							m_bAttack;
	bool							m_bAttackEnd;

	bool							m_bRelocation;

	Engine::ATTPOSITION*			m_pAttPosition;


	bool							m_bDungeonNPC;			// false 면 stage1   급조된 방식임

	CPlayers*						m_pMainPlayer;
public:
	//========Get========//
	bool		Get_bNPC_Collision(void){ return m_bNPC_Collision; }; 
	bool		Get_bDieState(void) { return m_bDieState; };

	CPlayers*	Get_MainPlayer(void) { return m_pMainPlayer; };
	bool		Get_bDungeonNPC(void) { return m_bDungeonNPC; };

	DWORD		Get_DwIndex(void) { return m_dwIndex; };

	CCollision*  Get_collsion(void) { return m_pCollision; };
	CUnits*		Get_TargetInfo(void) { return m_pTargetInfo; };

	float		Get_Speed();
	_uint		Get_AniIdx(void) { return m_iAniIdx; };
	_uint		Get_PrevAniIdx(void) { return m_iPrevAniIdx; };
	float		Get_OriAngle(void) { return m_fOriAngle; };

	_float		Get_CautionRange(void) { return m_fCautionRange; };
	_float		Get_CollisionRange(void) { return m_fCollisionRange; };

	_vec3		Get_OriPos(void) { return	m_vOriPos; };

	bool		Get_Target_Attack(void) { return m_bTarget_Attack; };
	bool		Get_bAttackStyle(void) { return m_AttackStyle; };
	
	bool		Get_bAttack(void) { return m_bAttack; };
	bool		Get_AttackEnd(void) { return m_bAttackEnd; };
	_float		Get_Att_DelayTime(void) { return m_fAtt_DelayTime; };

	_float		Get_Start_Chase(void) { return m_fStart_Chase; };
	_float		Get_AttackDist(void) { return m_fAttackDist; };

	bool		Get_bRelocation(void) { return m_bRelocation; };


	Engine::ATTPOSITION*		Get_AttackPos(void) { return m_pAttPosition; };

	//========Set========//
	void		Set_bDungeonNPC(bool	bType) { m_bDungeonNPC = bType; };

	void		Set_bNPC_Collision(bool	bType) { m_bNPC_Collision = bType; };

	void		Set_bDieStart(bool	bType) { m_bDieStart = bType; };
	void		Set_bDieState(bool bState) { m_bDieState = bState; };

	void		Set_DwIndex(DWORD dIndex) { m_dwIndex = dIndex; };

	void		Set_TargetInfo(CUnits* pTarget) { m_pTargetInfo = pTarget; };

	void		Set_AniIdx(_uint iAniIdx) { m_iAniIdx = iAniIdx; };
	void		Set_PrevAniIdx(_uint iAniIdx) { m_iPrevAniIdx = iAniIdx; };

	void		Set_Target_Attack(bool bTarget) { m_bTarget_Attack = bTarget; };
	void		Set_bAttack(bool bCheck) { m_bAttack = bCheck; };
	void		Set_AttackEnd(bool AttackEnd) { m_bAttackEnd = AttackEnd; };
	void		Set_bRelocation(bool bCheck) { m_bRelocation = bCheck; };

	void		Set_Att_DelayTime(_float fTime) { m_fAtt_DelayTime = fTime; };

	void		Set_Start_Chase(_float fTime) { m_fStart_Chase = fTime; };
	
	
	void		Set_AttackPos(Engine::ATTPOSITION* pInfo) { m_pAttPosition = pInfo; };

	void		Set_MyIndexNPC(DWORD dwIndex) { m_dwMyIndex = dwIndex; };

	//======General======//

	/////////////////////ActionCam
	void	Set_BossCam(bool bBossCam) { m_bBossCam = bBossCam; }
	bool	Get_BossCam(void) { return m_bBossCam; }
	bool	m_bBossCam;
};

#endif // NPCs_h__
