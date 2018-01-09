#include "stdafx.h"
#include "WyvernLesser.h"

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
#include "SoundMgr.h"
#include "MainPlayer.h"

CWyvernLesser::CWyvernLesser(LPDIRECT3DDEVICE9 pDevice)
: CWyverns(pDevice)
{

}
CWyvernLesser::~CWyvernLesser(void)
{

}
HRESULT CWyvernLesser::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.01f, 0.01f, 0.01f);
	m_pInfo->m_vPos = _vec3(106.f, 0.f, 117.f);
	m_fSpeed = 200.f;

	m_fTimeDelta = 0.f;
	return S_OK;
}
_int CWyvernLesser::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	m_fSoundTime += fTimeDelta;

	if(false == m_bAct_Mount)
		return 0;

	//요거하면 지형탐(콤포넌트에 Calculator 추가)

	if(false == m_bJump_Wyvern && false == m_bFly)
	{
		_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
		m_pInfo->m_vPos.y = fHeight;
	}
	if(false == m_bAxel && false == m_bDecelerate)
		m_fAcc = 0.f;

	//

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &m_vLook, &m_pInfo->m_matWorld);

	_int iResult = Engine::CGameObject::Update(fTimeDelta);
	Compute_ViewZ(&m_pInfo->m_vPos);

	m_fTimeDelta = fTimeDelta;

	if(true != m_bAct_Mount)
	{
		if(NULL == m_pPlayerInfo)
			m_pPlayerInfo = (Engine::CTransform*)m_pPlayer->GetComponent(L"Transform");
		m_pInfo->m_vPos = (m_pPlayerInfo)->m_vPos;
	}

	UpdateAnimation(m_eWYTrack, fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return iResult;
}
void CWyvernLesser::Render(void)
{
	if(false == m_bAct_Mount)
		return;

	LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	CUnits::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

	pEffect->AddRef();

	Set_ContantTable(pEffect);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pWyvernMesh->RenderMesh_ForShader(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}
void CWyvernLesser::UpdateAnimation(CWyverns::WYVERNANI eTrack ,float fTrackTime)
{
	m_eWYTrack = eTrack;


	switch(m_eWYTrack)
	{
	case CWyverns::WY_IDLE:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case CWyverns::WY_RUN_F:
		Ani_Move_To_Wyvern(0.f);
		break;
	case CWyverns::WY_RUN_B:
		Ani_Move_To_Wyvern(0.f);
		break;
	case CWyverns::WY_IDLE_ROT_R_LOOP:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case CWyverns::WY_IDLE_ROT_L_LOOP:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case CWyverns::WY_RUN_JUMP_UP:
		Ani_Jump_To_Wyvern(0.f);
		break;
	case CWyverns::WY_RUN_JUMP_LOOP:
		Ani_Fly_To_Wyvern(0.f);
		break;
	case CWyverns::WY_JUMP_LAND:
		Ani_Jump_To_Wyvern(0.f);
		break;
	case CWyverns::WY_FLYING_B:
		Ani_Fly_To_Wyvern(0.f);
		break;
	case CWyverns::WY_FLYING_F_FLUTTER:
		Ani_Fly_To_Wyvern(0.f);
		break;
	case CWyverns::WY_FLYING_IDLE:
		Ani_Fly_To_Wyvern(0.f);
		break;
	}

	m_pWyvernMesh->FrameMove(fTrackTime / m_fSlowTime);

	if(m_pWyvernMesh->GetPeriod() - 0.3 <= m_pWyvernMesh->GetTrackPosition())
	{
		if(m_eWYTrack == WY_RUN_JUMP_UP)
		{
			m_fJumpAcc = 0.f;
			m_bFly = true;
		}
		if(m_eWYTrack == WY_JUMP_LAND)
		{
			m_fJumpAcc = 0.f;
			m_fSlowTime = 1.f;
		}
		m_pWyvernMesh->SetAnimationSet(m_eWYTrack);
		//m_pMesh->SetTrackPos(0.0);
	}
}
////////////////////////////////////////////////////////////////////////// 애니메이션 관련 함수
void CWyvernLesser::Ani_Jump_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

	CPlayers* pPlayer =	((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer();
	if(pPlayer == NULL)
		return;
	float fSoundDistance = abs(D3DXVec3Length(&pPlayer->GetInfo()->m_vPos) - D3DXVec3Length(&m_pInfo->m_vPos));
	float fSoundVolume = -((fSoundDistance/6.f) - 0.5f);


	_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
	if(m_eWYTrack == WY_RUN_JUMP_UP)
	{
		if(m_fSoundTime < 0.1f)
			CSoundMgr::GetInstance()->WyvernSound1(L"WyvernFlyMove.ogg",fSoundVolume);
		if(m_fSoundTime > 6.f)
			m_fSoundTime = 0.f;
		m_fJumpAcc += 3.f * m_fTimeDelta;
		m_pInfo->m_vPos.y += (0.98f * m_fJumpAcc + m_fJumpAcc * 0.5f - 0.5f) * m_fTimeDelta;
	}
	if(m_eWYTrack == WY_JUMP_LAND)
	{
		if(m_pInfo->m_vPos.y >= fHeight)
		{
			m_fSlowTime = 2.f;
			m_pInfo->m_vPos.y -= 2.f * m_fTimeDelta; 
		}
		else
		{
			m_fSlowTime = 1.f;
			m_fJumpAcc = 0.f;
			m_bDown = false;
			m_bDead = false;
			m_bFly = false;
			m_bJump_Wyvern = false;

		}

	}

}

void CWyvernLesser::Ani_Move_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

}

void CWyvernLesser::Ani_Fly_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

	CPlayers* pPlayer =	((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer();
	if(pPlayer == NULL)
		return;
	float fSoundDistance = abs(D3DXVec3Length(&pPlayer->GetInfo()->m_vPos) - D3DXVec3Length(&m_pInfo->m_vPos));
	float fSoundVolume = -((fSoundDistance/6.f) - 0.5f);


	if(m_eWYTrack == CWyverns::WY_FLYING_B)
	{
		if(m_fSoundTime < 0.1f)
			CSoundMgr::GetInstance()->WyvernSound1(L"WyvernFlyMove.ogg",fSoundVolume);
		if(m_fSoundTime > 1.5f)
			m_fSoundTime = 0.f;
		m_pInfo->m_vPos -= m_pInfo->m_vDir * (m_fSpeed / 1.5f) * m_fTimeDelta; 
	}
	if(m_eWYTrack == CWyverns::WY_FLYING_F_FLUTTER)
	{
		if(m_fSoundTime < 0.1f)
			CSoundMgr::GetInstance()->WyvernSound1(L"WyvernFlyMove.ogg",fSoundVolume);
		if(m_fSoundTime > 1.5f)
			m_fSoundTime = 0.f;

		if(true == m_bAxel)
		{
			if(m_fAcc >= 15.f)
				m_eWYTrack = CWyverns::WY_RUN_JUMP_LOOP;
			else
				m_fAcc += 1.5f * m_fTimeDelta;

			m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f) * m_fTimeDelta;
		}
		else
		{
			if(true == m_bDecelerate)
			{
				if(m_fAcc >= 0.f)
					m_fAcc -= 1.f * m_fTimeDelta;
				else
				{
					m_fAcc = 0.f;
					m_bDecelerate = false;
				}
				m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f) * m_fTimeDelta;
			}
			else
			{
				if(m_bPoint == false)
					m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * 1.5f) * m_fTimeDelta;
				else
					m_pInfo->m_vPos += m_vPointDir * (m_fSpeed * 0.03f) * m_fTimeDelta;
			}
		}
	}
	if(m_eWYTrack == CWyverns::WY_RUN_JUMP_LOOP)
	{
		if(true == m_bDecelerate)
		{
			if(m_fAcc <= 12.f)
				m_eWYTrack = CWyverns::WY_FLYING_F_FLUTTER;
			else
				m_fAcc -= 1.f * m_fTimeDelta;

			m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f) * m_fTimeDelta;
		}
		else
		{
			if(m_fAcc >= 15.f && m_fAcc <= 20.f)
				m_fAcc += 1.5f * m_fTimeDelta;

			if(m_bPoint == false)
				m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f) * m_fTimeDelta;
			else
				m_pInfo->m_vPos += m_vPointDir * (m_fSpeed * 0.07f) * m_fTimeDelta;
		}
	}
	if(m_eWYTrack == CWyverns::WY_FLYING_IDLE)
	{
		if(true == m_bDown)
		{	
			if(m_fSoundTime < 0.1f)
				CSoundMgr::GetInstance()->WyvernSound1(L"WyvernFlyMove.ogg",fSoundVolume);
			if(m_fSoundTime > 1.5f)
				m_fSoundTime = 0.f;
			m_fJumpAcc -= 3.f * m_fTimeDelta;
		}
		else
		{
			if(m_fSoundTime < 0.1f)
				CSoundMgr::GetInstance()->WyvernSound1(L"WyvernFlyIdle.ogg",fSoundVolume);
			if(m_fSoundTime > 1.5f)
				m_fSoundTime = 0.f;
			m_pInfo->m_vPos = m_pInfo->m_vPos;
		}
	}

}

void CWyvernLesser::Ani_Stand_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

}
//////////////////////////////////////////////////////////////////////////
HRESULT CWyvernLesser::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_WyvernLesser");
	m_pWyvernMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_VTF");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 10000.0f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);

	//Calculator
	pComponent = m_pCalculatorCom = CCalculator::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Calculator", pComponent);

	return S_OK;
}

void CWyvernLesser::Set_ContantTable(LPD3DXEFFECT	pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CWyvernLesser* CWyvernLesser::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CWyvernLesser*		pGameObject = new CWyvernLesser(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CWyvernLesser::Free(void)
{
	CWyverns::Free();
}