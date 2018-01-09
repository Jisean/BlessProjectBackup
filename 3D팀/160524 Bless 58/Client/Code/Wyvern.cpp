#include "stdafx.h"
#include "Wyvern.h"

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

CWyvern::CWyvern(LPDIRECT3DDEVICE9 pDevice)
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
{

}
CWyvern::~CWyvern(void)
{

}
HRESULT CWyvern::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.01f, 0.01f, 0.01f);
	m_pInfo->m_vPos = _vec3(106.f, 0.f, 117.f);
	m_fSpeed = 200.f;

	return S_OK;
}
_int CWyvern::Update(const _float& fTimeDelta)
{
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

	//cout << m_fAcc << endl;
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &m_vLook, &m_pInfo->m_matWorld);

	_int iResult = Engine::CGameObject::Update(fTimeDelta);
	Compute_ViewZ(&m_pInfo->m_vPos);

	m_fTimeDelta = fTimeDelta;

	if(true == m_bAct_Mount)
	{
		if(NULL == m_pPlayerInfo)
		{
			m_pPlayerInfo = ((Engine::CTransform*)Engine::Get_Component(CStage::LAYER_GAMELOGIC, L"Paladin", L"Transform"));
			m_pInfo->m_vPos = (m_pPlayerInfo)->m_vPos;
		}
		if(NULL == m_pmatSaddle)
		{
			m_pmatSaddle = m_pWyvernMesh->FindFrame("Saddle");
		}
	}


	
	UpdateAnimation(m_eWYTrack,fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return iResult;
}
void CWyvern::Render(void)
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
void CWyvern::UpdateAnimation(WYVERNANI eTrack ,float fTrackTime)
{
	m_eWYTrack = eTrack;

	switch(m_eWYTrack)
	{
	case WY_IDLE:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case WY_RUN_F:
		Ani_Move_To_Wyvern(0.f);
		break;
	case WY_RUN_STOP:
		Ani_Move_To_Wyvern(0.f);
		break;
	case WY_RUN_B:
		Ani_Move_To_Wyvern(0.f);
		break;
	case WY_IDLE_ROT_R_START:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case WY_IDLE_ROT_R_LOOP:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case WY_IDLE_ROT_L_START:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case WY_IDLE_ROT_L_LOOP:
		Ani_Stand_To_Wyvern(0.f);
		break;
	case WY_RUN_JUMP_UP:
		Ani_Jump_To_Wyvern(0.f);
		break;
	case WY_RUN_JUMP_LOOP:
		Ani_Fly_To_Wyvern(0.f);
		break;
	case WY_JUMP_LAND:
		Ani_Jump_To_Wyvern(0.f);
		break;
	case WY_FLYING_B:
		Ani_Fly_To_Wyvern(0.f);
		break;
	case WY_FLYING_F_FLUTTER:
		Ani_Fly_To_Wyvern(0.f);
		break;
	case WY_FLYING_IDLE:
		Ani_Fly_To_Wyvern(0.f);
		break;
	}

	_vec3 vSaddlePos = _vec3((*m_pmatSaddle)._41,(*m_pmatSaddle)._42,(*m_pmatSaddle)._43);
	D3DXVec3TransformCoord(&vSaddlePos,&vSaddlePos,&m_pInfo->m_matWorld);

	m_pPlayerInfo->m_vPos = vSaddlePos;

	m_pPlayerInfo->m_fAngle[Engine::ANGLE_Y] = m_pInfo->m_fAngle[Engine::ANGLE_Y];
	m_pPlayerInfo->m_fAngle[Engine::ANGLE_X] = m_pInfo->m_fAngle[Engine::ANGLE_X];
	m_pPlayerInfo->m_fAngle[Engine::ANGLE_Z] = m_pInfo->m_fAngle[Engine::ANGLE_Z];

	m_pPlayerInfo->m_vPos.y = m_pPlayerInfo->m_vPos.y - 0.2f;

	m_pPlayerInfo->Update(); //바뀐 값들을 행렬 구성을 다시 해준다 씨1발

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
void CWyvern::Ani_Jump_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

	_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
	//cout << fTrackTime << endl;
	if(m_eWYTrack == WY_RUN_JUMP_UP)
	{
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

void CWyvern::Ani_Move_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

}

void CWyvern::Ani_Fly_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

	if(m_eWYTrack == WY_FLYING_B)
	{
		m_pInfo->m_vPos -= m_pInfo->m_vDir * (m_fSpeed / 1.5f) * m_fTimeDelta; 
	}
	if(m_eWYTrack == WY_FLYING_F_FLUTTER)
	{
		if(true == m_bAxel)
		{
			if(m_fAcc >= 15.f)
				m_eWYTrack = WY_RUN_JUMP_LOOP;
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
				m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * 1.5f) * m_fTimeDelta;
			}
		}
	}
	if(m_eWYTrack == WY_RUN_JUMP_LOOP)
	{
		if(true == m_bDecelerate)
		{
			if(m_fAcc <= 12.f)
				m_eWYTrack = WY_FLYING_F_FLUTTER;
			else
				m_fAcc -= 1.f * m_fTimeDelta;

			m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f) * m_fTimeDelta;
		}
		else
		{
			 if(m_fAcc >= 15.f && m_fAcc <= 20.f)
				m_fAcc += 1.5f * m_fTimeDelta;

			m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * m_fAcc + m_fAcc * 0.2f - 0.2f) * m_fTimeDelta;
		}
	}
	if(m_eWYTrack == WY_FLYING_IDLE)
	{
		if(true == m_bDown)
		{	
			m_fJumpAcc -= 3.f * m_fTimeDelta;
		}
		else
		m_pInfo->m_vPos = m_pInfo->m_vPos;
	}

}

void CWyvern::Ani_Stand_To_Wyvern(float fTrackTime)
{
	fTrackTime = (float)m_pWyvernMesh->GetTrackPosition();
	m_pWyvernMesh->SetAnimationSet(m_eWYTrack);

}
//////////////////////////////////////////////////////////////////////////
HRESULT CWyvern::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Wyvern");
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

void CWyvern::Set_ContantTable(LPD3DXEFFECT	pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CWyvern* CWyvern::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CWyvern*		pGameObject = new CWyvern(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CWyvern::Free(void)
{
	Engine::CGameObject::Free();
}

////////////////////////////////////////////////////////////////////////// GET
Engine::CDynamicMesh* CWyvern::Get_Mesh()
{
	return m_pWyvernMesh;
}

CWyvern::WYVERNANI CWyvern::Get_TrackID()
{
	return m_eWYTrack;
}

bool CWyvern::Get_ActMount()
{
	return m_bAct_Mount;
}

bool CWyvern::Get_Jump()
{
	return m_bJump_Wyvern;
}

bool CWyvern::Get_Fly()
{
	return m_bFly;
}

bool CWyvern::Get_Axel()
{
	return m_bAxel;
}

bool CWyvern::Get_Decelerate()
{
	return m_bDecelerate;
}

float CWyvern::Get_fAcc()
{
	return m_fAcc;
}

bool CWyvern::Get_Down()
{
	return m_bDown;
}

float CWyvern::Get_JumpAcc()
{
	return m_fJumpAcc;
}

////////////////////////////////////////////////////////////////////////// SET
void CWyvern::Set_TrackID(WYVERNANI eTrack)
{
	m_eWYTrack = eTrack;
}



void CWyvern::Set_ActMount(bool bCheck)
{
	m_bAct_Mount = bCheck;
}

void CWyvern::Set_Jump(bool bCheck)
{
	m_bJump_Wyvern = bCheck;
}

void CWyvern::Set_Fly(bool bCheck)
{
	m_bFly = bCheck;
}

void CWyvern::Set_JumpAcc_Init(float fJumpAcc)
{
	m_fJumpAcc = fJumpAcc;
}

void CWyvern::Set_Axel(bool bAxel)
{
	m_bAxel = bAxel;
}

void CWyvern::Set_Decelerate(bool bDecelerate)
{
	m_bDecelerate = bDecelerate;
}

void CWyvern::Set_Down(bool bCheck)
{
	m_bDown = bCheck;
}

void CWyvern::Set_JumpAcc(float fAcc)
{
	m_fJumpAcc = fAcc;
}
