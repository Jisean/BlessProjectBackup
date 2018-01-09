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

#ifndef Wyvern_h__
#define Wyvern_h__

#include "NPCs.h"

namespace Engine
{
	class CDynamicMesh;
}

class CCalculator;
class CWyvern
	: public CNPCs
{
public:
	enum WYVERNANI {WY_IDLE, 
		WY_RUN_F,
		WY_RUN_STOP,
		WY_RUN_B,
		WY_IDLE_ROT_R_START, 
		WY_IDLE_ROT_R_LOOP, 
		WY_IDLE_ROT_L_START, 
		WY_IDLE_ROT_L_LOOP, 
		WY_RUN_JUMP_UP, 
		WY_RUN_JUMP_LOOP, 
		WY_JUMP_LAND, 
		WY_FLYING_B, 
		WY_FLYING_F_FLUTTER, 
		WY_FLYING_IDLE ,
		WY_END};		
	//////////////////////////////////////////////////////////////////////////  와이번 상태값들
private:
	explicit CWyvern(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWyvern(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
	virtual void UpdateAnimation(WYVERNANI eTrack ,float fTrackTime);
public:
	////////////////////////////////////////////////////////////////////////// 와이번 애니메이션 상태 함수
	void	Ani_Jump_To_Wyvern(float fTrackTime);
	void	Ani_Move_To_Wyvern(float fTrackTime);
	void	Ani_Fly_To_Wyvern(float fTrackTime);
	void	Ani_Stand_To_Wyvern(float fTrackTime);
private:
	Engine::CDynamicMesh* m_pWyvernMesh;
	bool		m_bPush;
	bool		m_bAct_Mount;
	WYVERNANI	m_eWYTrack;
	bool		m_bJump_Wyvern;
	bool		m_bFly;
	float		m_fJumpAcc;
	float		m_fAcc , m_fAccSlow;
	bool		m_bAxel, m_bDecelerate;
	bool		m_bDown;
	const _matrix*	m_pmatSaddle;
	Engine::CTransform* m_pPlayerInfo;
private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CWyvern* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
public:
	////////////////////////////////////////////////////////////////////////// GET
	Engine::CDynamicMesh* Get_Mesh();
	WYVERNANI Get_TrackID();
	bool	  Get_ActMount();
	bool	  Get_Jump();
	bool	  Get_Fly();
	bool	  Get_Axel();
	bool	  Get_Decelerate();
	float	  Get_fAcc();
	bool	  Get_Down();
	float	  Get_JumpAcc();
public:
	////////////////////////////////////////////////////////////////////////// SET
	void Set_TrackID(WYVERNANI eTrack);
	void Set_ActMount(bool bCheck);
	void Set_Jump(bool bCheck);
	void Set_Fly(bool bCheck);
	void Set_JumpAcc_Init(float fJumpAcc);
	void Set_Axel(bool bAxel);
	void Set_Decelerate(bool bDecelerate);
	void Set_Down(bool bCheck);
	void Set_JumpAcc(float fAcc);
};


#endif // Wyvern_h__
