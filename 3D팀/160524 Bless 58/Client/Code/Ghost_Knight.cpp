#include "stdafx.h"
#include "Ghost_Knight.h"

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
#include "SoundMgr.h"
#include "MainPlayer.h"

#include "MonsterDmg.h"
#include "NPCs.h"

CGhost_Knight::CGhost_Knight(LPDIRECT3DDEVICE9 pDevice)
: CMonsters(pDevice)
, m_bPush(false)
, m_bBossCam(false)
, m_fBossCamTime(0.f)
, m_bBossCamSK1(false)
, m_bBossCamEff(true)
{
	m_bTest222 = false;
}

CGhost_Knight::~CGhost_Knight(void)
{

}

HRESULT CGhost_Knight::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	for(int i = 0; i < 15; ++i)
		m_fExploAngle[i] = 0.f;

	m_bBoss_Type = true;
	m_eUnitType = CUnits::TYPEID_BOSS;

	m_pInfo->m_vScale = _vec3(0.015f, 0.015f, 0.015f);

	m_fCautionRange = 5.f;
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
	pSK_Timing->fInterval_1 = 3.f;
	pSK_Timing->fInterval_2 = 3.7f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	// SK_02
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 3.f;
	pSK_Timing->fInterval_2 = 3.7f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);

	// SK_03
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 3.f;
	pSK_Timing->fInterval_2 = 3.7f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);
	// SK_03
	pSK_Timing = new ATTACK_DMG_TIMIMG;
	pSK_Timing->fInterval_1 = 6.2f;
	pSK_Timing->fInterval_2 = 6.5f;
	m_vecSkill_DMG_Timing.push_back(pSK_Timing);
	
	m_vecSK_Cooltime.push_back(0.f);		// SK_01
	m_vecSK_Cooltime.push_back(0.f);		// SK_02
	m_vecSK_Cooltime.push_back(0.f);		// SK_03
	m_vecSK_Cooltime.push_back(0.f);		// SK_04


	for(int i = 0; i < 15; ++i)
	{
		float fAngle = 24.f * i;
		m_vecSkillAngle.push_back(fAngle);
	}


	for(int i = 0; i < 20; ++i)
	{
		METEOIFNO* pInfo = new METEOIFNO;
		
		pInfo->vPos = _vec3(0.f, 0.f, 0.f);
		pInfo->bCheck = false;
		pInfo->fTime = 0.5f * i;
		m_vecMeteoInfo.push_back(pInfo);
	}

	m_fTesttime = 0.f;

	return S_OK;
}

_int CGhost_Knight::Update(const _float& fTimeDelta)
{
	AttackPositionSetting();
	Sound(m_dwMyIndex);
	if(m_pTargetInfo != NULL)
		m_fCautionRange = 50.f;
	else
		m_fCautionRange = 3.5f;


	if(m_bTest222)
	{
		vector<METEOIFNO*>::iterator itermeteo = m_vecMeteoInfo.begin();
		vector<METEOIFNO*>::iterator itermeteo_end = m_vecMeteoInfo.end();

		int i = 0;
		for(; itermeteo != itermeteo_end; ++itermeteo)
		{
			if(!(*itermeteo)->bCheck)
			{
				if(m_fTesttime > (*itermeteo)->fTime)
				{
					(*itermeteo)->bCheck = true;
					//m_fTesttime = 0.f;
					Engine::Call_MonsterFX(Engine::SKULL_METEO, &(*itermeteo)->vPos, false);
					if(i==1)
						CSoundMgr::GetInstance()->GhostKnightSound2(L"GhostKnightEffect5.ogg");
				}
				else
					m_fTesttime += fTimeDelta;
			}
			else
				++i;
		}

		if(i == m_vecMeteoInfo.size())
		{
			itermeteo = m_vecMeteoInfo.begin();
			itermeteo_end = m_vecMeteoInfo.end();
				
			m_bTest222 = false;

			for(; itermeteo != itermeteo_end; ++itermeteo)
				(*itermeteo)->bCheck = false;
			m_fTesttime = 0.f;
			
		}
	}

	
	KeyCheck();
	m_vOraPos = m_pInfo->m_vPos + m_pInfo->m_vDir * 2.f;

	if(!m_bInitInfo)
	{
		m_vOriPos = m_pInfo->m_vPos;
		//m_pInfo->m_vPos = _vec3(85, 0, 17);
		m_fOriAngle = m_pInfo->m_fAngle[Engine::ANGLE_Y];
		m_bInitInfo = true;
		m_dwIndex = Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos);

		m_pSkillRange = (CSkill_Range*)(Engine::Get_Scene()->GetObjList(LAYER_GAMELOGIC, L"CSkill_Range")->front());
		m_pSkillRange_1 = (CSkill_Range_1*)(Engine::Get_Scene()->GetObjList(LAYER_GAMELOGIC, L"CSkill_Range_1")->front());
	

		Engine::Call_MonsterFX(Engine::SKULL_ORA, &m_vOraPos, true);
	}

	if(m_bDieState)
		Engine::CEffect_Manager::GetInstance()->Off_MonsterFX(Engine::SKULL_ORA);

	if(m_bDieStart/*m_bDieState*/)
	{
		if(m_fDie_VanishTime < 3.f)
		{
			m_fDie_VanishTime += fTimeDelta;
		}
		else
		{
			/*if(m_pInfo->m_vPos.y > m_pTargetInfo->GetInfo()->m_vPos.y - 0.2f)
				m_pInfo->m_vPos.y -= 0.001f;
			else
				m_bDead = true;*/

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

	if(m_bBossCam)
	{
		ActionCamMove(fTimeDelta);
	}
	else
	{
		// AI 작동
		m_fFrameSpeedCtr = 1.f;
		if(m_iAniIdx != CAIMgr::STATEBOSS_DIE)
			CAIMgr::GetInstance()->BOSS_Pattern(&m_vecPlayersInfo, this, m_pCollision, fTimeDelta);
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

void CGhost_Knight::Render(void)
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

void CGhost_Knight::Skill_State(const _float& fTimeDelta)
{ 
	float fTrack = (float)m_pMesh->GetTrackPosition();

	/*if(m_iAniIdx != CAIMgr::STATEBOSS_SK_1)*/
 
	//if(m_iAniIdx != CAIMgr::STATEBOSS_SK_1 && m_iAniIdx != CAIMgr::STATEBOSS_SK_2 && m_iAniIdx != CAIMgr::STATEBOSS_SK_3 && 
	//	m_iAniIdx != CAIMgr::STATEBOSS_SK_4 && m_iAniIdx != CAIMgr::STATEBOSS_SK_5 && m_iAniIdx != CAIMgr::STATEBOSS_SK_6 && m_iAniIdx != CAIMgr::STATEBOSS_SK_7)
	//{
	//	if(m_bAttackEnd || m_bSkillState)
	//	{
	//		if(!m_bCall_Skill_State)
	//			m_vecSK_Cooltime[0] += fTimeDelta;	
	//		m_vecSK_Cooltime[1] += fTimeDelta;	
	//		m_vecSK_Cooltime[2] += fTimeDelta;	 
	//		m_vecSK_Cooltime[3] += fTimeDelta;	
	//		return;
	//	}
	//}

	if(m_bSkillState)
	{
		if(!m_bCall_Skill_State)  //소환 스킬 일땐 누적 금지
			m_vecSK_Cooltime[1] += fTimeDelta;	
		m_vecSK_Cooltime[0] += fTimeDelta;	
		m_vecSK_Cooltime[2] += fTimeDelta;	 
		m_vecSK_Cooltime[3] += fTimeDelta;	


		switch(m_iAniIdx)
		{
		case CAIMgr::STATEBOSS_SK_1:
			{
				if(m_bCall_Skill_State)
					return;
				
				vector<CMonsters*>::iterator iter = m_vecCall_Monster.begin();
				vector<CMonsters*>::iterator iter_end = m_vecCall_Monster.end();

				float fTiming_1 = m_vecSkill_DMG_Timing[0]->fInterval_1;
				float fTiming_2 = m_vecSkill_DMG_Timing[0]->fInterval_2;

				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					m_bCall_Skill_State = true;

					for(; iter != iter_end; ++iter)		
					{
						Engine::CTransform* pInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>((*iter)->GetComponent(L"Transform")));

						Engine::CStatus* pStatus = dynamic_cast<Engine::CStatus*>(const_cast<Engine::CComponent*>((*iter)->GetComponent(L"Com_Status")));

						// 임시
						pStatus->m_tRealStat.m_fCurHp = pStatus->m_tRealStat.m_fMaxHp;
						(*iter)->Set_DMG_Check(0.f);
						(*iter)->Set_FrameSpeedCtr(1.f);
						(*iter)->SetDead(false);
						(*iter)->Set_bDieState(false);
						(*iter)->Set_bMonster_Wake(true);
						(*iter)->Set_AniIdx(CAIMgr::STATE_GET_UP);

						(*iter)->Set_bDieStart(false);

						_vec3 vPos = pInfo->m_vPos;
						vPos.y += 0.06f;
						Engine::Call_MonsterFX(Engine::SKULL_SUMMON_TARGET, &vPos, false);
					}
					
					_vec3 vPos2 =  m_pInfo->m_vPos;
					vPos2.y += 0.06f;
					Engine::Call_MonsterFX(Engine::SKULL_SUMMON, &vPos2, false);
					CSoundMgr::GetInstance()->GhostKnightSound1(L"GhostKnightEffect1.ogg");
				}
				return;
			}

		case CAIMgr::STATEBOSS_SK_2:
			{
				_vec3 vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
				_vec3 vMyPos = m_pInfo->m_vPos;

				_vec3 vLength = vTargetPos - vMyPos;	
				float fLength = D3DXVec3Length(&vLength);
				
				
				float fTiming_1 = m_vecSkill_DMG_Timing[1]->fInterval_1;
				float fTiming_2 = m_vecSkill_DMG_Timing[1]->fInterval_2;

				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{

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
								{
									((CPlayers*)(*iterInfo))->Set_DamageType((CPlayers::ACT_STATE)m_ePlayer_ACT_Type, fDamege, Engine::CStatus::DMG_MELEE);										
								}
								else
								{
									const_cast<Engine::CStatus*>((((CNPCs*)(*iterInfo))->Get_TargetInfo())->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
									Engine::CGameObject* pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(*iterInfo))->GetInfo()->m_vPos, fDamege);							 
									Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);
								}
							}
						}

						for(int i = 0; i < 15; ++i)
						{
							_mymat matRot;						
							
							m_fExploAngle[i] = m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(( i * 24.f));
							matRot.RotY(D3DXToRadian(( i * 24.f)));
							D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 0.f, 0.f);

							D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRot);
							m_vExploPos[i] = m_pInfo->m_vPos + vDir * 20.f;
							Engine::Call_MonsterFX(Engine::SKULL_FIREEXPLO2, &m_vExploPos[i], false, NULL, &m_fExploAngle[i]);
							if(i == 0)
								CSoundMgr::GetInstance()->GhostKnightSound1(L"GhostKnightEffect2.ogg");
						}
						 
					}	 
				}
				return;
			}
		case  CAIMgr::STATEBOSS_SK_5:
			{
				//_vec3 vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
				//_vec3 vMyPos = m_pInfo->m_vPos;
				//_vec3 vDir = vTargetPos - vMyPos;
				//_vec3 vMyDir = m_pInfo->m_vDir;

				//D3DXVec3Normalize(&vDir, &vDir);
				//D3DXVec3Normalize(&vMyDir, &vMyDir);

				//float fDot = D3DXVec3Dot(&vDir, &vMyDir);
				//float fAngle = D3DXToDegree(acosf(fDot));

				//if(fAngle < 10.f)
				//{
				//	//m_bSkillState = true;
				//	//m_iAniIdx = CAIMgr::STATEBOSS_SK_5;	
				//}
				//return;
				float fTiming_1 = m_vecSkill_DMG_Timing[2]->fInterval_1;
				float fTiming_2 = m_vecSkill_DMG_Timing[2]->fInterval_2;

				vector<int> vecOverlap; 
				int iRand_Z = 0;
				int iSkillTime = 0;
				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					m_bTest222 = true;
					vector<METEOIFNO*>::iterator itermeteo = m_vecMeteoInfo.begin();
					vector<METEOIFNO*>::iterator itermeteo_end = m_vecMeteoInfo.end();

					for(; itermeteo != itermeteo_end; ++itermeteo)
					{
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

						_vec3 vPos = m_pInfo->m_vPos;
						//vPos.x = /*m_pInfo->m_vPos.x*/ + (iRand_X / 100.f - 1);
						//vPos.z = /*m_pInfo->m_vPos.z*/ + (iRand_Z / 100.f - 1);

						vPos.x = m_pTargetInfo->GetInfo()->m_vPos.x + (iRand_X / 100.f - 1);
						vPos.z = m_pTargetInfo->GetInfo()->m_vPos.z + (iRand_Z / 100.f - 1);
						vPos.y += 0.06f;
						(*itermeteo)->vPos = vPos;
					}
				}
				return;


			}
		case CAIMgr::STATEBOSS_SK_6:
			{	
				_vec3 vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
				_vec3 vMyPos = m_pInfo->m_vPos;

				_vec3 vLength = vTargetPos - vMyPos;	
				float fLength = D3DXVec3Length(&vLength);


				float fTiming_1 = m_vecSkill_DMG_Timing[3]->fInterval_1;
				float fTiming_2 = m_vecSkill_DMG_Timing[3]->fInterval_2;

				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					if(!m_bDMG_Timing)
					{
						m_ePlayer_ACT_Type = ACT_STUN;
						m_bDMG_Timing = true;			 

						vector<CUnits*>::iterator iterInfo = CAIMgr::GetInstance()->Get_player_NPC_Info()->begin();
						vector<CUnits*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_player_NPC_Info()->end();

						for(; iterInfo != iterInfo_end; ++iterInfo)
						{		
							vTargetPos = (*iterInfo)->GetInfo()->m_vPos;
							vMyPos = m_pInfo->m_vPos;

							vLength = vTargetPos - vMyPos;	
							fLength = D3DXVec3Length(&vLength);

							if(2.f > fLength)
							{
								float fDamege = (_float)CRandom_Number::Get_RandomNum(5,3);

								if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
								{
									((CPlayers*)(*iterInfo))->Set_DamageType((CPlayers::ACT_STATE)m_ePlayer_ACT_Type, fDamege, Engine::CStatus::DMG_MELEE);										
								}
								else
								{
									const_cast<Engine::CStatus*>((((CNPCs*)(*iterInfo))->Get_TargetInfo())->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
									Engine::CGameObject* pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(*iterInfo))->GetInfo()->m_vPos, fDamege);							 
									Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);
								}
							}
						}

						Engine::Call_MonsterFX(Engine::SKULL_EXPLO, &m_pInfo->m_vPos, false);

						for(int i = 0; i < 15; ++i)
						{
							
							Engine::Call_MonsterFX(Engine::SKULL_THORN, &m_pInfo->m_vPos, false, NULL, &m_vecSkillAngle[i]);
							if(i == 0)
								CSoundMgr::GetInstance()->GhostKnightSound1(L"GhostKnightEffect6.ogg");
						}
					}
				}
				return;
			}
		}
		return;
	}

	// 데미지 처리는 매니져에서 
	// 소환 스킬 
	
	if(m_vecSK_Cooltime[0] > 5.f && !m_bAttackEnd && !m_bCall_Skill_State)
	{
		m_bSkillState = true;
		m_iAniIdx = CAIMgr::STATEBOSS_SK_1;	

		//m_iEnforce_Sk_Idx = m_iAniIdx;
		return;
	}
	else
	{
		if(!m_bCall_Skill_State)	// 소환 몬스터 모두 죽기 전까지 증가 안함
			m_vecSK_Cooltime[0] += fTimeDelta;	
		
		else
		{
			vector<CMonsters*>::iterator iter = m_vecCall_Monster.begin();
			vector<CMonsters*>::iterator iter_end = m_vecCall_Monster.end();
		
			int iSize = m_vecCall_Monster.size();
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

	// 범위 스킬1	-- 점프해서 내려찍기
	if(m_vecSK_Cooltime[1] > 20.f && !m_bAttackEnd)
	{
		m_iCoolTime_FullIndex = 1; 

		_vec3 vTargetPos = m_pTargetInfo->GetInfo()->m_vPos;
		_vec3 vMyPos = m_pInfo->m_vPos;

		_vec3 vLength = vTargetPos - vMyPos;	
		float fLength = D3DXVec3Length(&vLength);

		if(1.f > fLength)
		{
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
						((CPlayers*)(*iterInfo))->Set_RangeSkill_Check(m_pInfo->m_vPos, true, 2.f);
				}

				m_pSkillRange->Start_SkillRange(m_pSkillRange_1, m_pInfo->m_vPos, 2.f, 0.015f);
				m_pSkillRange_1->Start_SkillRange(m_pInfo->m_vPos, 0.2f, 2.f, 0.015f);

				m_bOnSkill_Range = true;
			}
		}
		return;
	}
	else
	{	
		if(!m_bSkillState)
			m_vecSK_Cooltime[1] += fTimeDelta;	
	}

	//// 범위 스킬2	-- 블랙홀
	if(m_vecSK_Cooltime[2] > 12.f && !m_bAttackEnd)
	{
		m_iCoolTime_FullIndex = 2;

		m_bSkillState = true;
		m_iAniIdx = CAIMgr::STATEBOSS_SK_5;	

		if(!m_bOnSkill_Range)
		{
			// 플레이어에게 스킬 범위를 보냄
			vector<CUnits*>::iterator iterInfo = CAIMgr::GetInstance()->Get_player_NPC_Info()->begin();
			vector<CUnits*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_player_NPC_Info()->end();

			for(; iterInfo != iterInfo_end; ++iterInfo)
			{
				if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
					((CPlayers*)(*iterInfo))->Set_RangeSkill_Check(m_pTargetInfo->GetInfo()->m_vPos, true, 1.f);
			}

			m_pSkillRange->Start_SkillRange(m_pSkillRange_1, m_pTargetInfo->GetInfo()->m_vPos, 1.f, 0.008f);
			m_pSkillRange_1->Start_SkillRange(m_pTargetInfo->GetInfo()->m_vPos, 0.2f, 1.f, 0.008f);

			Engine::Call_MonsterFX(Engine::SKULL_SHIELD, &m_pInfo->m_vPos, true);
			

			m_bOnSkill_Range = true;
		}


		return;
	}
	else
	{	
		if(!m_bSkillState)
			m_vecSK_Cooltime[2] += fTimeDelta;	
	}
	
	// 범위 스킬3	-- 큰 범위 스킬
	
	if(m_vecSK_Cooltime[3] > 23.f && !m_bAttackEnd)
	{
		m_iCoolTime_FullIndex = 3;

		m_bSkillState = true;
		m_iAniIdx = CAIMgr::STATEBOSS_SK_6;	

		if(!m_bOnSkill_Range)
		{
			vector<CUnits*>::iterator iterInfo = CAIMgr::GetInstance()->Get_player_NPC_Info()->begin();
			vector<CUnits*>::iterator iterInfo_end = CAIMgr::GetInstance()->Get_player_NPC_Info()->end();

			for(; iterInfo != iterInfo_end; ++iterInfo)
			{
				if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
					((CPlayers*)(*iterInfo))->Set_RangeSkill_Check(m_pInfo->m_vPos, true, 2.f);
			}

			m_pSkillRange->Start_SkillRange(m_pSkillRange_1, m_pInfo->m_vPos, 2.f, 0.0085f);
			m_pSkillRange_1->Start_SkillRange(m_pInfo->m_vPos, 0.5f, 2.f, 0.0085f);
			m_bOnSkill_Range = true;
		}
		return;
	}
	else
	{	
		if(!m_bSkillState)
			m_vecSK_Cooltime[3] += fTimeDelta;	
	}
}

void CGhost_Knight::AniFrameEnd(Engine::ANI_INFO* pData)
{
	if(m_pMesh->GetPeriod() - pData->fPeriod < m_pMesh->GetTrackPosition())
	{
		if(m_iAniIdx == CAIMgr::STATEBOSS_DIE)
		{
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
		
		if(m_bSkillState == true)
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

		// 스킬 쿨타임 초기화
		if(m_iAniIdx == CAIMgr::STATEBOSS_SK_1)
			m_vecSK_Cooltime[0] = 0.f;
		else if(m_iAniIdx == CAIMgr::STATEBOSS_SK_2)
			m_vecSK_Cooltime[1] = 0.f;
		else if(m_iAniIdx == CAIMgr::STATEBOSS_SK_5)
			m_vecSK_Cooltime[2] = 0.f;
		else if(m_iAniIdx == CAIMgr::STATEBOSS_SK_6)
			m_vecSK_Cooltime[3] = 0.f;

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
 

HRESULT CGhost_Knight::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Ghost_Knight");
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

	pBaseStat->m_fHealth = 450.f;
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


void CGhost_Knight::Set_ContantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CGhost_Knight* CGhost_Knight::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGhost_Knight*		pGameObject = new CGhost_Knight(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CGhost_Knight::Free(void)
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

void CGhost_Knight::KeyCheck(void)
{
	//if(CKeyMgr::GetInstance()->GetKeyDown('Z'))
	//{
	//	m_iAniIdx = CAIMgr::STATEBOSS_SK_1;
	//}
}

void CGhost_Knight::Sound(DWORD dwMyIndex)
{
	float fTrack = (float)m_pMesh->GetTrackPosition();
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
		/*if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterAtt1.ogg");*/
		break;
	case CAIMgr::STATEBOSS_ATK_2:	
		/*if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			CSoundMgr::GetInstance()->GhostKnightSound2(L"SoulEaterSkill2.ogg");*/
		break;
	case CAIMgr::STATEBOSS_DMG_F:	
		break;
	case CAIMgr::STATEBOSS_STUN:
		break;
	case CAIMgr::STATEBOSS_DIE:		
		/*if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterDead.ogg", fSoundVolume);*/
		break;
	case CAIMgr::STATEBOSS_SK_1:
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->GhostKnightSound2(L"GhostKnightSkill1.ogg");
		break;
	case CAIMgr::STATEBOSS_SK_2:
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->GhostKnightSound2(L"GhostKnightSkill2.ogg");
		break;
	case CAIMgr::STATEBOSS_SK_3:	
		/*if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			CSoundMgr::GetInstance()->SoulEaterSound1(L"SoulEaterDig.ogg", fSoundVolume);*/
		break;
	case CAIMgr::STATEBOSS_SK_4:	
		break;
	case CAIMgr::STATEBOSS_SK_5:
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->GhostKnightSound2(L"GhostKnightSkill5.ogg");
		break;
	case CAIMgr::STATEBOSS_SK_6:	
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->GhostKnightSound2(L"GhostKnightSkill6.ogg");
		break;
	case CAIMgr::STATEBOSS_SK_7:	
		break;
	}
	return;
}

void CGhost_Knight::ActionCamMove(_float fTimeDelta)
{
	_vec3  vCenterPos = _vec3(89.282f, 1.509f, 154.901f);
	_myvec vLength = vCenterPos - m_pInfo->m_vPos;
	_float fLength = vLength.Length();
	m_fBossCamTime += fTimeDelta;

	if(fLength > 0.2f)
	{
		Set_AniIdx(CAIMgr::STATEBOSS_RUN);
		vLength.Normalize();
		vLength *= 0.017f;
		m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos, &vLength, m_dwIndex, 0.f);
		m_fBossCamTime = 0.f;
	}
	else
	{
		if(m_fBossCamTime < 3.f)
			Set_AniIdx(CAIMgr::STATEBOSS_IDLE_P);
		else if(m_fBossCamTime < 9.5f)
		{
			if(!m_bBossCamSK1)
			{
				CSoundMgr::GetInstance()->GhostKnightSound3(L"GhostKnightAppear.ogg");
				m_vecSK_Cooltime[3] = 10.f;
				m_pTargetInfo = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front();
			}

			if(m_fBossCamTime > 7.f)
			{
				m_fFrameSpeedCtr = 1.2f;
				if(m_bBossCamEff)
				{
					for(int i = 0; i < 15; ++i)
					{
						Engine::Call_MonsterFX(Engine::SKULL_THORN, &m_pInfo->m_vPos, false, NULL, &m_vecSkillAngle[i]);
						if(i ==0)
							CSoundMgr::GetInstance()->GhostKnightSound1(L"GhostKnightEffect5.ogg");
					}
					
					vector<CNPCs*>::iterator iternpc = CAIMgr::GetInstance()->Get_NPCInfo()->begin();
					vector<CNPCs*>::iterator iternpc_end = CAIMgr::GetInstance()->Get_NPCInfo()->end();

					for(iternpc; iternpc != iternpc_end; ++iternpc)
						const_cast<Engine::CStatus*>((*iternpc)->GetStatus())->m_tRealStat.m_fCurHp = 0.f;

					m_bBossCamEff = false;
				}
			}
			if(m_fBossCamTime < 9.5f)
			{
				Set_AniIdx(CAIMgr::STATEBOSS_SK_6);
				Skill_State(fTimeDelta);
			}
			else
				Set_AniIdx(CAIMgr::STATEBOSS_IDLE_P);
		}
		else if(m_fBossCamTime >= 9.5f)
		{
			if(m_fBossCamTime < 12.5f)
			{
				float fTrack = (float)m_pMesh->GetTrackPosition();
				Set_AniIdx(CAIMgr::STATEBOSS_SK_1);
				if(!m_bCall_Skill_State)
				{
					vector<CMonsters*>::iterator iter = m_vecCall_Monster.begin();
					vector<CMonsters*>::iterator iter_end = m_vecCall_Monster.end();

					float fTiming_1 = m_vecSkill_DMG_Timing[0]->fInterval_1 - 0.5f;
					float fTiming_2 = m_vecSkill_DMG_Timing[0]->fInterval_2 - 0.5f;

					if(fTrack > fTiming_1 && fTrack < fTiming_2)
					{
						m_bCall_Skill_State = true;

						for(; iter != iter_end; ++iter)		
						{
							Engine::CTransform* pInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>((*iter)->GetComponent(L"Transform")));

							Engine::CStatus* pStatus = dynamic_cast<Engine::CStatus*>(const_cast<Engine::CComponent*>((*iter)->GetComponent(L"Com_Status")));

							// 임시
							pStatus->m_tRealStat.m_fCurHp = pStatus->m_tRealStat.m_fMaxHp;
							(*iter)->Set_DMG_Check(0.f);
							(*iter)->Set_FrameSpeedCtr(1.f);
							(*iter)->SetDead(false);
							(*iter)->Set_bDieState(false);
							(*iter)->Set_bMonster_Wake(true);
							(*iter)->Set_AniIdx(CAIMgr::STATE_GET_UP);

							(*iter)->Set_bDieStart(false);

							_vec3 vPos = pInfo->m_vPos;
							vPos.y += 0.06f;
							Engine::Call_MonsterFX(Engine::SKULL_SUMMON_TARGET, &vPos, false);
						}

						_vec3 vPos2 =  m_pInfo->m_vPos;
						vPos2.y += 0.06f;
						Engine::Call_MonsterFX(Engine::SKULL_SUMMON, &vPos2, false);
						CSoundMgr::GetInstance()->GhostKnightSound1(L"GhostKnightEffect1.ogg");
					}
				}
			}
			else
				Set_AniIdx(CAIMgr::STATEBOSS_IDLE_P);
		}
	}

	CAIMgr::GetInstance()->BOSS_Change_State(this);

	if(!m_bBossCamSK1 && m_iAniIdx == CAIMgr::STATEBOSS_SK_6)
	{
		m_fFrameSpeedCtr = 1.5f;
		m_bBossCamSK1 = true;
	}

}