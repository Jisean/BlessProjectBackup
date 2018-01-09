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

#ifndef Wyverns_h__
#define Wyverns_h__

#include "NPCs.h"

namespace Engine
{
	class CDynamicMesh;
}

class CPlayers;
class CCalculator;
class CWyverns
	: public CNPCs
{
public:
	enum WYVERNANI {WY_IDLE, 
		WY_RUN_F,
		WY_RUN_B,
		WY_IDLE_ROT_L_LOOP, 
		WY_IDLE_ROT_R_LOOP, 
		WY_RUN_JUMP_UP, 
		WY_RUN_JUMP_LOOP, 
		WY_JUMP_LAND, 
		WY_FLYING_B, 
		WY_FLYING_F_FLUTTER, 
		WY_FLYING_IDLE ,
		WY_END};
	//////////////////////////////////////////////////////////////////////////  와이번 상태값들
protected:
	explicit CWyverns(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWyverns(void);
public:
	virtual HRESULT Initialize(void){return S_OK; }
	virtual _int Update(const _float& fTimeDelta) {return 0;}
	virtual void Render(void) {}
	virtual void UpdateAnimation(){};
	virtual HRESULT AddComponent(void) {return S_OK; }
	virtual void Free(void);
protected:
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
	_vec3		m_vPointDir;
	_bool		m_bPoint;
	float		m_fSoundTime;
protected:
	const _matrix*	m_pmatSaddle;
	Engine::CTransform* m_pPlayerInfo;
protected:
	CPlayers*	m_pPlayer;
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
	bool	  Get_bPoint(){return m_bPoint;}
	void	  Set_Point_Dir(_vec3 vPoint_Dir){m_vPointDir = vPoint_Dir;}
	void	  Set_bPoint(bool bPoint){m_bPoint = bPoint;}
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
	void Set_TargetPlayer(CPlayers* pPlayers);
	void Set_WyvernInfo(D3DXVECTOR3 vPos, float fYAngle);
};


#endif // Wyverns_h__
