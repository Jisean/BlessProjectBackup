/*!
 * \file Player.h
 * \date 2016/03/08 13:58
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

#ifndef Paladin_h__
#define Paladin_h__

#include "Players.h"

namespace Engine
{
	class CStatus;
}

class CMainPlayer;
class CSphere;
class CCalculator;
class CPaladin
	: public CPlayers
{
private:
	enum eSubPlayer{SUB_GUARDIAN,SUB_RANGER,SUB_MAGICIAN , SUB_END};
private:
	explicit CPaladin(LPDIRECT3DDEVICE9 pDevice,PLAYERJOB eTypeJob);
	virtual ~CPaladin(void);
private:
////////////////////////////////////////////////////////////////////////// 애니메이션 트랙 함수
	void	Ani_Run(float fTrackTime);
	void	Ani_Attack(float fTrackTime);
	void	Ani_Skill_Buff1(float fTrackTime);
	void	Ani_Skill_Buff2(float fTrackTime);
	void	Ani_Skill_Buff3(float fTrackTime);
	void	Ani_Skill_Buff4(float fTrackTime);
	void	Ani_Skill_ATK1(float fTrackTime);
	void	Ani_Skill_Dash(float fTrackTime);
	void	Ani_Skill_Innate(float fTrackTime);
	void	Ani_Jump(float fTrackTime);
	void	Ani_Hit(float fTrackTime);
	void	Ani_Down(float fTrackTime);
	void	Ani_Die(float fTrackTime);
	void	Ani_DeadBody(float fTrackTime);
	void	Ani_Stand(float fTrackTime);
	void	Ani_DMG_B(float fTrackTime);
	void	Ani_DMG_F(float fTrackTime);
	void	Ani_Stun(float fTrackTime);
	void	Ani_Mount(float	fTrackTime);
private:
	void	Movement_NonTarget();
	void	Movement_Target();
	void	Movement_Heals();
	void	Movement_Wyvern(CMainPlayer* pMainPlayer);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	
	////////////////////////////////////////////////////////////////////////// 모든 애니메이션 함수 제어하는 함수
	virtual void UpdateAnimation(TRACKID eTrack ,float fTrackTime);
public:
	////////////////////////////////////////////////////////////////////////// AI 상태
	virtual void AI_Stand();
	virtual	void AI_Patrol();
	virtual	void AI_Combat();
	virtual	void AI_Move();
	virtual	void AI_State();
	virtual	void UpdateState_AI();
	void		 ConsoleDebug();
	void		 ChoiseHeal();
	void		 RangeHeal();
	void		 DefenceBuff();
public:
	bool		m_bDefenceBuff;
public:
	_float	m_fRangeHeal_Time;
	bool	m_bRangeHeal;
private:
	bool		m_bPush;
	CSphere*	m_pSphere;
	CPlayers*	m_pSubPlayer[SUB_END];
	CPlayers*	m_pSubTarGet;
	Engine::CStatus* m_pTargetStatus;
	_myvec		m_vCenterPos;
	_vec3		m_vHealPos;
	_float		m_fSaveAngle;
	_myvec		m_vSaveDir;
	bool		m_bGuardianCol;
	WORD		m_wSubTarGet_Cnt;
	_myvec		m_vSubPos,m_vPrevPos;
	_float		m_fSubAngle;
	_vec3		m_vChoisePos;
	WORD		m_wPrevTarget_Cnt;
	CMainPlayer* m_pMainPlayer;
private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CPaladin* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
public:
	////////////////////////////////////////////////////////////////////////// GET
public:
	////////////////////////////////////////////////////////////////////////// SET
	
};


#endif // Paladin_h__
