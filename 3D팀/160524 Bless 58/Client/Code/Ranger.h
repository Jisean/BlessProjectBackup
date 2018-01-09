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

#ifndef Ranger_h__
#define Ranger_h__

#include "Players.h"

class CMainPlayer;
class CSphere;
class CCalculator;
class CRanger
	: public CPlayers
{

private:
	explicit CRanger(LPDIRECT3DDEVICE9 pDevice,PLAYERJOB eTypeJob);
	virtual ~CRanger(void);
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
	void	Movement_Wyvern(CMainPlayer* pMainPlayer);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
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
private:
	bool		m_bPush;
	bool		m_bWyvern_constant;
	float		fTime;
	CSphere*	m_pSphere;
	bool		m_bConstance;
	float		m_fTimeSecond;
	_float		m_fSaveAngle;
	_int		m_iArrowCnt;
	float		m_fShotSecond;
	float		m_fSkill1_Second;
	//웅스 추가
	float		m_fSkill3Angle;
	CMainPlayer* m_pMainPlayer;
	bool		m_bStop;
private:
	const _matrix* m_pmatHand;
	const _matrix* m_pmatSaddle;	// 안장 BORN 행렬
private:
	virtual HRESULT AddComponent(void);
	//void KeyCheck(const _float& fTimeDelta);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CRanger* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
public:
	////////////////////////////////////////////////////////////////////////// GET
public:
	////////////////////////////////////////////////////////////////////////// SET
};


#endif // Ranger_h__
