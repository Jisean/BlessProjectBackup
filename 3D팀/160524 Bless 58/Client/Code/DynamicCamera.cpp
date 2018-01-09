#include "stdafx.h"
#include "DynamicCamera.h"

#include "Include.h"
#include "Export_Function.h"
#include "KeyMgr.h"
#include "Stage.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "MainPlayer.h"
#include "WyvernBrown.h"
#include "Ghost_Knight.h"
#include "Players.h"
#include "Soldier_1.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_fCamSpeed(0.f)
, m_bClick(false)
, m_bMouseFix(true)
, m_eCamState(CAM_NORMAL)
, m_pPlayerInfo(NULL)
, m_pPlayerMesh(NULL)
, m_fDistance(0.f)
, m_vPreEye(0.f, 0.f, 0.f)
, m_vPreAt(0.f, 0.f, 0.f)
, m_bLengthStart(true)
, m_fLengthStart(0.f)
, m_pMainPlayer(NULL)
, m_pWyvernInfo(NULL)
, m_pWyvernMesh(NULL)
, m_ePrevState(CAM_DEFAULT)
, m_bChange(false)
, m_fAngle_Yaw(0.f)
, m_fSaveAngle(0.f)
, m_bInterpol(false)
, m_bFree(false)
, m_fSeconds(0.f)
, m_bBossCamStart(true)
, m_fBossCamTime(0.f)
{
	m_bActionCamSet.reset();
	m_bBossCamSeq.reset();
	ZeroMemory(m_vBossCamAtSet, sizeof(_myvec) * 8);
	ZeroMemory(m_vBossCamEyeSet, sizeof(_myvec) * 8);
}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed = 50.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();

	m_fDistance = 3.f;
	m_fAngle_Pitch = -300.f;

	return S_OK;
}

HRESULT CDynamicCamera::RenewalCamera(void)
{
	m_bActionCamSet.set(ACT_CHARCHANGE);

	m_vPreEye = m_vEye;
	m_vPreAt = m_vAt;
	m_pPlayerInfo = (Engine::CTransform*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");
	m_pPlayerMesh = (Engine::CDynamicMesh*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Mesh");
	m_pWyvernMesh = m_pMainPlayer->Get_TargetPlayer()->Get_Wyvern()->Get_Mesh();
	m_pWyvernInfo = ((Engine::CTransform*)m_pMainPlayer->Get_TargetPlayer()->Get_Wyvern()->GetComponent(L"Transform"));

	return S_OK;
}

void CDynamicCamera::KeyCheck(const _float& fTimeDelta)
{
	float		fTime = fTimeDelta;

	if(Engine::GetDIKeyState(DIK_LEFT) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fCamSpeed / 3.f * fTime;
		m_vAt -= vRight * m_fCamSpeed / 3.f * fTime;
	}

	if(Engine::GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fCamSpeed/ 3.f * fTime;
		m_vAt += vRight * m_fCamSpeed / 3.f* fTime;
	}

	if(Engine::GetDIKeyState(DIK_UP) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fCamSpeed/ 3.f * fTime;
		m_vAt += vLook * m_fCamSpeed/ 3.f * fTime;
	}

	if(Engine::GetDIKeyState(DIK_DOWN) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fCamSpeed / 3.f* fTime;
		m_vAt -= vLook * m_fCamSpeed/ 3.f * fTime;
	}
}

_int CDynamicCamera::Update(const _float& fTimeDelta)
{
	if(CKeyMgr::GetInstance()->GetKeyDown(VK_NUMPAD6))
	{
		m_bActionCamSet.set(ACT_BOSSROOM);
		CGhost_Knight* pBoss = (CGhost_Knight*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ghost_Knight")->front();
		pBoss->Set_BossCam(true);
	}
	if(CKeyMgr::GetInstance()->GetKeyDown('M'))
	{
		m_bBossCamSeq.reset();
		m_bActionCamSet.reset(ACT_BOSSROOM);
		RenewalCamera();
		CGhost_Knight* pBoss = (CGhost_Knight*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ghost_Knight")->front();
		pBoss->Set_BossCam(false);
		CPlayers* pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Guardian")->front();
		pPlayer->Set_BossCam(false);
		pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Paladin")->front();
		pPlayer->Set_BossCam(false);
		pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Magician")->front();
		pPlayer->Set_BossCam(false);
		pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ranger")->front();
		pPlayer->Set_BossCam(false);
		/*CSoldier_1* pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_0")->front();
		pNPC->Set_BossCam(false);
		pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_1")->front();
		pNPC->Set_BossCam(false);
		pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_2")->front();
		pNPC->Set_BossCam(false);
		pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_3")->front();
		pNPC->Set_BossCam(false);
		pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_4")->front();
		pNPC->Set_BossCam(false);*/
	}

	Engine::CCamera::Update(fTimeDelta);

	m_fTimeDelta = fTimeDelta;

	if(m_bStart)
	{
		m_pPlayerInfo = (Engine::CTransform*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");
		m_pPlayerMesh = (Engine::CDynamicMesh*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Mesh");
		m_bStart = false;
	}

	if(NULL == m_pMainPlayer)
		m_pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());

	else
	{
		if(NULL == m_pWyvernMesh || NULL == m_pWyvernInfo)
		{
			m_pWyvernMesh = m_pMainPlayer->Get_TargetPlayer()->Get_Wyvern()->Get_Mesh();
			m_pWyvernInfo = ((Engine::CTransform*)m_pMainPlayer->Get_TargetPlayer()->Get_Wyvern()->GetComponent(L"Transform"));
		}
		if(m_pMainPlayer->Get_TargetPlayer()->Test_ActBitSet(CPlayers::ACT_MOUNT))
		{
			if(m_pMainPlayer->Get_TargetPlayer()->Get_Wyvern()->Get_TrackID() == CWyvernBrown::WY_RUN_JUMP_LOOP)
				m_eCamState = CAM_WYVERN_STATIC;

			else
			{
				if(Engine::GetDIMouseState(Engine::CInput::DIM_RBUTTON) & 0x80)
				{
					m_eCamState = CAM_WYVERN_STATIC;
					m_bMouseFix = true;
				}
				if(CKeyMgr::GetInstance()->GetKeyUp(VK_RBUTTON))
				{
					m_fAngle_Yaw = 0.f;
					m_eCamState = CAM_WYVERN_DYNAMIC;
					m_bMouseFix = true;
				}
			}
		}
		else
		{
			if(m_eCamState == CAM_WYVERN_DYNAMIC ||
				m_eCamState == CAM_WYVERN_STATIC)
			{
				m_eCamState = CAM_NORMAL_DYNAMIC;
			}
		}
	}

	if(CKeyMgr::GetInstance()->GetKeyDown('H'))
	{
		if(false == m_bChange)
			m_bChange = true;
		else
			m_bChange = false;

	}
	if(CKeyMgr::GetInstance()->GetKeyDown('P'))
	{
		if(m_bMouseFix)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}

	if(m_eCamState != CAM_WYVERN_DYNAMIC && 
		m_eCamState != CAM_WYVERN_STATIC)
	{
		if(m_bChange)
			m_eCamState = CAM_FREE;
		else
		{
			m_eCamState = CAM_NORMAL;
		}
	}

	if(m_bActionCamSet.any())
		ActionCamera(fTimeDelta);
	else
	{
		switch(m_eCamState)
		{
		case CAM_NORMAL:
			NormalCamera();
			if(m_bMouseFix)
			{
				FixMouse();
				FixedMouseMove();
			}
			m_ePrevState = m_eCamState;
			break;
		case CAM_FREE:
			KeyCheck(fTimeDelta);
			if(m_bMouseFix)
			{
				FixMouse();
				MouseMove();
			}
			break;
		case CAM_WYVERN_STATIC:
			WyvernCamera_Static();
			if(m_bMouseFix)
			{
				FixMouse();
				FixedMouseMove();
			}
			m_ePrevState = m_eCamState;
			break;
		case CAM_WYVERN_DYNAMIC:
			WyvernCamera_Dynamic();
			if(m_bMouseFix)
			{
				FixMouse();
				FixedMouseMove();
			}
			m_ePrevState = m_eCamState;
			break;
		}
	}

#ifdef _DEBUG
	wchar_t	szEyePos[128] = L"";
	wchar_t	szAtPos[128] = L"";
	wsprintf(szEyePos, L"EyePos : %d, %d, %d", m_vEye.x, m_vEye.y, m_vEye.z);
	wsprintf(szAtPos, L"AtPos : %d, %d, %d", m_vAt.x, m_vAt.y, m_vAt.z);
	Engine::Render_Font(L"¸¼Àº°íµñ", _vec3(650.f, 500.f, 0.f), szEyePos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Engine::Render_Font(L"¸¼Àº°íµñ", _vec3(650.f, 550.f, 0.f), szAtPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif

	return Engine::CCamera::Update(fTimeDelta);
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pDevice);
	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CDynamicCamera::FixMouse(void)
{
	POINT		ptMouse = {WINCX >> 1, WINCY >> 1};
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::MouseMove(void)
{

	int		iDistance = 0;
	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}

void CDynamicCamera::NormalCamera(void)
{

	if(true == m_bFree)
	{
		m_fSeconds += m_fTimeDelta;

		if(m_fSeconds >= 0.01f)
		{
			m_fSeconds = 0.f;
			m_bFree = false;
		}
		return;
	}
	D3DXMATRIX matHead = *(m_pPlayerMesh->FindFrame("BN_Head"));

	D3DXMATRIX matScale, matTrans, matRotX, matRotY, matRotZ;
	D3DXMatrixScaling(&matScale, m_pPlayerInfo->m_vScale.x, m_pPlayerInfo->m_vScale.y, m_pPlayerInfo->m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pPlayerInfo->m_vPos.x, m_pPlayerInfo->m_vPos.y, m_pPlayerInfo->m_vPos.z);
	D3DXMatrixRotationX(&matRotX, m_pPlayerInfo->m_fAngle[Engine::ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_pPlayerInfo->m_fAngle[Engine::ANGLE_Z]);

	matHead *= matScale * matRotX * matRotY * matRotZ * matTrans;

	//===================== woong's Camera ==============================

	//static bool bCamera = false;

	//if(bCamera == false)
	//{
	//	bCamera = true;
	//	m_vEye = - (m_pPlayerInfo->m_vDir * 2.f) ;
	//	D3DXVec3Normalize(&m_vEye, &m_vEye);

	//	m_vEye *= m_fDistance;

	//	m_vAt = m_pPlayerInfo->m_vPos;
	//	m_vEye += m_pPlayerInfo->m_vPos;

	//}
	//====================================================================

	/*if(m_pMainPlayer->Get_TargetPlayer()->Test_KeyBitSet(CPlayers::STAKEY_MOVE))
	{*/
		m_vEye = m_pPlayerInfo->m_vDir * (-1);
		D3DXVec3Normalize(&m_vEye, &m_vEye);

		m_vEye *= m_fDistance;
	//}

	D3DXVECTOR3		vRight, vUp;
	D3DXMATRIX		matCamState = m_pPlayerInfo->m_matWorld;
	memcpy(&vRight, &matCamState.m[2][0], sizeof(float) * 3);
	memcpy(&vUp, &matCamState.m[1][0], sizeof(float) * 3);

	D3DXMATRIX		matAxis;
	D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(m_fAngle_Pitch / 10.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matAxis);

	D3DXMatrixRotationAxis(&matAxis, &vUp, m_fAngle_Yaw);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matAxis);

	m_vEye += m_pPlayerInfo->m_vPos;


	if(m_pMainPlayer->Get_TargetPlayer()->Test_KeyBitSet(CPlayers::STAKEY_MOVE) ||
		m_pMainPlayer->Get_TargetPlayer()->Test_KeyBitSet(CPlayers::STAKEY_COMBO))
	{
		if(m_pMainPlayer->Get_TargetPlayer()->Get_TrackID() == CPlayers::DASH ||
			m_pMainPlayer->Get_TargetPlayer()->Get_TrackID() == CPlayers::HIDDEN_SKILL ||
			m_pMainPlayer->Get_TargetPlayer()->Any_SkillBitSet())
			memcpy(&m_vAt, &matHead.m[3][0], sizeof(D3DXVECTOR3));
		else
		{
			m_vAt = m_pPlayerInfo->m_vPos + m_pPlayerInfo->m_vDir * 2.f;
			m_vAt.y = m_pPlayerInfo->m_vPos.y + 0.3f;
		}
		
	}
	else
	{
		if(m_pMainPlayer->Get_TargetPlayer()->Get_TrackID() == CPlayers::STAND_NORMAL)
		{
			m_vAt = m_pPlayerInfo->m_vPos + m_pPlayerInfo->m_vDir * 2.f;
			m_vAt.y = m_pPlayerInfo->m_vPos.y + 0.3f;
		}
		else
			memcpy(&m_vAt, &matHead.m[3][0], sizeof(D3DXVECTOR3));
	}


	
}

void CDynamicCamera::FixedMouseMove(void)
{
	_int iDistance = 0;
	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Z))
	{
		if(m_fDistance - iDistance / 1000.f > 0.5f && m_fDistance - iDistance / 1000.f < 5.5f)
			m_fDistance -= iDistance / 1000.f;
	}

	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		m_fAngle_Pitch -= (float)iDistance;

		if(m_fAngle_Pitch > -90)
			m_fAngle_Pitch = -90;

		if(m_fAngle_Pitch < -816)
			m_fAngle_Pitch = -816;
	}

	iDistance = 0;
	if(m_pMainPlayer->Get_TargetPlayer()->Test_ActBitSet(CPlayers::ACT_MOUNT))
	{
		if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
		{
			if(NULL != m_pWyvernInfo)
			{
				if(m_eCamState == CAM_WYVERN_STATIC && m_bMouseFix == true)
				{
					m_pWyvernInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(iDistance / 10.f) * m_fTimeDelta * 5.f;
				}
				if(m_pMainPlayer->Get_TargetPlayer()->Get_Wyvern()->Get_TrackID() == CWyvernBrown::WY_RUN_JUMP_LOOP)
					m_pWyvernInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(iDistance / 10.f) * m_fTimeDelta * 5.f;

				else
				{
					m_fAngle_Yaw += D3DXToRadian(iDistance / 10.f);

					if(m_fAngle_Yaw > D3DXToRadian(180.f))
						m_fAngle_Yaw = -D3DXToRadian(180.f);

					if(m_fAngle_Yaw < -D3DXToRadian(180.f))
						m_fAngle_Yaw = D3DXToRadian(180.f);
				}
			}
		}
	}
	else
	{
		if(m_pMainPlayer->Get_TargetPlayer()->Test_KeyBitSet(CPlayers::STAKEY_MOVE) &&
			!m_pMainPlayer->Get_TargetPlayer()->Any_SkillBitSet())
		{
	

				if(m_fAngle_Yaw != 0.f && false == m_bInterpol)
				{
					if(m_fAngle_Yaw > D3DXToRadian(10.f) && m_fAngle_Yaw <= D3DXToRadian(180.f))
						m_fAngle_Yaw -= D3DXToRadian(10.f);

					else if(m_fAngle_Yaw < -D3DXToRadian(10.f) && m_fAngle_Yaw >= -D3DXToRadian(180.f))
						m_fAngle_Yaw += D3DXToRadian(10.f);
					else					
					{
						m_fAngle_Yaw = 0.f;
						m_bInterpol = true;		// º¸°£ ³¡³µ´Ù
					}
				}
			
		}
		if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
		{
			if(m_pMainPlayer->Get_TargetPlayer()->Test_KeyBitSet(CPlayers::STAKEY_MOVE) &&
				!m_pMainPlayer->Get_TargetPlayer()->Any_SkillBitSet())
			{
					m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(iDistance / 10.f);
					
					_float fTemp = m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y];

					if(m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] > D3DXToRadian(180.f))
					{
						m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] = -D3DXToRadian(180.f);
						if(!_finite(m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y]))
							m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
					}
					if(m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] < -D3DXToRadian(180.f))
					{
						m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(180.f);
						if(!_finite(m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y]))
							m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
					}
					if(m_bInterpol == true)
					{
						//m_fAngle_Yaw = m_fSaveAngle; // ÀÌ°ÅÇÏ¸é ¾ÈµÇ ¾¾¹ß
					}
				
				//m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] = m_fSaveAngle;
			}
			else
			{	
				////======

				/*D3DXMATRIX		matAxis;
				D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance / 10.f));

				D3DXVECTOR3		vDir;
				vDir = m_vEye - m_pPlayerInfo->m_vPos;
				D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

				m_vEye = m_pPlayerInfo->m_vPos + vDir;*/

				/*D3DXVECTOR3		vRight, vUp;
				D3DXMATRIX		matCamState = m_pPlayerInfo->m_matWorld;
				memcpy(&vRight, &matCamState.m[2][0], sizeof(float) * 3);
				memcpy(&vUp, &matCamState.m[1][0], sizeof(float) * 3);

				D3DXMATRIX		matAxis;			
				
				D3DXMatrixRotationAxis(&matAxis, &vUp, D3DXToRadian(iDistance / 10.f));
				D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matAxis);
				m_vEye = */
				//========
				m_fAngle_Yaw += D3DXToRadian(iDistance / 10.f);

				if(m_fAngle_Yaw > D3DXToRadian(180.f))
					m_fAngle_Yaw = -D3DXToRadian(180.f);

				if(m_fAngle_Yaw < -D3DXToRadian(180.f))
					m_fAngle_Yaw = D3DXToRadian(180.f);

			}

		}	
	}
}
void CDynamicCamera::ActionCamera(const _float& fTimeDelta)
{
	if(m_bActionCamSet.test(ACT_CHARCHANGE))
	{
		D3DXMATRIX matHead = *(m_pPlayerMesh->FindFrame("BN_Head"));

		D3DXMATRIX matScale, matTrans, matRotX, matRotY, matRotZ;
		D3DXMatrixScaling(&matScale, m_pPlayerInfo->m_vScale.x, m_pPlayerInfo->m_vScale.y, m_pPlayerInfo->m_vScale.z);
		D3DXMatrixTranslation(&matTrans, m_pPlayerInfo->m_vPos.x, m_pPlayerInfo->m_vPos.y, m_pPlayerInfo->m_vPos.z);
		D3DXMatrixRotationX(&matRotX, m_pPlayerInfo->m_fAngle[Engine::ANGLE_X]);
		D3DXMatrixRotationY(&matRotY, m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y]);
		D3DXMatrixRotationZ(&matRotZ, m_pPlayerInfo->m_fAngle[Engine::ANGLE_Z]);

		matHead *= matScale * matRotX * matRotY * matRotZ * matTrans;

		_myvec vEye = _myvec(0.f, 0.f, 0.f);
		_myvec vAt = _myvec(0.f, 0.f, 0.f);
		vEye = m_pPlayerInfo->m_vDir * (-1);
		vEye.Normalize();
		vEye *= m_fDistance;

		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState = m_pPlayerInfo->m_matWorld;
		memcpy(&vRight, &matCamState.m[2][0], sizeof(float) * 3);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(m_fAngle_Pitch / 10.f));
		vEye.TransformNormal(&vEye, &matAxis);

		vEye += m_pPlayerInfo->m_vPos;

		memcpy(&vAt, &matHead.m[3][0], sizeof(D3DXVECTOR3));

		_myvec vEyeLen = _myvec(0.f, 0.f, 0.f);
		_myvec vAtLen = _myvec(0.f, 0.f, 0.f);
		vEyeLen = vEye - m_vPreEye;
		vAtLen	= vAt - m_vPreAt;

		_myvec vMoveLen = m_vEye - vEye;
		_float fMoveLen = vMoveLen.Length();

		m_vEye += vEyeLen * fTimeDelta;
		m_vAt += vAtLen * fTimeDelta;

		//if(m_bLengthStart)
		//{
		//	m_fLengthStart = fMoveLen * 0.1f;
		//	m_bLengthStart = false;
		//}
		if(fMoveLen < 3.f	/*m_fLengthStart*/)
		{
			m_bLengthStart = true;
			m_bActionCamSet.reset(ACT_CHARCHANGE);
		}
	}
	else if(m_bActionCamSet.test(ACT_BOSSROOM))
	{
		m_fBossCamTime += fTimeDelta;
		_float fCamSpeed = 1.f;
		if(m_bBossCamStart)
		{
			m_vBossCamAtSet[0] = _myvec(110.854f,	2.3345f,	154.979f);
			m_vBossCamAtSet[1] = _myvec(93.0249f,	3.64643f,	155.239f);
			m_vBossCamAtSet[2] = _myvec(91.8829f,	3.46863f,	156.662f);
			m_vBossCamAtSet[3] = _myvec(89.5728f,	3.56896f,	159.611f);
			m_vBossCamAtSet[4] = _myvec(86.9792f,	3.04957f,	157.642f);
			m_vBossCamAtSet[5] = _myvec(86.7621f,	2.77996f,	155.143f);
			m_vBossCamAtSet[6] = _myvec(88.9202f,	2.67996f,	153.381f);
			m_vBossCamAtSet[7] = _myvec(88.9202f,	2.52021f,	153.381f);

			m_vBossCamEyeSet[0] = _myvec(108.061f,	2.68484f,	155.024f);
			m_vBossCamEyeSet[1] = _myvec(95.6706f,	5.07043f,	155.357f);
			m_vBossCamEyeSet[2] = _myvec(93.1171f,	4.37885f,	157.799f);
			m_vBossCamEyeSet[3] = _myvec(89.6217f,	4.23366f,	161.175f);
			m_vBossCamEyeSet[4] = _myvec(84.0599f,	3.25998f,	159.465f);
			m_vBossCamEyeSet[5] = _myvec(83.5274f,	3.03711f,	155.156f);
			m_vBossCamEyeSet[6] = _myvec(85.1016f,	2.73575f,	150.75f);
			m_vBossCamEyeSet[7] = _myvec(89.0217f,	2.70258f,	149.87f);

			m_fBossCamSpeed[0]		= 0.15f;
			m_fBossCamSpeed[1]		= 0.5f;
			m_fBossCamSpeed[2]		= 0.5f;
			m_fBossCamSpeed[3]		= 0.6f;
			m_fBossCamSpeed[4]		= 0.4f;
			m_fBossCamSpeed[5]		= 0.5f;
			m_fBossCamSpeed[6]		= 0.4f;
			m_fBossCamSpeed[7]		= 0.f;

			m_bBossCamStart = false;
			CGhost_Knight* pBoss = (CGhost_Knight*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ghost_Knight")->front();
			pBoss->Set_BossCam(true);
			CPlayers* pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Guardian")->front();
			pPlayer->Set_BossCam(true);
			pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Paladin")->front();
			pPlayer->Set_BossCam(true);
			pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Magician")->front();
			pPlayer->Set_BossCam(true);
			pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ranger")->front();
			pPlayer->Set_BossCam(true);
			CSoldier_1* pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_0")->front();
			pNPC->Set_BossCam(true);
			pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_1")->front();
			pNPC->Set_BossCam(true);
			pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_2")->front();
			pNPC->Set_BossCam(true);
			pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_3")->front();
			pNPC->Set_BossCam(true);
			pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_4")->front();
			pNPC->Set_BossCam(true);

			m_fBossCamTime = 0.f;
		}
		if(!m_bBossCamSeq.any())
		{
			m_vAt = m_vBossCamAtSet[0];
			m_vEye = m_vBossCamEyeSet[0];
			m_bBossCamSeq.set(0);
		}
		else
		{
			_myvec vLengthAt, vLengthEye, vLengthMove;
			//_float fLengthMove;

			if(!m_bBossCamSeq.test(1) && m_bBossCamSeq.test(0))
			{
				fCamSpeed = m_fBossCamSpeed[0];
				vLengthAt = m_vBossCamAtSet[1] - m_vBossCamAtSet[0];
				vLengthEye = m_vBossCamEyeSet[1] - m_vBossCamEyeSet[0];
				//vLengthMove = m_vBossCamEyeSet[1] - m_vEye;
				//fLengthMove = vLengthMove.Length();

				m_vAt += vLengthAt * fTimeDelta * fCamSpeed;
				m_vEye += vLengthEye * fTimeDelta * fCamSpeed;

				if(/*fLengthMove < 0.2f*/m_fBossCamTime > 7.f)
					m_bBossCamSeq.set(1);
			}
			else if(!m_bBossCamSeq.test(2) && m_bBossCamSeq.test(1))
			{
				fCamSpeed = m_fBossCamSpeed[1];
				vLengthAt = m_vBossCamAtSet[2] - m_vBossCamAtSet[1];
				vLengthEye = m_vBossCamEyeSet[2] - m_vBossCamEyeSet[1];
				//vLengthMove = m_vBossCamEyeSet[2] - m_vEye;
				//fLengthMove = vLengthMove.Length();

				m_vAt += vLengthAt * fTimeDelta * fCamSpeed;
				m_vEye += vLengthEye * fTimeDelta * fCamSpeed;

				if(/*fLengthMove < 0.2f*/m_fBossCamTime > 9.f)
					m_bBossCamSeq.set(2);
			}
			else if(!m_bBossCamSeq.test(3) && m_bBossCamSeq.test(2))
			{
				fCamSpeed = m_fBossCamSpeed[2];
				vLengthAt = m_vBossCamAtSet[3] - m_vBossCamAtSet[2];
				vLengthEye = m_vBossCamEyeSet[3] - m_vBossCamEyeSet[2];
				//vLengthMove = m_vBossCamEyeSet[3] - m_vEye;
				//fLengthMove = vLengthMove.Length();

				m_vAt += vLengthAt * fTimeDelta * fCamSpeed;
				m_vEye += vLengthEye * fTimeDelta * fCamSpeed;

				if(/*fLengthMove < 0.2f*/m_fBossCamTime > 11.f)
					m_bBossCamSeq.set(3);
			}
			else if(!m_bBossCamSeq.test(4) && m_bBossCamSeq.test(3))
			{
				fCamSpeed = m_fBossCamSpeed[3];
				vLengthAt = m_vBossCamAtSet[4] - m_vBossCamAtSet[3];
				vLengthEye = m_vBossCamEyeSet[4] - m_vBossCamEyeSet[3];
				//vLengthMove = m_vBossCamEyeSet[4] - m_vEye;
				//fLengthMove = vLengthMove.Length();

				m_vAt += vLengthAt * fTimeDelta * fCamSpeed;
				m_vEye += vLengthEye * fTimeDelta * fCamSpeed;

				if(/*fLengthMove < 0.2f*/m_fBossCamTime > 13.f)
					m_bBossCamSeq.set(4);
			}
			else if(!m_bBossCamSeq.test(5) && m_bBossCamSeq.test(4))
			{
				fCamSpeed = m_fBossCamSpeed[4];
				vLengthAt = m_vBossCamAtSet[5] - m_vBossCamAtSet[4];
				vLengthEye = m_vBossCamEyeSet[5] - m_vBossCamEyeSet[4];
				//vLengthMove = m_vBossCamEyeSet[5] - m_vEye;
				//fLengthMove = vLengthMove.Length();

				m_vAt += vLengthAt * fTimeDelta * fCamSpeed;
				m_vEye += vLengthEye * fTimeDelta * fCamSpeed;

				if(/*fLengthMove < 0.2f*/m_fBossCamTime > 15.f)
					m_bBossCamSeq.set(5);
			}
			else if(!m_bBossCamSeq.test(6) && m_bBossCamSeq.test(5))
			{
				fCamSpeed = m_fBossCamSpeed[5];
				vLengthAt = m_vBossCamAtSet[6] - m_vBossCamAtSet[5];
				vLengthEye = m_vBossCamEyeSet[6] - m_vBossCamEyeSet[5];
				//vLengthMove = m_vBossCamEyeSet[6] - m_vEye;
				//fLengthMove = vLengthMove.Length();

				m_vAt += vLengthAt * fTimeDelta * fCamSpeed;
				m_vEye += vLengthEye * fTimeDelta * fCamSpeed;

				if(/*fLengthMove < 0.2f*/m_fBossCamTime > 17.f)
					m_bBossCamSeq.set(6);
			}
			else if(!m_bBossCamSeq.test(7) && m_bBossCamSeq.test(6))
			{
				fCamSpeed = m_fBossCamSpeed[6];
				vLengthAt = m_vBossCamAtSet[7] - m_vBossCamAtSet[6];
				vLengthEye = m_vBossCamEyeSet[7] - m_vBossCamEyeSet[6];
				//vLengthMove = m_vBossCamEyeSet[7] - m_vEye;
				//fLengthMove = vLengthMove.Length();

				m_vAt += vLengthAt * fTimeDelta * fCamSpeed;
				m_vEye += vLengthEye * fTimeDelta * fCamSpeed;

				if(/*fLengthMove < 0.2f*/m_fBossCamTime > 20.f)
				{
					m_bBossCamSeq.reset();
					m_bActionCamSet.reset(ACT_BOSSROOM);
					RenewalCamera();
					CGhost_Knight* pBoss = (CGhost_Knight*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ghost_Knight")->front();
					pBoss->Set_BossCam(false);
					CPlayers* pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Guardian")->front();
					pPlayer->Set_BossCam(false);
					pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Paladin")->front();
					pPlayer->Set_BossCam(false);
					pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Magician")->front();
					pPlayer->Set_BossCam(false);
					pPlayer = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ranger")->front();
					pPlayer->Set_BossCam(false);
					CSoldier_1* pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_0")->front();
					pNPC->Set_BossCam(false);
					pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_1")->front();
					pNPC->Set_BossCam(false);
					pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_2")->front();
					pNPC->Set_BossCam(false);
					pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_3")->front();
					pNPC->Set_BossCam(false);
					pNPC = (CSoldier_1*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Soldier_1_4")->front();
					pNPC->Set_BossCam(false);
				}
			}
		}
	}
}

void CDynamicCamera::WyvernCamera_Static(void)
{
	m_vEye = m_pWyvernInfo->m_vDir * (-1);

	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;

	D3DXVECTOR3		vRight;
	D3DXMATRIX		matCamState = m_pWyvernInfo->m_matWorld;
	memcpy(&vRight, &matCamState.m[2][0], sizeof(float) * 3);

	D3DXMATRIX		matAxis;
	D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(m_fAngle_Pitch / 10.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matAxis);

	m_vEye += m_pWyvernInfo->m_vPos;

	m_vAt = m_pWyvernInfo->m_vPos + m_pWyvernInfo->m_vDir * 2.f;
	m_vAt.y += 0.85f;
}

void CDynamicCamera::Set_CamState(CAMSTATE eCam)
{
	m_eCamState = eCam;

}

CDynamicCamera::CAMSTATE CDynamicCamera::Get_CamState()
{
	return m_eCamState;
}

void CDynamicCamera::Set_MouseFix(bool bMouseFix)
{
	m_bMouseFix = bMouseFix;
}

void CDynamicCamera::Set_CameraType(WORD Number)
{
	m_eCamState = (CAMSTATE)Number;
}

void CDynamicCamera::WyvernCamera_Dynamic(void)
{
	D3DXMATRIX matHead = *(m_pWyvernMesh->FindFrame("Saddle"));

	D3DXMatrixIdentity(&matHead);
	D3DXMATRIX matScale, matTrans, matRotX, matRotY, matRotZ;
	D3DXMatrixScaling(&matScale, m_pWyvernInfo->m_vScale.x, m_pWyvernInfo->m_vScale.y, m_pWyvernInfo->m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pWyvernInfo->m_vPos.x, m_pWyvernInfo->m_vPos.y, m_pWyvernInfo->m_vPos.z);
	D3DXMatrixRotationX(&matRotX, m_pWyvernInfo->m_fAngle[Engine::ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_pWyvernInfo->m_fAngle[Engine::ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_pWyvernInfo->m_fAngle[Engine::ANGLE_Z]);

	matHead = matScale * matRotX * matRotY * matRotZ * matTrans;

	m_vEye = m_pWyvernInfo->m_vDir * (-1);

	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;

	D3DXVECTOR3		vRight , vUp;
	D3DXMATRIX		matCamState = m_pWyvernInfo->m_matWorld;
	memcpy(&vRight, &matCamState.m[2][0], sizeof(float) * 3);
	memcpy(&vUp, &matCamState.m[1][0], sizeof(float) * 3);

	D3DXMATRIX		matAxis;
	D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(m_fAngle_Pitch / 10.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matAxis);

	D3DXMatrixRotationAxis(&matAxis, &m_vUp, m_fAngle_Yaw);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matAxis);

	m_vEye += m_pWyvernInfo->m_vPos;
	memcpy(&m_vAt, &matHead.m[3][0], sizeof(D3DXVECTOR3));
	m_vAt.y += 0.85f;
}

_float CDynamicCamera::Get_SaveAngle()
{
	return m_fSaveAngle;
}

_float CDynamicCamera::Get_fAngle_Yaw()
{
	return m_fAngle_Yaw;
}

void CDynamicCamera::Set_fAngle_Yaw(_float fAngle_Yaw)
{
	m_fAngle_Yaw = fAngle_Yaw;
}

void CDynamicCamera::Set_Interpol(_bool bCheck)
{
	m_bInterpol = bCheck;
}

void CDynamicCamera::Set_SaveAngle(_float fAngle)
{
	m_fSaveAngle = fAngle;
}

void CDynamicCamera::Set_Free(_bool bCheck)
{
	m_bFree = bCheck;
}

_bool CDynamicCamera::Get_Free()
{
	return m_bFree;
}

void CDynamicCamera::Free(void)
{
	Engine::CGameObject::Free();
	_mymat matIdentity;
	matIdentity.Identity();

	m_pDevice->SetTransform(D3DTS_PROJECTION, &matIdentity);
	m_pDevice->SetTransform(D3DTS_VIEW, &matIdentity);
}

