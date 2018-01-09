#include "stdafx.h"
#include "Wyverns.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Calculator.h"
#include "Material.h"
#include "Status.h"
#include "Stage.h"
#include "Players.h"

CWyverns::CWyverns(LPDIRECT3DDEVICE9 pDevice)
: CNPCs(pDevice)
, m_bPush(false)
, m_pWyvernMesh(NULL)
, m_bAct_Mount(false)
, m_eWYTrack(WY_IDLE)
, m_bJump_Wyvern(false)
, m_fJumpAcc(0.f)
, m_bFly(false)
, m_fAcc(0.f)
, m_fAccSlow(0.f)
, m_bAxel(false)
, m_bDecelerate(false)
, m_pmatSaddle(NULL)
, m_pPlayerInfo(NULL)
, m_bDown(false)
, m_pPlayer(NULL)
, m_vPointDir(_vec3(0,0,0))
, m_bPoint(false)
, m_fSoundTime(0.f)
{

}
CWyverns::~CWyverns(void)
{

}

void CWyverns::Free(void)
{
	CUnits::Free();
	Engine::CGameObject::Free();
}

////////////////////////////////////////////////////////////////////////// GET
Engine::CDynamicMesh* CWyverns::Get_Mesh()
{
	return m_pWyvernMesh;
}

CWyverns::WYVERNANI CWyverns::Get_TrackID()
{
	return m_eWYTrack;
}

bool CWyverns::Get_ActMount()
{
	return m_bAct_Mount;
}

bool CWyverns::Get_Jump()
{
	return m_bJump_Wyvern;
}

bool CWyverns::Get_Fly()
{
	return m_bFly;
}

bool CWyverns::Get_Axel()
{
	return m_bAxel;
}

bool CWyverns::Get_Decelerate()
{
	return m_bDecelerate;
}

float CWyverns::Get_fAcc()
{
	return m_fAcc;
}

bool CWyverns::Get_Down()
{
	return m_bDown;
}

float CWyverns::Get_JumpAcc()
{
	return m_fJumpAcc;
}

////////////////////////////////////////////////////////////////////////// SET
void CWyverns::Set_TrackID(WYVERNANI eTrack)
{
	m_eWYTrack = eTrack;
}

void CWyverns::Set_ActMount(bool bCheck)
{
	m_bAct_Mount = bCheck;
}

void CWyverns::Set_Jump(bool bCheck)
{
	m_bJump_Wyvern = bCheck;
}

void CWyverns::Set_Fly(bool bCheck)
{
	m_bFly = bCheck;
}

void CWyverns::Set_JumpAcc_Init(float fJumpAcc)
{
	m_fJumpAcc = fJumpAcc;
}

void CWyverns::Set_Axel(bool bAxel)
{
	m_bAxel = bAxel;
}

void CWyverns::Set_Decelerate(bool bDecelerate)
{
	m_bDecelerate = bDecelerate;
}

void CWyverns::Set_Down(bool bCheck)
{
	m_bDown = bCheck;
}

void CWyverns::Set_JumpAcc(float fAcc)
{
	m_fJumpAcc = fAcc;
}

void CWyverns::Set_TargetPlayer(CPlayers* pPlayers)
{
	m_pPlayer = pPlayers;
}

void CWyverns::Set_WyvernInfo(D3DXVECTOR3 vPos, float fYAngle)
{
	m_pInfo->m_vPos = vPos;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = fYAngle;
	m_pInfo->Update();
}
