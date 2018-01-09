#include "stdafx.h"
#include "S_Slime.h"

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
#include "Skill_Range.h"
#include "Skill_Range_1.h"
#include "MainPlayer.h"
#include "SoundMgr.h"

#include "MonsterDmg.h"
#include "NPCs.h"

CS_Slime::CS_Slime(LPDIRECT3DDEVICE9 pDevice)
: CMonsters(pDevice)
, m_bPush(false)
//, m_iAniIdx(0)
{
	m_pBoss = NULL;
	m_fSkillTime = 0.f;
}

CS_Slime::~CS_Slime(void)
{

}

HRESULT CS_Slime::Initialize(void)
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

	// SK_01
	ATTACK_DMG_TIMIMG* pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 3.f;
	pSK_Timing->fInterval_2 = 4.5f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);


	m_vecSK_Cooltime.push_back(0.f);		// SK_01



	m_iAniIdx = CAIMgr::STATE_DOWN_LOOP;
	m_pMesh->SetAnimationSet(m_iAniIdx);
	m_pMesh->SetTrackPos(0.0);


	return S_OK;
}

_int CS_Slime::Update(const _float& fTimeDelta)
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

		TCHAR	szObjName[30] = L"";
		wsprintf(szObjName, L"Skill_Range_%d", m_dwMyIndex);

		TCHAR	szObjName2[30] = L"";
		wsprintf(szObjName2, L"Skill_Range_1_%d", m_dwMyIndex);

		m_pSkillRange = (CSkill_Range*)(Engine::Get_Scene()->GetObjList(LAYER_GAMELOGIC, szObjName)->front());
		m_pSkillRange_1 = (CSkill_Range_1*)(Engine::Get_Scene()->GetObjList(LAYER_GAMELOGIC, szObjName2)->front());
	}


	if(m_pBoss->Get_AniIdx() == CAIMgr::STATEBOSS_DIE)
	{
		m_pStatus->m_tRealStat.m_fCurHp = 0.f;
		m_fGetHit_DMG = 50.f; 
		m_fFrameSpeedCtr = 1.f;
	}


	if(m_bDieStart /*m_bDieState*/)
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
				{
					m_bInitInfo = false;
					m_bDead = true;
					m_fDie_VanishTime = 0.f;
				}
			}
			else
			{
				if(m_pInfo->m_vPos.y > m_vOriPos.y - 0.2f)
					m_pInfo->m_vPos.y -= 0.001f;
				else
				{
					m_bInitInfo = false;
					m_bDead = true;
					m_fDie_VanishTime = 0.f;
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

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &m_vLook, &m_pInfo->m_matWorld);

	// AI 작동 

	if(m_iAniIdx != CAIMgr::STATE_DIE)
		CAIMgr::GetInstance()->Monster_Pattern(&m_vecPlayersInfo, this, m_pCollision, fTimeDelta);


	// 해당 애니메이션 인덱스 정보를 적용 시켜 줌
	Engine::ANI_INFO* Data = m_vecAniInfo[m_iAniIdx];
	m_pMesh->SetAniInfo(*Data);	

	// 애니메이션 끝
	AniFrameEnd(Data);

	_int iResult = CLandObject::Update(fTimeDelta);
	iResult = Engine::CGameObject::Update(fTimeDelta);


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

void CS_Slime::Render(void)
{
	m_pMesh->FrameMove(m_fFrameTime * m_fFrameSpeedCtr);

	//if(m_bCulled)
	//{
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



	// Box & Sphere Render
#ifdef _DEBUG
	//		//BoxRender();
#endif
	//}
}

void CS_Slime::AniFrameEnd(Engine::ANI_INFO* pData)
{
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

		// 스킬 쿨타임 초기화
		if(m_iAniIdx == CAIMgr::STATE_SK_1)
		{
			m_bOnSkill_Range = false;
			m_pStatus->m_tRealStat.m_fCurHp = 0.f;
			m_vecSK_Cooltime[0] = 0.f;
		}

		if(m_bSkillState)
		{
			// 플레이어에게 스킬 범위를 보냄
			vector<CUnits*>::iterator iterInfo = CAIMgr::GetInstance()->Get_player_NPC_Info()->begin();
			vector<CUnits*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_player_NPC_Info()->end();

			for(; iterInfo != iterInfo_end; ++iterInfo)
			{
				if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
					((CPlayers*)(*iterInfo))->Set_RangeSkill_Check(_vec3(0.f, 0.f, 0.f) , false, 0.f);
			}
		}
		m_bSkillState = false;


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

void CS_Slime::Skill_State(const _float& fTimeDelta)
{
	float fTrack = (float)m_pMesh->GetTrackPosition();

	if(m_bSkillState)
	{
		switch(m_iAniIdx)
		{
		case CAIMgr::STATE_SK_1:
			{
				_vec3 vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
				_vec3 vMyPos = m_pInfo->m_vPos;

				_vec3 vLength = vTargetPos - vMyPos;	
				float fLength = D3DXVec3Length(&vLength);

				float fTiming_1 = m_vecSkill_DMG_Timing[0]->fInterval_1;
				float fTiming_2 = m_vecSkill_DMG_Timing[0]->fInterval_2;

				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					// 공격 데미지

					if(!m_bDMG_Timing)
					{
						m_ePlayer_ACT_Type = ACT_HIT_UP;

						m_bDMG_Timing = true;			 

						vector<CUnits*>::iterator iterInfo = CAIMgr::GetInstance()->Get_player_NPC_Info()->begin();
						vector<CUnits*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_player_NPC_Info()->end();

						for(; iterInfo != iterInfo_end; ++iterInfo)
						{		
							vTargetPos = (*iterInfo)->GetInfo()->m_vPos;
							vMyPos = m_pInfo->m_vPos;

							vLength = vTargetPos - vMyPos;	
							fLength = D3DXVec3Length(&vLength);

							if(1.f > fLength)
							{
								float fDamege = (_float)CRandom_Number::Get_RandomNum(3,3);

								if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
									((CPlayers*)(*iterInfo))->Set_DamageType((CPlayers::ACT_STATE)m_ePlayer_ACT_Type, fDamege, Engine::CStatus::DMG_MELEE);	
								else
								{
									//const_cast<Engine::CStatus*>((((CNPCs*)(*iterInfo)))->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
									//Engine::CGameObject* pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(*iterInfo))->GetInfo()->m_vPos, fDamege);							 
									//Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);
								}
								 
							}
						}
						_vec3 vSkillPos= m_pInfo->m_vPos;
						Engine::Call_MonsterFX(Engine::SPIDER_EXPLO, &vSkillPos, false);
					}	
				}
				return;
			}
		}
		return;
	}

	_vec3  vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
	_vec3  vDist = m_pInfo->m_vPos - vTargetPos;

	float fDist = D3DXVec3Length(&vDist);

	if(fDist < 2.f && m_vecSK_Cooltime[0] > m_fSkillTime)
	{
		m_bSkillState = true;
		m_iAniIdx = CAIMgr::STATE_SK_1;

		if(!m_bOnSkill_Range)
		{
			// 플레이어에게 스킬 범위를 보냄
			vector<CUnits*>::iterator iterInfo = CAIMgr::GetInstance()->Get_player_NPC_Info()->begin();
			vector<CUnits*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_player_NPC_Info()->end();

			for(; iterInfo != iterInfo_end; ++iterInfo)
			{
				if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
					((CPlayers*)(*iterInfo))->Set_RangeSkill_Check(m_pInfo->m_vPos, true, 1.f);
			}

			m_pSkillRange->Start_SkillRange(m_pSkillRange_1, m_pInfo->m_vPos, 1.f, 0.0085f);
			m_pSkillRange_1->Start_SkillRange(m_pInfo->m_vPos, 0.2f, 1.f, 0.0085f);
			m_bOnSkill_Range = true;
		}
		return;
	}
	else
		m_vecSK_Cooltime[0] += fTimeDelta;	
}

HRESULT CS_Slime::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Slime");
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

void CS_Slime::Set_ContantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CS_Slime* CS_Slime::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CS_Slime*		pGameObject = new CS_Slime(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CS_Slime::Free(void)
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

void CS_Slime::KeyCheck(void)
{

}

void CS_Slime::Sound(DWORD dwMyIndex)
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
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->SlimeSound(L"SlimeMove.ogg", dwMyIndex, fSoundVolume);
		break;
	case CAIMgr::STATE_RUN:
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->SlimeSound(L"SlimeMove.ogg", dwMyIndex, fSoundVolume);
		break;
	case CAIMgr::STATE_ATK_1:
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->SlimeSound(L"SlimeAtt.ogg", dwMyIndex, fSoundVolume);
		break;
	case CAIMgr::STATE_ATK_2:	
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->SlimeSound(L"SlimeAtt.ogg", dwMyIndex, fSoundVolume);
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
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->SlimeSound(L"SlimeDead.ogg", dwMyIndex, fSoundVolume);
		break;
	}

	return;
}