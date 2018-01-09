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

#ifndef Guardian_h__
#define Guardian_h__

#include "Players.h"

class CMainPlayer;
class CSphere;
class CCalculator;
class CGuardian
	: public CPlayers
{
private:
	explicit CGuardian(LPDIRECT3DDEVICE9 pDevice,PLAYERJOB eTypeJob);
	virtual ~CGuardian(void);
private:
	void	Ani_Run(float fTrackTime);
	void	Ani_Attack(float fTrackTime);
	void	Ani_Skill_01(float fTrackTime);
	void	Ani_Skill_02(float fTrackTime);
	void	Ani_Skill_03(float fTrackTime);
	void	Ani_Skill_04(float fTrackTime);
	void	Ani_Skill_05(float fTrackTime);
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
	void	Ani_Constance(float fTrackTime);
private:
	void	Movement_NonTarget();
	void	Movement_Target();
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
	virtual void UpdateAnimation(TRACKID eTrack ,float fTrackTime);
public:
	////////////////////////////////////////////////////////////////////////// AI ป๓ลย
	virtual void AI_Stand();
	virtual	void AI_Patrol();
	virtual	void AI_Combat();
	virtual	void AI_Move();
	virtual	void AI_State();
	virtual	void UpdateState_AI();
	void		 ConsoleDebug();
private:
	bool		m_bPush;
	CSphere*	m_pSphere;
	char		m_szNameKey[20];
	float		m_fDashAngle;
	int			m_iFirstNull_Check;
	bool		m_bTemp;
	_float		m_fBossCamTime;
	_bool		m_bBossCamSK1;
	_bool		m_bBossCamSK2;
	CMainPlayer*	m_pMainPlayer;
private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
	void ActionCamMove(_float fTimeDelta);
public:
	static CGuardian* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
public:
	////////////////////////////////////////////////////////////////////////// GET
public:
	////////////////////////////////////////////////////////////////////////// SET
};


#endif // Guardian_h__
