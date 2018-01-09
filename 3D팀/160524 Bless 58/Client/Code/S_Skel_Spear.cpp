#include "stdafx.h"
#include "S_Skel_Spear.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Calculator.h"
#include "Material.h"
#include "Status.h"
#include "AniCtrl.h"
#include "KeyMgr.h"
#include "AIMgr.h"

#include "Stage.h"
#include "Players.h"
#include "Magician.h"
#include "Guardian.h"
#include "Paladin.h"
#include "Ranger.h"

#include "Collision.h"
#include "MainPlayer.h"
#include "SoundMgr.h"

CS_Skel_Spear::CS_Skel_Spear(LPDIRECT3DDEVICE9 pDevice)
: CMonsters(pDevice)
, m_bPush(false)
//, m_iAniIdx(0)
{
	m_pBoss = NULL;
}

CS_Skel_Spear::~CS_Skel_Spear(void)
{

}

HRESULT CS_Skel_Spear::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_eUnitType = CUnits::TYPEID_SUMMON;

	m_pInfo->m_vScale = _vec3(0.01f, 0.01f, 0.01f);

	m_fCautionRange = 5.f;
	m_fCollisionRange = 0.3f;
	m_fAttackDist = 0.4f;
	m_fSpeed = 1.5f;

	// ATK_01
	ATTACK_DMG_TIMIMG* pTiming = new ATTACK_DMG_TIMIMG;
	pTiming->fInterval_1 = 1.033f;
	pTiming->fInterval_2 = 1.1f;
	m_vecAttack_DMG_Timing.push_back(pTiming);

	// ATK_02
	ATTACK_DMG_TIMIMG* pTiming2 = new ATTACK_DMG_TIMIMG;
	pTiming2->fInterval_1 = 1.052f;
	pTiming2->fInterval_2 = 1.098f;
	m_vecAttack_DMG_Timing.push_back(pTiming2);

	m_iAniIdx = CAIMgr::STATE_DOWN_LOOP;
	m_pMesh->SetAnimationSet(m_iAniIdx);
	m_pMesh->SetTrackPos(0.0);

	m_bMonster_Wake = false;
	m_bDieState = true;

	m_bDieStart = true;
	return S_OK;
}

_int CS_Skel_Spear::Update(const _float& fTimeDelta)
{
	AttackPositionSetting();
	KeyCheck();
	Sound(m_dwMyIndex);

	if(!m_bInitInfo)
	{
		m_vOriPos = m_pInfo->m_vPos;
		m_fOriAngle = m_pInfo->m_fAngle[Engine::ANGLE_Y];
		m_bInitInfo = true;
		m_dwIndex = Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos);

		_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
		m_pInfo->m_vPos.y = fHeight;	
	}


	if(m_pBoss->Get_AniIdx() == CAIMgr::STATEBOSS_DIE)
	{
		m_pStatus->m_tRealStat.m_fCurHp = 0.f;
		m_fGetHit_DMG = 50.f; 
		m_bMonster_Wake = true;
		m_fFrameSpeedCtr = 1.f;
	}
	
	if(m_bMonster_Wake)
	{
		if(m_bDieStart/*m_bDieState*/)
		{	
			if(m_pBoss->Get_bDieState())		// 보스가 죽으면 소환물도 죽는다
			{
				if(m_fDie_VanishTime < 3.f)
				{
					m_fDie_VanishTime += fTimeDelta;
				}
				else
				{
					if(m_pTargetInfo != NULL)
					{
						if(m_pInfo->m_vPos.y > m_pTargetInfo->GetInfo()->m_vPos.y - 0.2f)
							m_pInfo->m_vPos.y -= 0.001f;
						else
							m_bDead = true;
					}
					else
					{
						if(m_pInfo->m_vPos.y > m_vOriPos.y - 0.2f)
							m_pInfo->m_vPos.y -= 0.001f;
						else
							m_bDead = true;
					}
				}
			}	 
		}
		else
		{
			//요거하면 지형탐(콤포넌트에 Calculator 추가)
			if(TYPE_TERRAIN == Engine::CNaviMgr::GetInstance()->GetCellType(m_dwIndex))
			{
				_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
				m_pInfo->m_vPos.y = fHeight;
			}
		}
	}


	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &m_vLook, &m_pInfo->m_matWorld);

	// AI 작동 
	if(m_bMonster_Wake)
	{
		if(m_iAniIdx != CAIMgr::STATE_DIE)
			CAIMgr::GetInstance()->Monster_Pattern(&m_vecPlayersInfo, this, m_pCollision, fTimeDelta);
	}

	// 해당 애니메이션 인덱스 정보를 적용 시켜 줌
	Engine::ANI_INFO* Data = m_vecAniInfo[m_iAniIdx];
	m_pMesh->SetAniInfo(*Data);	
	 
	// 애니메이션 끝
	AniFrameEnd(Data);

	_int iResult = 0;
	if(m_bCulled)
	{
		if(20 == m_iCullingCnt)
		{
			iResult = CUnits::Update(fTimeDelta);
			m_iCullingCnt = 0;
		}
		else
			++m_iCullingCnt;
	}
	else
		iResult = CUnits::Update(fTimeDelta);

	iResult = Engine::CGameObject::Update(fTimeDelta);

	if(m_bCulled)
		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	vector<D3DXMATRIX*>::iterator iterOri = m_vecOriMat.begin();
	vector<D3DXMATRIX*>::iterator itermat = m_vecMatrixBox.begin();
	vector<D3DXMATRIX*>::iterator itermat_end = m_vecMatrixBox.end();

	for(; itermat != itermat_end; ++itermat)		
	{
		memcpy((*itermat), (*iterOri), sizeof(_mymat));
		++iterOri;
	}

	m_fFrameTime = fTimeDelta;


	return iResult;
}

void CS_Skel_Spear::Render(void)
{
	m_pMesh->FrameMove(m_fFrameTime * m_fFrameSpeedCtr);

	if(m_bCulled)
	{
		LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
		if(NULL == pEffect)
			return;

		CUnits::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

		pEffect->AddRef();

		Set_ContantTable(pEffect);

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);

		m_pMesh->RenderMesh_ForShader(pEffect);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);    
	}
}

void CS_Skel_Spear::AniFrameEnd(Engine::ANI_INFO* pData)
{
 	if(!m_bMonster_Wake)
	{
		if(m_pMesh->GetPeriod() - pData->fPeriod < m_pMesh->GetTrackPosition())
			m_fFrameSpeedCtr = 0.f;	
		return;
	}

	if(m_pMesh->GetPeriod() - pData->fPeriod < m_pMesh->GetTrackPosition())
	{	
		if(m_iAniIdx == CAIMgr::STATE_DIE)
		{
			m_bMonster_Wake = false;

			//m_bDieState = true;
			m_bDieStart = true;
			
			m_bDMG_Timing = false;
			m_bHitState = false;
			m_bStunState = false;
			m_bAttackEnd = false;
			m_bAttack = false;
			m_fGetHit_DMG = 0.f;

			m_fFrameSpeedCtr = 0.f;
			return;
		}

		m_bDMG_Timing = false;

		m_bHitState = false;
		m_bStunState = false;

		if(m_bAttackEnd)
		{
			m_bAttackEnd = false;
			m_bAttack = false;
		}

		if(m_iAniIdx == CAIMgr::STATE_DOWN || m_iAniIdx == CAIMgr::STATE_DOWN_LOOP)
		{
			if(m_iAniIdx == CAIMgr::STATE_DOWN)
				m_iAniIdx = CAIMgr::STATE_DOWN_LOOP;
			else
				m_iAniIdx = CAIMgr::STATE_GET_UP;
		}
		else
		{	
			m_bDownState = false;
			if(m_iAniIdx == CAIMgr::STATE_IDLE_P)
			{
				m_iAniIdx = CAIMgr::STATE_IDLE_P;
				m_iPrevAniIdx = CAIMgr::STATE_IDLE_P;
			}
			else
			{	
				if(m_iAniIdx != CAIMgr::STATE_RUN && m_iAniIdx != CAIMgr::STATE_WALK)
				{
					m_iAniIdx = CAIMgr::STATE_IDLE_C;
					m_iPrevAniIdx = CAIMgr::STATE_IDLE_C;
					m_pMesh->SetAnimationSet(m_iAniIdx);
					m_pMesh->SetTrackPos(0.0);
				}	
			}
			m_ePlayer_ACT_Type = ACT_DEFAULT;
		}
	}
}

HRESULT CS_Skel_Spear::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Skel_Spear");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
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

	// For.Status Component
	Engine::_BaseStat* pBaseStat = new Engine::_BaseStat;
	pBaseStat->m_fStr = 25.f;
	pBaseStat->m_fDex = 15.f;
	pBaseStat->m_fHealth = 150.f;
	pBaseStat->m_fInt = 8.f;
	pBaseStat->m_fWis = 8.f;
	pBaseStat->m_fExStat = 10.f;
	pBaseStat->m_fLevel = 1.f;
	pComponent = m_pStatus = Engine::CStatus::Create(*pBaseStat);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Status", pComponent);
	Engine::Safe_Delete(pBaseStat);

	//Calculator
	pComponent = m_pCalculatorCom = CCalculator::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Calculator", pComponent);

	// For.Collision Component
	pComponent = m_pCollision = CCollision::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Collision", pComponent);

	return S_OK;
}

void CS_Skel_Spear::Set_ContantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CS_Skel_Spear* CS_Skel_Spear::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CS_Skel_Spear*		pGameObject = new CS_Skel_Spear(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CS_Skel_Spear::Free(void)
{
	Engine::CGameObject::Free();
	CUnits::Free();

	m_vecPlayersInfo.clear();
	m_vecSK_Cooltime.clear();

	vector<ATTACK_DMG_TIMIMG*>::iterator iter = m_vecAttack_DMG_Timing.begin();
	vector<ATTACK_DMG_TIMIMG*>::iterator iter_end = m_vecAttack_DMG_Timing.end();

	for(; iter != iter_end; ++iter)
	{
		Engine::Safe_Delete((*iter));
	}
	m_vecAttack_DMG_Timing.clear();

	vector<ATTACK_DMG_TIMIMG*>::iterator iterSk = m_vecSkill_DMG_Timing.begin();
	vector<ATTACK_DMG_TIMIMG*>::iterator iterSk_end = m_vecSkill_DMG_Timing.end();

	for(; iterSk != iterSk_end; ++iterSk)
	{
		Engine::Safe_Delete((*iterSk));
	}
	m_vecSkill_DMG_Timing.clear();
}

void CS_Skel_Spear::KeyCheck(void)
{

}

void CS_Skel_Spear::Sound(DWORD dwMyIndex)
{
	float fTrack = (float)m_pMesh->GetTrackPosition();
	CPlayers* pPlayer =	((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer();
	if(pPlayer == NULL)
		return;

	float fLength = abs(D3DXVec3Length(&pPlayer->GetInfo()->m_vPos) - D3DXVec3Length(&m_pInfo->m_vPos));
	float fSoundVolume = -((fLength/6.f) - 0.5f);

	if(fLength > 6.f)
		return;

	switch(m_iAniIdx)
	{
	case CAIMgr::STATE_IDLE_P:
		break;
	case CAIMgr::STATE_IDLE_C:
		break;
	case CAIMgr::STATE_WALK:

		break;
	case CAIMgr::STATE_RUN:

		break;
	case CAIMgr::STATE_ATK_1:
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->SkelSpearSound(L"SkelSpearAtt1.ogg", dwMyIndex, fSoundVolume);
		break;
	case CAIMgr::STATE_ATK_2:	
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->SkelSpearSound(L"SkelSpearAtt2.ogg", dwMyIndex, fSoundVolume);
		break;
	case CAIMgr::STATE_SK_1:	

		break;
	case CAIMgr::STATE_DMG_F:		

		break;
	case CAIMgr::STATE_DOWN:		

		break;
	case CAIMgr::STATE_DOWN_LOOP:

		break;
	case CAIMgr::STATE_GET_UP:

		break;
	case CAIMgr::STATE_STUN:	

		break;
	case CAIMgr::STATE_DIE:
		break;
	}

	return;
}