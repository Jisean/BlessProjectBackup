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

#ifndef Magician_h__
#define Magician_h__

#include "Players.h"

class CMainPlayer;
class CSphere;
class CCalculator;
class CMagician
	: public CPlayers
{

private:
	explicit CMagician(LPDIRECT3DDEVICE9 pDevice,PLAYERJOB eTypeJob);
	virtual ~CMagician(void);
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
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
	virtual void UpdateAnimation(TRACKID eTrack ,float fTrackTime);
private:
	void	Movement_NonTarget();
	void	Movement_Target();
	void	Movement_Wyvern(CMainPlayer* pMainPlayer);
public:
	//////////////////////////////////////////////////////////////////////////AI 상태
	virtual void AI_Stand();
	virtual	void AI_Patrol();
	virtual	void AI_Combat();
	virtual	void AI_Move();
	virtual	void AI_State();
	virtual	void UpdateState_AI();
	void		 ConsoleDebug();
	void		 Thunderbolt();
	void		 Meteo();
	void		 Starfall();
private:
	bool		m_bPush;
	CSphere*	m_pSphere;
	bool		m_bConstance;
	//웅스 추가

	_float		m_fSkillFour_Time;
	_float		m_fSkillOne_Time;
	_float		m_fSkillFive_Time;
	bool		m_bFirst_Atk;
	bool		m_bSecond_Atk;
	bool		m_bThunderBolt;
	bool		m_bMeteo;
	bool		m_bStarfall;
	_vec3		m_vTargetPos;
	_int		m_iSkillFour_Cnt;
	_int		m_iSkillFive_Cnt;
	CMainPlayer* m_pMainPlayer;
	bool		m_bStop;
public:
	bool	Get_First_Atk();
	bool	Get_Second_Atk();
	void	Set_First_Atk(bool bCheck){m_bFirst_Atk = bCheck;}
	void	Set_Second_Atk(bool bCheck){m_bSecond_Atk = bCheck;}
private:
	virtual HRESULT AddComponent(void);
	//void KeyCheck(const _float& fTimeDelta);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CMagician* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
public:
	////////////////////////////////////////////////////////////////////////// GET
public:
	////////////////////////////////////////////////////////////////////////// SET
};


#endif // Magician_h__
