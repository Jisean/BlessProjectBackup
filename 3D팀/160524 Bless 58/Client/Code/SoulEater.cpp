#include "stdafx.h"
#include "SoulEater.h"

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
#include "DynamicMesh.h"
#include "Skill_Range.h"
#include "Skill_Range_1.h"

#include "SoundMgr.h"
#include "MainPlayer.h"

#include "MonsterDmg.h"
#include "NPCs.h"

CSoulEater::CSoulEater(LPDIRECT3DDEVICE9 pDevice)
: CMonsters(pDevice)
, m_bPush(false)
//, m_iAniIdx(0)
{
	m_fTest = 0.f;
}

CSoulEater::~CSoulEater(void)
{

}

HRESULT CSoulEater::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_bBoss_Type = true;
	m_eUnitType = CUnits::TYPEID_BOSS;

	m_pInfo->m_vScale = _vec3(0.011f, 0.011f, 0.011f);

	m_fCautionRange = 3.5f;
	m_fCollisionRange = 0.5f;
	m_fAttackDist = 0.5f;

	// ATK_01
	ATTACK_DMG_TIMIMG* pTiming = new ATTACK_DMG_TIMIMG;
	pTiming->fInterval_1 = 0.6f;
	pTiming->fInterval_2 = 0.7f;
	m_vecAttack_DMG_Timing.push_back(pTiming);

	// ATK_02
	ATTACK_DMG_TIMIMG* pTiming2 = new ATTACK_DMG_TIMIMG;
	pTiming2->fInterval_1 = 1.1f;
	pTiming2->fInterval_2 = 1.2f;
	m_vecAttack_DMG_Timing.push_back(pTiming2);

	// SK_01
	ATTACK_DMG_TIMIMG* pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 1.1f;
	pSK_Timing->fInterval_2 = 1.8f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	// SK_02
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 1.58f;
	pSK_Timing->fInterval_2 = 1.7f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	// SK_03
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 2.1f;
	pSK_Timing->fInterval_2 = 2.3f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	// SK_04
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 3.f;
	pSK_Timing->fInterval_2 = 3.7f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	// SK_05
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 3.f;
	pSK_Timing->fInterval_2 = 3.7f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	// SK_06
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 3.f;
	pSK_Timing->fInterval_2 = 3.7f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	m_vecSK_Cooltime.push_back(0.f);		// SK_01
	m_vecSK_Cooltime.push_back(0.f);		// SK_02
	m_vecSK_Cooltime.push_back(0.f);		// SK_03
	m_vecSK_Cooltime.push_back(0.f);		// SK_04
	m_vecSK_Cooltime.push_back(0.f);		// SK_05
	m_vecSK_Cooltime.push_back(0.f);		// SK_06
	
	m_vecPoisonDMG_delay.push_back(0.f);
	m_vecPoisonDMG_delay.push_back(0.f);
	m_vecPoisonDMG_delay.push_back(0.f);
	m_vecPoisonDMG_delay.push_back(0.f);
	return S_OK;
}

_int CSoulEater::Update(const _float& fTimeDelta)
{
	KeyCheck();
	Sound(m_dwMyIndex);

	AttackPositionSetting();

	if(m_pTargetInfo != NULL)
		m_fCautionRange = 6.5f;
	else
		m_fCautionRange = 3.5f;

	if(!m_bInitInfo)
	{

		m_vOriPos = m_pInfo->m_vPos;
		//m_pInfo->m_vPos = _vec3(85, 0, 17);
		m_fOriAngle = m_pInfo->m_fAngle[Engine::ANGLE_Y];
		m_bInitInfo = true;
		m_dwIndex = Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos);

		m_pSkillRange = (CSkill_Range*)(Engine::Get_Scene()->GetObjList(LAYER_GAMELOGIC, L"Skill_Range_SoulBoss")->front());
		m_pSkillRange_1 = (CSkill_Range_1*)(Engine::Get_Scene()->GetObjList(LAYER_GAMELOGIC, L"Skill_Range_SoulBoss_1")->front());

	}

	
	if(m_bDieStart/*m_bDieState*/ && m_iAniIdx != CAIMgr::STATEBOSS_SK_3)
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
	if(m_iAniIdx != CAIMgr::STATEBOSS_DIE)
		CAIMgr::GetInstance()->BOSS_Pattern(&m_vecPlayersInfo, this, m_pCollision, fTimeDelta);

	 

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

void CSoulEater::Render(void)
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

void CSoulEater::AniFrameEnd(Engine::ANI_INFO* pData)
{
	if(m_pMesh->GetPeriod() - pData->fPeriod < m_pMesh->GetTrackPosition())
	{
		if(m_iAniIdx == CAIMgr::STATEBOSS_DIE)
		{
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
		
		m_iCoolTime_FullIndex = 7;
		m_bSkillEnd = true;
		m_bOnSkill_Range = false;


		m_bDMG_Timing = false;

		m_bHitState = false;
		m_bStunState = false;

		m_fGetHit_DMG = 0.f;

		if(m_bAttackEnd)
		{
			m_bAttackEnd = false;
			m_bAttack = false;
		}

		// 소환 스킬 숨기 모션 나감 
		// 소환 몹 다 죽으면 땅속에서 나오는 모션
		if(m_iAniIdx == CAIMgr::STATEBOSS_SK_3)
		{
			if(!m_bCall_Skill_State)
			{
				m_bDieState = false;
				m_bSkillEnd = false;
				m_iAniIdx = CAIMgr::STATEBOSS_SK_5;
				return;
			}
			else
			{			
				m_bDieState = true;
				m_bSkillEnd = false;
				m_fFrameSpeedCtr = 0.f;
				return;
			}
		}

		// 스킬 쿨타임 초기화
		if(m_iAniIdx == CAIMgr::STATEBOSS_SK_1)
			m_vecSK_Cooltime[0] = 0.f;
		else if(m_iAniIdx == CAIMgr::STATEBOSS_SK_2)
			m_vecSK_Cooltime[1] = 0.f;
		else if(m_iAniIdx == CAIMgr::STATEBOSS_SK_3)
			m_vecSK_Cooltime[2] = 0.f;
		else if(m_iAniIdx == CAIMgr::STATEBOSS_SK_5)
			m_vecSK_Cooltime[4] = 0.f;
		else if(m_iAniIdx == CAIMgr::STATEBOSS_SK_6)
			m_vecSK_Cooltime[5] = 0.f;


		m_bDownState = false;

		if(m_iAniIdx == CAIMgr::STATEBOSS_IDLE_P)
		{
			m_iAniIdx = CAIMgr::STATEBOSS_IDLE_P;
			m_iPrevAniIdx = CAIMgr::STATEBOSS_IDLE_P;
		}
		else
		{	
			if(m_iAniIdx != CAIMgr::STATEBOSS_RUN && m_iAniIdx != CAIMgr::STATEBOSS_WALK) 
			{
				m_iAniIdx = CAIMgr::STATEBOSS_IDLE_C;
				m_iPrevAniIdx = CAIMgr::STATEBOSS_IDLE_C;
				m_pMesh->SetAnimationSet(m_iAniIdx);
				m_pMesh->SetTrackPos(0.0);
			}
		}

		m_ePlayer_ACT_Type = ACT_DEFAULT;
	}
}

void CSoulEater::Skill_State(const _float& fTimeDelta)
{
	float fTrack = (float)m_pMesh->GetTrackPosition();

	if(m_bSkillState)
	{
		if(!m_bCall_Skill_State)  //소환 스킬 일땐 누적 금지
			m_vecSK_Cooltime[2] += fTimeDelta;	
		m_vecSK_Cooltime[0] += fTimeDelta;	
		m_vecSK_Cooltime[1] += fTimeDelta;	 
		m_vecSK_Cooltime[3] += fTimeDelta;	
		
		SkillAttack_Timing(fTrack, fTimeDelta);
		return;
	}

	// 데미지 처리는 매니져에서 
	 

	//// 스킬1	-- 공격 스킬 휘두르기
	if(m_vecSK_Cooltime[0] > 5.f && !m_bAttackEnd)
	{
		_vec3 vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
		_vec3 vMyPos = m_pInfo->m_vPos;

		_vec3 vLength = vTargetPos - vMyPos;	
		float fLength = D3DXVec3Length(&vLength);

		if(fLength < m_fCollisionRange)
		{
			m_iCoolTime_FullIndex = 0;

			m_bSkillState = true;
			m_iAniIdx = CAIMgr::STATEBOSS_SK_1;	

			//m_ePlayer_ACT_Type = ACT_HIT_UP;
		}
		return; 
	}
	else
	{	
		if(!m_bSkillState)
			m_vecSK_Cooltime[0] += fTimeDelta;	
	}


	//// 범위 스킬 
	if(m_vecSK_Cooltime[1] > 15.f && !m_bAttackEnd)
	{	
		m_iCoolTime_FullIndex = 1;

		m_bSkillState = true;
		m_iAniIdx = CAIMgr::STATEBOSS_SK_2;	

		if(!m_bOnSkill_Range)
		{
			// 플레이어에게 스킬 범위를 보냄
			vector<CUnits*>::iterator iterInfo = CAIMgr::GetInstance()->Get_player_NPC_Info()->begin();
			vector<CUnits*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_player_NPC_Info()->end();

			for(; iterInfo != iterInfo_end; ++iterInfo)
			{
				if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
					((CPlayers*)(*iterInfo))->Set_RangeSkill_Check(m_pInfo->m_vPos, true, 4.f);
			}

			m_pSkillRange->Start_SkillRange(m_pSkillRange_1, m_pInfo->m_vPos, 4.f, 0.065f);
			m_pSkillRange_1->Start_SkillRange(m_pInfo->m_vPos, 0.2f, 4.f, 0.065f);
			m_bOnSkill_Range = true;	
		}

		return;
	}
	else
	{
		if(!m_bSkillState)
			m_vecSK_Cooltime[1] += fTimeDelta;
	}

	//// 소환 숨기 스킬
	if(m_vecSK_Cooltime[2] > 7.f && !m_bAttackEnd)
	{
		m_iCoolTime_FullIndex = 2;

		_vec3 vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
		_vec3 vMyPos = m_pInfo->m_vPos;

		_vec3 vLength = vTargetPos - vMyPos;	
		float fLength = D3DXVec3Length(&vLength);
	
		m_bSkillState = true;
		m_iAniIdx = CAIMgr::STATEBOSS_SK_3;	
		return;
	}
	else
	{	
		/*if(!m_bSkillState)
			m_vecSK_Cooltime[2] += fTimeDelta;	*/

		if(!m_bCall_Skill_State)	// 소환 몬스터 모두 죽기 전까지 증가 안함
			m_vecSK_Cooltime[2] += fTimeDelta;	
		else
		{
			vector<CMonsters*>::iterator iter = m_vecSummon_Monster.begin();
			vector<CMonsters*>::iterator iter_end = m_vecSummon_Monster.end();

			int iSize = m_vecSummon_Monster.size();
			int i = 0;
			// 몬스터가 모두 죽었는지 개수 체크 
			for(; iter != iter_end; ++iter)
			{
				if((*iter)->Get_bDieState())
					++i;		
			}
			// 소환 몬스터 모두 죽으면 쿨타임 활성화
			if(i == iSize)
			{
				m_bCall_Skill_State = false;	 
			}
		}
	}
}

void CSoulEater::KeyCheck(void)
{

}

void CSoulEater::SkillAttack_Timing(float fTrack, float fTimeDelta)
{
	switch(m_iAniIdx)
	{
	case CAIMgr::STATEBOSS_SK_1:
		{
			if(m_bOnSkill_Range)
				return;

			float fTiming_1 = m_vecSkill_DMG_Timing[0]->fInterval_1;
			float fTiming_2 = m_vecSkill_DMG_Timing[0]->fInterval_2;
			
			if(fTrack > fTiming_1 && fTrack < fTiming_2)
			{
				m_bOnSkill_Range = true;
				float fDamege = (_float)CRandom_Number::Get_RandomNum(3,3);
				((CPlayers*)m_pTargetInfo)->Set_DamageType((CPlayers::ACT_STATE)m_ePlayer_ACT_Type, fDamege, Engine::CStatus::DMG_MELEE);			
			}
			return;
		}

	case CAIMgr::STATEBOSS_SK_2:
		{
			float fTiming_1 = m_vecSkill_DMG_Timing[1]->fInterval_1;
			float fTiming_2 = m_vecSkill_DMG_Timing[1]->fInterval_2;

			vector<int> vecOverlap; 
			int iRand_Z = 0;
			int iSkillTime = 0;
			if(fTrack > fTiming_1 && fTrack < fTiming_2)
			{
				vector<int> vecOverlap; 
				int iRand_Z = 0;
				int iSkillTime = 0;

				Engine::Call_MonsterFX(Engine::SPIDER_SMOKE, &m_pInfo->m_vPos, false);

				m_vecSkillPos.clear();
				for(int i = 0; i < 60; ++i)		
				{ 
					_vec3 vSkillPos= m_pInfo->m_vPos;

					int iRand_X = 0;
					iRand_Z = rand() % 600;

					while(true)      // X 좌표 중복 검사
					{
						iRand_X = rand() % 600;
						vector<int>::iterator iterOverlap = vecOverlap.begin();
						vector<int>::iterator iterOverlap_end = vecOverlap.end();

						for(; iterOverlap != iterOverlap_end; ++iterOverlap)
						{
							if((*iterOverlap) == iRand_X)
								break;                  
						} 

						if(iterOverlap == iterOverlap_end)
						{
							// 중복이 아니라면 임시 저장 후 저장된 것으로 중복체크함
							vecOverlap.push_back(iRand_X);
							break;
						}
					}

					vSkillPos.x = m_pInfo->m_vPos.x + (iRand_X / 100.f - 3);
					vSkillPos.z = m_pInfo->m_vPos.z + (iRand_Z / 100.f - 3);

					m_vecSkillPos.push_back(vSkillPos);

					if(i % 2 != 0)
						vSkillPos.y += 0.01f;
					else
						vSkillPos.y += 0.008f;

					Engine::Call_MonsterFX(Engine::SPIDER_SMOKE_FLOOR, &m_vecSkillPos[i], false);
				}	
			}

			if(m_bOnSkill_Range)
			{
				vector<CPlayers*>::iterator iterInfo = CAIMgr::GetInstance()->Get_PlayerInfo()->begin();
				vector<CPlayers*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_PlayerInfo()->end();

				int iCnt = 0;
				for(; iterInfo != iterInfo_end; ++iterInfo)
				{		
					_vec3 vTargetPos = (*iterInfo)->GetInfo()->m_vPos;
					_vec3 vMyPos = m_pInfo->m_vPos;

					_vec3 vLength = vTargetPos - vMyPos;	
					float fLength = D3DXVec3Length(&vLength);

					if(4.f > fLength)
					{
						if(m_vecPoisonDMG_delay[iCnt] > 1.f)
						{
							float fDamege = (_float)CRandom_Number::Get_RandomNum(1,3);
							
							if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
							{
								((*iterInfo))->Set_DamageType((CPlayers::ACT_STATE)m_ePlayer_ACT_Type, fDamege, Engine::CStatus::DMG_MELEE);			
							}
							else
							{		
								//const_cast<Engine::CStatus*>((((CNPCs*)(*iterInfo)))->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
								//Engine::CGameObject* pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(*iterInfo))->GetInfo()->m_vPos, fDamege);							 
								//Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);
							}

							m_vecPoisonDMG_delay[iCnt] = 0.f;
						}
						else
							m_vecPoisonDMG_delay[iCnt] += fTimeDelta;
					}
					else
						m_vecPoisonDMG_delay[iCnt] = 0.f;

					++iCnt;
				}
			}

			return;
		}
	case CAIMgr::STATEBOSS_SK_3:
		{
			if(m_bCall_Skill_State)
			{		 
				vector<CMonsters*>::iterator iter = m_vecSummon_Monster.begin();
				vector<CMonsters*>::iterator iter_end = m_vecSummon_Monster.end();

				int iSize = m_vecSummon_Monster.size();
				int i = 0;
				// 몬스터가 모두 죽었는지 개수 체크 
				for(; iter != iter_end; ++iter)
				{
					if((*iter)->Get_bDieState())
						++i;		
				}
				// 소환 몬스터 모두 죽으면 쿨타임 활성화
				if(i == iSize)
				{
					m_fFrameSpeedCtr = 1.f;
					m_bCall_Skill_State = false;	 
				}

				return;
			}

			vector<CMonsters*>::iterator iter = m_vecSummon_Monster.begin();
			vector<CMonsters*>::iterator iter_end = m_vecSummon_Monster.end();

			float fTiming_1 = m_vecSkill_DMG_Timing[2]->fInterval_1;
			float fTiming_2 = m_vecSkill_DMG_Timing[2]->fInterval_2;

			vector<int> vecOverlap; 
			int iRand_Z = 0;
			int iSkillTime = 0;
			if(fTrack > fTiming_1 && fTrack < fTiming_2)
			{
				m_bCall_Skill_State = true;

				for(; iter != iter_end; ++iter)		
				{
					Engine::CTransform* pInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>((*iter)->GetComponent(L"Transform")));
					Engine::CStatus* pStatus = dynamic_cast<Engine::CStatus*>(const_cast<Engine::CComponent*>((*iter)->GetComponent(L"Com_Status")));

					int iRand_X = 0;
					iRand_Z = rand() % 200;

					while(true)      // X 좌표 중복 검사
					{
						iRand_X = rand() % 200;
						vector<int>::iterator iterOverlap = vecOverlap.begin();
						vector<int>::iterator iterOverlap_end = vecOverlap.end();

						for(; iterOverlap != iterOverlap_end; ++iterOverlap)
						{
							if((*iterOverlap) == iRand_X)
								break;                  
						} 

						if(iterOverlap == iterOverlap_end)
						{
							// 중복이 아니라면 임시 저장 후 저장된 것으로 중복체크함
							vecOverlap.push_back(iRand_X);
							break;
						}
					}

					pInfo->m_vPos.x = m_pInfo->m_vPos.x + (iRand_X / 100.f - 1);
					pInfo->m_vPos.z = m_pInfo->m_vPos.z + (iRand_Z / 100.f - 1);

					// 임시
					pStatus->m_tRealStat.m_fCurHp = pStatus->m_tRealStat.m_fMaxHp;
					(*iter)->Set_DMG_Check(0.f);
					(*iter)->Set_FrameSpeedCtr(1.f);
					(*iter)->SetDead(false);
					(*iter)->Set_bDieState(false);
					 
					(*iter)->Set_AniIdx(CAIMgr::STATE_IDLE_P);

					(*iter)->Set_bDieStart(false);

					iSkillTime = rand() % 10 + 5;
					(*iter)->Set_SkillTime((float)iSkillTime);	
				}	
			}
			return;
		}
	case CAIMgr::STATEBOSS_SK_5:
		{
			return;
		}

	}
	return;
}

HRESULT CSoulEater::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_SoulEater");
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
	pBaseStat->m_fHealth = 300.f;
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

void CSoulEater::Set_ContantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CSoulEater* CSoulEater::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSoulEater*		pGameObject = new CSoulEater(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSoulEater::Free(void)
{
	Engine::CGameObject::Free();
	CUnits::Free();

	m_vecPlayersInfo.clear();
	m_vecSK_Cooltime.clear();
	m_vecPoisonDMG_delay.clear();
	m_vecSkillPos.clear();
	m_vecSummon_Monster.clear();

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

void CSoulEater::Sound(DWORD dwMyIndex)
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
		case CAIMgr::STATEBOSS_IDLE_P:
			break;
		case CAIMgr::STATEBOSS_IDLE_C:
			break;
		case CAIMgr::STATEBOSS_WALK:
			break;
		case CAIMgr::STATEBOSS_RUN:
			break;
		case CAIMgr::STATEBOSS_ATK_1:
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
				CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterAtt1.ogg", fSoundVolume);
			break;
		case CAIMgr::STATEBOSS_ATK_2:	
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
				CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterAtt2.ogg", fSoundVolume);
			break;
		case CAIMgr::STATEBOSS_DMG_F:	

			break;
		case CAIMgr::STATEBOSS_STUN:		
			
			break;
		case CAIMgr::STATEBOSS_DIE:		
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
				CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterDead.ogg", fSoundVolume);
			break;
		case CAIMgr::STATEBOSS_SK_1:
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
				CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterAtt1.ogg", fSoundVolume);
			break;
		case CAIMgr::STATEBOSS_SK_2:
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
				CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterSkill.ogg", fSoundVolume);
			break;
		case CAIMgr::STATEBOSS_SK_3:	
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
				CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterDig.ogg", fSoundVolume);
			break;
		case CAIMgr::STATEBOSS_SK_4:	
		
			break;
		case CAIMgr::STATEBOSS_SK_5:	
		
			break;
		case CAIMgr::STATEBOSS_SK_6:	
		
			break;
		case CAIMgr::STATEBOSS_SK_7:	
			
			break;
	}

	return;
}
