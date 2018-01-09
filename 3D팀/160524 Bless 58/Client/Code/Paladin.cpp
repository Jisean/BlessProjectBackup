#include "stdafx.h"
#include "Paladin.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Calculator.h"
#include "Material.h"
#include "Status.h"
#include "Scene.h"
#include "Stage.h"
#include "WyvernBrown.h"
#include "DynamicCamera.h"
#include "Sphere.h"
#include "Guardian.h"
#include "KeyMgr.h"
#include "Magician.h"
#include "Ranger.h"
#include "Transform.h"
#include "MainPlayer.h"
#include "AniCtrl.h"
#include "DynamicMesh.h"
#include <d3dx9anim.h>
#include "DmgView.h"
#include "SoundMgr.h"
#include "Stage2.h"
#include "QuestWin1.h"


CPaladin::CPaladin(LPDIRECT3DDEVICE9 pDevice,PLAYERJOB eTypeJob)
: CPlayers(pDevice)
, m_bPush(false)
, m_pSphere(NULL)
, m_pTargetStatus(NULL)
, m_vCenterPos(_vec3(0,0,0))
, m_fRangeHeal_Time(0.f)
, m_bRangeHeal(false)
, m_vHealPos(_vec3(0,0,0))
, m_fSaveAngle(0.f)
, m_vSaveDir(_vec3(0,0,0))
, m_bGuardianCol(false)
, m_wSubTarGet_Cnt(0)
, m_pSubTarGet(NULL)
, m_vSubPos(_vec3(0,0,0))
, m_fSubAngle(0.f)
, m_vPrevPos(_vec3(0,0,0))
, m_bDefenceBuff(false)
, m_vChoisePos(_vec3(0,0,0))
, m_wPrevTarget_Cnt(99)
, m_pMainPlayer(NULL)
{
	m_eTypeJob = eTypeJob;
	m_bCoolTimeSet.reset();
}

CPaladin::~CPaladin(void)
{
	m_pWyvern = NULL;
	m_pDynamicCamera = NULL;
	m_pMainPlayer = NULL;
	m_pTargetMonster = NULL;
	m_pSphere = NULL;
}


HRESULT CPaladin::Initialize(void)
{
	m_bActStateSet.reset();
	m_bAtkComboSet.reset();
	m_bKeyStateset.reset();
	m_bSkillSet.reset();
	m_bAIset.reset();
	m_bEventRootSet.reset();

	for(int i = 0; i < 3; ++i)
	m_fEventTime[i] = 0.f;

	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.01f, 0.01f, 0.01f);
	m_pInfo->m_vPos = _vec3(106.f, 0.f, 122.f);
	m_fRange = 25.f;

	m_dwIndex = 35;			/// 네비메쉬 인덱스
	m_fTargetDis = 1.8f;	/// 몬스터 타겟 잡는 거리
	m_pTargetMonster = NULL; /// 타겟 몬스터 포인터
	m_fTargetAngle = 0.f;
	m_bSlowCheck = false;
	m_fAttackSlow = 0.f;
	m_vFormation = _vec3(0,0,0);

	m_vQuestEnd_Start_Pos = _vec3(129.f,0.f,123.f);
	m_vQuestEnd_End_Pos = _vec3(134.f,21.f,11.f);


	// 위치잡기
	m_pInfo->m_vPos = _vec3(135.f,2.65f,130.f);
	m_dwIndex = 71;

	D3DXMatrixIdentity(&m_matSaddle);

	//////////////////////////////////////////////////////////////////////////  스킬 쿨타임 맥스치를 정해준다

	for(int i = 0; i < SKILL_END; ++i)
	{
		m_fSkillCooltime[i] = 0.f;
		m_fSkillCooltime_Max[i] = 0.f;
	}

	for(int i = 0; i < 5; ++i)
		m_fConsoleTime[i] = 0.f;

	m_fSkillCooltime_Max[SKILL_1] =	7.f;		/// 지정 힐
	m_fSkillCooltime_Max[SKILL_2] = 30.f;		/// 방어 버프
	m_fSkillCooltime_Max[SKILL_3] = 30.f;		/// 공격 버프
	m_fSkillCooltime_Max[SKILL_4] = 13.f;		/// 범위 힐
	m_fSkillCooltime_Max[SKILL_5] = 5.f;		/// 올려치기
	m_fSkillCooltime_Max[SKILL_DASH] = 4.f;	/// 대쉬어택
	m_fSkillCooltime_Max[SKILL_INNATE] = 10.f;	/// 방패막기
	m_fSkillCooltime_Max[SKILL_DEFAULT] = 1.f;  /// 기본공격

	return S_OK;
}

_int CPaladin::Update(const _float& fTimeDelta)
{
	if(m_bBossCam)
		m_fSpeed = 40.f;
	else if(m_fSpeed != 130.f)
		m_fSpeed = 130.f;

	m_pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front());
	if(m_pMainPlayer == NULL)
		__debugbreak();

	if(m_pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
	{
		if(*m_pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_MAIN && *m_pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_BEFORE)
			return 0;

		if(*m_pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_MAIN && *m_pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_COMPLETE)
		{

		}
	}

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &m_vLook, &m_pInfo->m_matWorld);

	m_pSoundTargetPlayer =	((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer();
	if(m_pSoundTargetPlayer == NULL)
		return 0;

	m_fSoundTargetDistance = abs(D3DXVec3Length(&m_pSoundTargetPlayer->GetInfo()->m_vPos) - D3DXVec3Length(&m_pInfo->m_vPos));
	m_fSoundVolume = -((m_fSoundTargetDistance/6.f) - 0.5f);


	m_fTimeDelta = fTimeDelta;


	// 몬스터의 공격 좌표 할당해줌 
	AttackPositionSetting();
	SKill_CoolTime();
	RangeHeal();
	//ConsoleDebug();

	if(false == m_bPush)
	{
		m_pSubPlayer[SUB_GUARDIAN] = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Guardian")->front();
		m_pSubPlayer[SUB_MAGICIAN] = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Magician")->front();
		m_pSubPlayer[SUB_RANGER] = (CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front();
		m_bPush = true;
	}


	//요거하면 지형탐(콤포넌트에 Calculator 추가)
	if(!m_bActStateSet.test(ACT_MOUNT))
	{
		if(TYPE_TERRAIN == Engine::CNaviMgr::GetInstance()->GetCellType(m_dwIndex))
		{
			m_fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
			m_pInfo->m_vPos.y = m_fHeight;
		}
	}
	if(NULL == m_pWyvern)
	{
		// 모든 플레이어는 와이번 객체 포인터를 가지고 있는다
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CWyvernBrown::Create(m_pDevice);
		m_pWyvern = ((CWyvernBrown*)pGameObject);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"WyvernBrown 생성 실패");	
		Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Wyvern_0",pGameObject);
	}
	else
		m_matSaddle = *(const_cast<D3DXMATRIX*>(((Engine::CDynamicMesh*)m_pWyvern->GetComponent(L"Mesh"))->FindFrame("Saddle")));

	/// 여기까지 함

	
	//if(m_pMainPlayer->m_bCtrlbitset.test(CMainPlayer::CTRL_F1))
	//if(m_pMainPlayer->m_bCtrlbitset.test(CMainPlayer::CTRL_F1))

	if(!m_bMainPlayer)
	{
		if(!m_pMainPlayer->m_bCtrlbitset.test(CMainPlayer::CTRL_F1))
			CPlayers::TargetSetting(this);
		else
			m_pTargetMonster = NULL;
	}
	else
		CPlayers::TargetSetting(this);

	if(m_eAI_StateID == AI_DIE)
	{
		if(m_pTargetMonster != NULL)
			m_pTargetMonster = NULL;

		m_bDead = true;
	}
	if(!m_bActStateSet.test(ACT_MOUNT))
	{
		if(m_bActStateSet.test(ACT_STUN))
		{
			if(!m_bActStateSet.test(ACT_DMG_HIT) &&
				!m_bActStateSet.test(ACT_HIT_UP))
			{
				m_eTrackID = CPlayers::STUN;
			}
		}
		if(m_bActStateSet.test(ACT_DMG_HIT))
		{
			if(!m_bActStateSet.test(ACT_STUN) &&
				!m_bActStateSet.test(ACT_HIT_UP))
			{
				m_eTrackID = CPlayers::DMG_F;
			}
		}
		if(m_bActStateSet.test(ACT_HIT_UP))
		{
			if(!m_bActStateSet.test(ACT_STUN) &&
				!m_bActStateSet.test(ACT_DMG_HIT))
			{
				m_eTrackID = CPlayers::HIT_UP_ALL;
			}
		}
	}

	if(m_bMainPlayer)
	{
		m_fTargetDis = 2.5f;
		ChoiseHeal();

		UpdateAnimation(m_eTrackID,fTimeDelta);
	}
	else
	{
		Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_TARGETSELECT);
		m_fTargetDis = 2.2f;
		// 여다가 AI 체크 던지면됨

		if(m_pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
		{
			if(*m_pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_MAIN && *m_pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_COMPLETE)
			{
				if(m_bEventEnd == false)
					Movement_Wyvern(m_pMainPlayer);
			}
		}
		if(!m_bEventRootSet.any())
		{
			if(!m_bActStateSet.test(ACT_MOUNT))
			{
				if(!m_bActStateSet.test(ACT_STUN) &&
					!m_bActStateSet.test(ACT_DMG_HIT) &&
					!m_bActStateSet.test(ACT_HIT_UP))
				{
					if(!EvasionRange())
					UpdateState_AI();
				}
			}
		}

		UpdateAnimation(m_eTrackID,fTimeDelta);
	}

	_int iResult = CUnits::Update(fTimeDelta);
	iResult = Engine::CGameObject::Update(fTimeDelta);

	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return iResult;
}

void CPaladin::Render(void)
{	
	if(m_bActStateSet.test(ACT_MOUNT))
	{
		_vec3 vSaddlePos = _vec3(m_matSaddle._41,m_matSaddle._42,m_matSaddle._43);
		D3DXVec3TransformCoord(&vSaddlePos,&vSaddlePos,&((Engine::CTransform*)m_pWyvern->GetComponent(L"Transform"))->m_matWorld);

		m_pInfo->m_vPos = vSaddlePos;
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = ((Engine::CTransform*)m_pWyvern->GetComponent(L"Transform"))->m_fAngle[Engine::ANGLE_Y]; 
		m_pInfo->m_fAngle[Engine::ANGLE_X] = ((Engine::CTransform*)m_pWyvern->GetComponent(L"Transform"))->m_fAngle[Engine::ANGLE_X]; 
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = ((Engine::CTransform*)m_pWyvern->GetComponent(L"Transform"))->m_fAngle[Engine::ANGLE_Z]; 

		m_pInfo->m_vPos.y = m_pInfo->m_vPos.y - 0.2f;
		m_pInfo->Update();

	}
	if(m_bCulled)
	{
		LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
		if(NULL == pEffect)
			return;

		CUnits::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

		pEffect->AddRef();

		// 스펙큘러 직접주기 - 혜은추가
		pEffect->SetFloat("g_fSpecMapping", 1.f);
		Set_ContantTable(pEffect);

		//pEffect->SetFloat("g_fSpecRange", m_fSpecRange);	
		//pEffect->SetFloat("g_fSpecPow", m_fSpecPower);	

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);

		//pEffect->SetFloat("g_fSpecRange", 0.7f);	// 범위 설정 값이 높을 수록 범위가 축소된다.
		//pEffect->SetFloat("g_fSpecPow", 0.25f);		// 파워 설정 값이 높을 수록 빛의 세기가 세진다.

		m_pMesh->RenderMesh_ForShader(pEffect);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

void CPaladin::UpdateAnimation(TRACKID eTrack ,float fTrackTime)
{
	m_eTrackID = eTrack;

	if(m_eTrackID != CPlayers::DASH)
	{
		m_fAcc = 0.f;
		m_fSlowTime = 1.f;
	}

	switch(m_eTrackID)
	{
	case CPlayers::ATK_0:
		Ani_Attack(0.f);
		Engine::Call_TrailFX(Engine::TRAIL_MACE);
		break;
	case CPlayers::ATK_1:
		Ani_Attack(0.f);
		break;
	case CPlayers::ATK_2:
		Ani_Attack(0.f);
		break;
	case CPlayers::DASH:
		Ani_Skill_Dash(0.f);
		break;
	case CPlayers::HIDDEN_SKILL:
		Ani_Skill_Innate(0.f);
		break;
	case CPlayers::SK_0:
		Ani_Skill_Buff1(0.f);
		break;
	case CPlayers::SK_1:
		Ani_Skill_Buff2(0.f);
		break;
	case CPlayers::SK_2:
		Ani_Skill_Buff3(0.f);
		break;
	case CPlayers::SK_3:
		Ani_Skill_Buff4(0.f);
		break;
	case CPlayers::SK_4:
		Ani_Skill_ATK1(0.f);
		break;
	case CPlayers::STAND_NORMAL:
		Ani_Stand(0.f);
		break;
	case CPlayers::STAND_WP:
		Ani_Stand(0.f);
		break;
	case CPlayers::RUN_F:
		Ani_Run(0.f);
		break;
	case CPlayers::RUN_B:
		Ani_Run(0.f);
		break;
	case CPlayers::RUN_L:
		Ani_Run(0.f);
		break;
	case CPlayers::RUN_R:
		Ani_Run(0.f);
		break;
	case CPlayers::RUN_FL:
		Ani_Run(0.f);
		break;
	case CPlayers::RUN_FR:
		Ani_Run(0.f);
		break;
	case CPlayers::RUN_BL:
		Ani_Run(0.f);
		break;
	case CPlayers::RUN_BR:
		Ani_Run(0.f);
		break;
	case CPlayers::JUMP_UP:
		Ani_Jump(0.f);
		break;
	case CPlayers::JUMP_LOOP:
		Ani_Jump(0.f);
		break;
	case CPlayers::JUMP_DOWN:
		Ani_Jump(0.f);
		break;
	case CPlayers::HIT_UP_ALL:
		Ani_Hit(0.f);
		break;
	case CPlayers::DMG_F:
		Ani_DMG_F(0.f);
		break;
	case CPlayers::DMG_B:
		Ani_DMG_B(0.f);
		break;
	case CPlayers::DIE:
		Ani_Die(0.f);
		break;
	case CPlayers::STUN:
		Ani_Stun(0.f);
		break;
	case CPlayers::DEADBODY:
		Ani_DeadBody(0.f);
		break;
	case CPlayers::MOUNT_DIVE:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_FLYING_B:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_FLYING_BOOST_FLUTTER:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_FLYING_F_FLUTTER:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_FLYING_F_GLIDE:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_FLYING_IDLE:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_RUN_F:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_RUN_B:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_ROT_R:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_ROT_L:
		Ani_Mount(0.f);
		break;
	case CPlayers::MOUNT_IDLE:
		Ani_Mount(0.f);
		break;
	}
	m_eOriTrack = m_eTrackID;
	m_pMesh->FrameMove((fTrackTime * 1.5f) / m_fSlowTime);

	if(m_pMesh->GetPeriod() - m_fPeriod <= m_pMesh->GetTrackPosition())
	{
		m_ePrev_TrackID = m_eTrackID;
		m_bTrack = false;
		m_fSlowTime = 1.f;
		m_bCancel = false;

		switch(m_eTrackID)
		{
		case HIDDEN_SKILL:
			m_eTrackID = STAND_NORMAL;
			m_bActStateSet.reset(ACT_EVASION);
			m_bSkillSet.reset(SKILL_INNATE);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			break;
		case DASH:
			m_eTrackID = STAND_NORMAL;
			m_fAcc = 0.f;
			m_bActStateSet.reset(ACT_DASH);
			m_bSkillSet.reset(SKILL_DASH);
			m_fSlowTime = 1.f;
			Engine::Set_RadialRender(false);
			break;
		case ATK_0:
			m_eTrackID = STAND_NORMAL;
			m_bCombo_Input = false;
			m_bAtkComboSet.reset(ATTACK_1);
			m_fPeriod = 0.3f;
			m_fSlowTime = 1.f;
			break;
		case ATK_1:
			m_eTrackID = STAND_NORMAL;
			m_bCombo_Input = false;
			m_bAtkComboSet.reset(ATTACK_2);
			m_fPeriod = 0.3f;
			m_fSlowTime = 1.f;
			break;
		case ATK_2:
			m_eTrackID = STAND_NORMAL;
			m_bCombo_Input = false;
			m_bAtkComboSet.reset(ATTACK_3);
			m_fPeriod = 0.3f;
			m_fSlowTime = 1.f;
			break;
		case SK_0:
			m_eTrackID = STAND_NORMAL;
			m_bSkillSet.reset(SKILL_1);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			break;
		case SK_1:
			m_eTrackID = STAND_NORMAL;
			m_bSkillSet.reset(SKILL_2);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			break;
		case SK_2:
			m_eTrackID = STAND_NORMAL;
			m_bSkillSet.reset(SKILL_3);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			break;
		case SK_3:
			m_eTrackID = STAND_NORMAL;
			m_bSkillSet.reset(SKILL_4);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			if(m_bMainPlayer == false)
				m_vCenterPos = _vec3(0.f,0.f,0.f);
			break;
		case SK_4:
			m_eTrackID = STAND_NORMAL;
			m_bSkillSet.reset(SKILL_5);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			break;
		case DIE:
			m_eTrackID = STAND_NORMAL;
			m_bActStateSet.reset(ACT_DIE);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			break;
		case STAND_NORMAL:
			break;
		case STAND_WP:
			break;
		case JUMP_UP:
			break;
		case JUMP_LOOP:
			break;
		case JUMP_DOWN:
			break;
		case HIT_UP_ALL:
			m_eTrackID = STAND_NORMAL;
			m_bActStateSet.reset(ACT_HIT_UP);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;

			if(m_bSkillSet.any())
				m_bSkillSet.reset();
			if(m_bAtkComboSet.any())
				m_bAtkComboSet.reset();
			break;
		case DMG_F:
			m_eTrackID = STAND_NORMAL;
			m_bActStateSet.reset(ACT_DMG_HIT);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;

			if(m_bSkillSet.any())
				m_bSkillSet.reset();
			if(m_bAtkComboSet.any())
				m_bAtkComboSet.reset();
			break;
		case DMG_B:
			break;
		case STUN:
			m_eTrackID = STAND_NORMAL;
			m_bActStateSet.reset(ACT_STUN);
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;

			if(m_bSkillSet.any())
				m_bSkillSet.reset();
			if(m_bAtkComboSet.any())
				m_bAtkComboSet.reset();
			break;
		case DEADBODY:
			break;
		}
		m_pMesh->SetAnimationSet(m_eTrackID);
	}
}

void CPaladin::AI_Stand()
{
	if(NULL != m_pTargetMonster)
	{

	}
	else
		m_eTrackID = STAND_NORMAL;
}
void CPaladin::AI_Patrol()
{
	if(m_bSkillSet.test(SKILL_1) ||
		m_bSkillSet.test(SKILL_4) || 
		m_bSkillSet.test(SKILL_2) ||
		m_bSkillSet.test(SKILL_3))
		return;

	if(m_bActStateSet.test(ACT_DASH))
	{
		if(m_eTrackID != CPlayers::DASH)
		{
			m_bSkillSet.reset(SKILL_DASH);
			m_bActStateSet.reset(ACT_DASH);
		}
	}

	if(m_pTargetMonster == NULL)
		Movement_NonTarget();

	if(m_pTargetMonster != NULL)
		Movement_Target();

	Movement_Heals();
}
void CPaladin::AI_Combat()
{
	/// 전투 상태

	/// 어떤 공격을 할 건지 판단을 거리로 체크한다

	/// TargetDis  현재 플레이어 타겟 과의 고정 거리
	/// CurrentDis 실제 플레이어와 타겟의 거리 

	if(m_bSkillSet.any())
	{
		if(m_eTrackID != SK_4)
			m_bSkillSet.reset(SKILL_5);
		if(m_eTrackID != SK_3)
			m_bSkillSet.reset(SKILL_4);
		if(m_eTrackID != SK_2)
			m_bSkillSet.reset(SKILL_3);
		if(m_eTrackID != SK_1)
			m_bSkillSet.reset(SKILL_2);
		if(m_eTrackID != SK_0)
			m_bSkillSet.reset(SKILL_1);

		if(m_eTrackID != DASH)
		{
			m_bSkillSet.reset(SKILL_DASH);
			m_bActStateSet.reset(ACT_DASH);
		}
	}
	if(m_bSkillSet.test(SKILL_5))
	{
		if(m_eTrackID != SK_4)
			m_bSkillSet.reset(SKILL_5);
	}
	if(m_bSkillSet.test(SKILL_1) ||
		m_bSkillSet.test(SKILL_2) ||
		m_bSkillSet.test(SKILL_3) ||
		m_bSkillSet.test(SKILL_4))
	{
		if(m_bAtkComboSet.count() != 0)
			m_bAtkComboSet.reset();

		return;
	}

	if(m_fCurrentDis > 0.4f	&& m_fCurrentDis <= m_fTargetDis)
	{
		if(m_fCurrentDis > 1.3f && m_fCurrentDis < m_fTargetDis)
		{
			if(!m_bCoolTimeSet.test(SKILL_3))	/// 공버프
			{
				if(m_bTrack == true)
					return;

				for(int i = 0; i < SUB_END; ++i)
					const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->AtkBuff(2.f);

				m_eTrackID = SK_2;
				m_eAI_StateID = AI_SKILL;
				m_bSkillSet.set(SKILL_3);
			}
		}	
		if(m_fCurrentDis > 1.f && m_fCurrentDis <= 1.3f)
		{
			if(!m_bCoolTimeSet.test(SKILL_2))	/// 방버프
			{
				if(m_bTrack == true)
					return;

				for(int i = 0; i < SUB_END; ++i)
					const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->ResistUp(10.f);	

				m_pStatus->ResistUp(10.f);

				m_eTrackID = SK_1;
				m_eAI_StateID = AI_SKILL;
				m_bSkillSet.set(SKILL_2);
			}
		}
		if(m_fCurrentDis > 0.4f && m_fCurrentDis <= 1.f)
		{
			if(m_bAtkComboSet.any())
			{
				if(m_bTrack == true)
					return;
				else
					m_bAtkComboSet.reset();
			}
			else
			{
				if(!m_bCoolTimeSet.test(SKILL_DASH))
				{
					//m_bCoolTimeSet.set(SKILL_DASH);	/// 쿨타임 적용
					m_bActStateSet.set(ACT_DASH);  /// 상태 적용
					m_eTrackID = DASH;				/// 트랙 적용
					m_bSkillSet.set(SKILL_DASH);

					m_eAI_StateID = AI_DASH;
				}

			}		
		}
		if(!m_bSkillSet.any())
		{
			if(m_bAtkComboSet.any())
			{
				if(m_bTrack == true)
					return;
				else
					m_bAtkComboSet.reset();
			}
			else
				m_eAI_StateID = AI_TARGET_MOVE;

		}
	}
	////////////////////////////////////////////////////////////////////////// 기본
	if(m_fCurrentDis <= 0.4f)
	{
		if(m_eAI_StateID == AI_DASH)
		{
			if(m_bTrack == true)
				return;
		}
		if(!m_bSkillSet.any() || !m_bCoolTimeSet.any())
		{
			if(m_bAtkComboSet.count() == 0)
			{
				m_eTrackID = ATK_0;
				m_bAtkComboSet.set(ATTACK_1);
			}
			if(m_ePrev_TrackID == CPlayers::ATK_0 && m_bTrack == false)
			{
				m_bAtkComboSet.reset(ATTACK_1);
				m_eTrackID = ATK_1;
				m_bAtkComboSet.set(ATTACK_2);
			}
			if(m_ePrev_TrackID == CPlayers::ATK_1 && m_bTrack == false)
			{
				m_bAtkComboSet.reset(ATTACK_2);
				m_eTrackID = ATK_2;
				m_bAtkComboSet.set(ATTACK_3);
			}
			if(m_ePrev_TrackID == CPlayers::ATK_2 && m_bTrack == false)
				m_bAtkComboSet.reset();

			m_eAI_StateID = AI_ATTACK;

		}
	}
	////////////////////////////////////////////////////////////////////////// 스킬
	if(m_fCurrentDis < 0.4f)
	{
		if(m_eAI_StateID == AI_DASH)
		{
			if(m_bTrack == true)
				return;
		}
		if(m_bAtkComboSet.any())
		{
			if(!m_bCoolTimeSet.any())
				m_bAtkComboSet.reset();

			if(m_bCoolTimeSet.any())
				return;
		}
		if(!m_bSkillSet.test(SKILL_1))
		{
			if(!m_bCoolTimeSet.test(SKILL_5))
			{
				//m_bCoolTimeSet.set(SKILL_5);	/// 쿨타임 적용
				m_bSkillSet.set(SKILL_5);  /// 상태 적용
				m_eTrackID = SK_4;				/// 트랙 적용
				m_eAI_StateID = AI_SKILL;
			}

		}
	}
}

void CPaladin::AI_Move()
{
	if(m_eAI_StateID ==	AI_TARGET_MOVE)
	{
		//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(m_vTargetDir * (m_fSpeed * 0.01f) * m_fTimeDelta),m_dwIndex,m_fHeight);
		//m_pInfo->m_vPos += (m_vTargetDir * (m_fSpeed * 0.01f) * m_fTimeDelta);
		if(m_bGuardianCol == false)
			MoveOnNaviMesh(&(m_vTargetDir * (m_fSpeed * 0.01f) * m_fTimeDelta));

		m_eTrackID = RUN_F;
	}
	if(m_eAI_StateID == AI_MOVE)
	{
		if(m_vCenterPos != _vec3(0,0,0))
		{
			_myvec vDir = m_vCenterPos - m_pInfo->m_vPos;
			_float fDis = vDir.Length();
			vDir.Normalize();
			_float fRadian = acosf(vDir.Dot(&m_vLook));

			if(m_pInfo->m_vPos.z > m_vCenterPos.z)
				fRadian = -fRadian;
			else
				fRadian = fRadian;

			//m_pInfo->m_vPos += (vDir * (m_fSpeed * 0.02f) * m_fTimeDelta);
			//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vDir * (m_fSpeed * 0.02f) * m_fTimeDelta),m_dwIndex,m_fHeight);
			MoveOnNaviMesh(&(vDir * (m_fSpeed * 0.02f) * m_fTimeDelta));

			_float fTemp = m_pInfo->m_fAngle[Engine::ANGLE_Y];

			m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fRadian;
			if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
				m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;

		}
		m_eTrackID = RUN_F;
	}

}
void CPaladin::AI_State()
{

}

void CPaladin::UpdateState_AI()
{
	AI_Patrol();

	switch(m_eAI_StateID)
	{
	case CPlayers::AI_DIE:
		AI_State();
		break;
	case CPlayers::AI_DMG:
		AI_State();
		break;
	case CPlayers::AI_STUN:
		AI_State();
		break;
	case CPlayers::AI_DASH:
		AI_Combat();
		break;
	case CPlayers::AI_EVASION:
		AI_Combat();
		break;
	case CPlayers::AI_MOUNT:
		AI_State();
		break;
	case CPlayers::AI_DOWN:
		AI_State();
		break;
	case CPlayers::AI_MOVE:
		AI_Move();
		break;
	case CPlayers::AI_STAND:
		AI_Stand();
		break;
	case CPlayers::AI_SKILL:
		AI_Combat();
		break;
	case CPlayers::AI_ATTACK:
		AI_Combat();
		break;
	case CPlayers::AI_TARGET_MOVE:
		AI_Move();
		break;
	default:
		m_eAI_StateID = AI_STAND;
		break;
	}

}

void CPaladin::ConsoleDebug()
{
	////////////////////////////////////////////////////////////////////////// 상태 콘솔

	if(CKeyMgr::GetInstance()->GetToggleKey(VK_F1))
	{
		if(m_fConsoleTime[0] > 0.3f)
			m_fConsoleTime[0] = 0.f;
		else
			m_fConsoleTime[0] += m_fTimeDelta;

		if(m_fConsoleTime[0] == 0.f)
		{
			cout << " ////////////////////////////////// " << " m_bSkillSet" << endl;
			cout << " [0] / SKILL_1" << " / " << m_bSkillSet.test(SKILL_1) << endl;
			cout << " [1] / SKILL_2" << " / " << m_bSkillSet.test(SKILL_2) << endl;
			cout << " [2] / SKILL_3" << " / " << m_bSkillSet.test(SKILL_3) << endl;
			cout << " [3] / SKILL_4" << " / " << m_bSkillSet.test(SKILL_4) << endl;
			cout << " [4] / SKILL_5" << " / " << m_bSkillSet.test(SKILL_5) << endl;
			cout << " [5] / SKILL_DASH" << " / " << m_bSkillSet.test(SKILL_DASH) << endl;
			cout << " [6] / SKILL_INNATE" << " / " << m_bSkillSet.test(SKILL_INNATE) << endl;
			cout << " [7] / SKILL_DEFAULT" << " / " << m_bSkillSet.test(SKILL_DEFAULT) << endl;
		}

	}

	if(CKeyMgr::GetInstance()->GetToggleKey(VK_F2))
	{
		if(m_fConsoleTime[1] > 0.5f)
			m_fConsoleTime[1] = 0.f;
		else
			m_fConsoleTime[1] += m_fTimeDelta;


		if(m_fConsoleTime[1] == 0.f)
		{
			cout << " ////////////////////////////////// " << " m_bAtkComboSet" << endl;
			cout << " [0] / ATTACK_1" << " / " << m_bAtkComboSet.test(ATTACK_1) << endl;
			cout << " [1] / ATTACK_2" << " / " << m_bAtkComboSet.test(ATTACK_2) << endl;
			cout << " [2] / ATTACK_3" << " / " << m_bAtkComboSet.test(ATTACK_3) << endl;
		}
	}

	if(CKeyMgr::GetInstance()->GetToggleKey(VK_F3))
	{
		if(m_fConsoleTime[2] > 0.3f)
			m_fConsoleTime[2] = 0.f;
		else
			m_fConsoleTime[2] += m_fTimeDelta;


		if(m_fConsoleTime[2] == 0.f)
		{
			cout << " ////////////////////////////////// " << " m_bActStateSet" << endl;
			cout << " [0] / ACT_DASH" << " / " << m_bActStateSet.test(ACT_DASH) << endl;
			cout << " [1] / ACT_JUMP" << " / " << m_bActStateSet.test(ACT_JUMP) << endl;
			cout << " [2] / ACT_MOUNT" << " / " << m_bActStateSet.test(ACT_MOUNT) << endl;
			cout << " [3] / ACT_STUN" << " / " << m_bActStateSet.test(ACT_STUN) << endl;
			cout << " [4] / ACT_CANCLE" << " / " << m_bActStateSet.test(ACT_CANCLE) << endl;
			cout << " [5] / ACT_EVASION" << " / " << m_bActStateSet.test(ACT_EVASION) << endl;
			cout << " [6] / ACT_DOWN" << " / " << m_bActStateSet.test(ACT_DOWN) << endl;
			cout << " [7] / ACT_DMG_HIT" << " / " << m_bActStateSet.test(ACT_DMG_HIT) << endl;
			cout << " [8] / ACT_DIE" << " / " << m_bActStateSet.test(ACT_DIE) << endl;
			cout << " [9] / ACT_DEFAULT" << " / " << m_bActStateSet.test(ACT_DEFAULT) << endl;
		}
	}

	if(CKeyMgr::GetInstance()->GetToggleKey(VK_F4))
	{
		if(m_fConsoleTime[3] > 0.3f)
			m_fConsoleTime[3] = 0.f;
		else
			m_fConsoleTime[3] += m_fTimeDelta;


		if(m_fConsoleTime[3] == 0.f)
		{
			cout << " ////////////////////////////////// " << " m_fSkillCooltime" << endl;
			cout << " [0] / SKILL_1" << " / " << m_fSkillCooltime[SKILL_1] << endl;
			cout << " [1] / SKILL_2" << " / " << m_fSkillCooltime[SKILL_2] << endl;
			cout << " [2] / SKILL_3" << " / " << m_fSkillCooltime[SKILL_3] << endl;
			cout << " [3] / SKILL_4" << " / " << m_fSkillCooltime[SKILL_4] << endl;
			cout << " [4] / SKILL_5" << " / " << m_fSkillCooltime[SKILL_5] << endl;
			cout << " [5] / SKILL_DASH" << " / " << m_fSkillCooltime[SKILL_DASH] << endl;
			cout << " [6] / SKILL_INNATE" << " / " << m_fSkillCooltime[SKILL_INNATE] << endl;
			cout << " [7] / SKILL_DEFAULT" << " / " << m_fSkillCooltime[SKILL_DEFAULT] << endl;
		}
	}

	if(CKeyMgr::GetInstance()->GetToggleKey(VK_F5))
	{
		if(m_fConsoleTime[4] > 0.3f)
			m_fConsoleTime[4] = 0.f;
		else
			m_fConsoleTime[4] += m_fTimeDelta;


		if(m_fConsoleTime[4] == 0.f)
		{
			cout << " ////////////////////////////////// " << " m_eAI_State" << endl;
			switch(m_eAI_StateID)
			{
			case CPlayers::AI_STAND:
				cout << "AI_STAND" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_DIE:
				cout << "AI_DIE" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_MOUNT:
				cout << "AI_MOUNT" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_EVASION:
				cout << "AI_EVASION" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_STUN:
				cout << "AI_STUN" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_DASH:
				cout << "AI_DASH" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_DMG:
				cout << "AI_DMG" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_DOWN:
				cout << "AI_DOWN" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_MOVE:
				cout << "AI_MOVE" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_SKILL:
				cout << "AI_SKILL" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_TARGET_MOVE:
				cout << "AI_TARGET_MOVE" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_ATTACK:
				cout << "AI_ATTACK" << " / " << m_eAI_StateID << endl;
				break;
			case CPlayers::AI_DEFAULT:
				cout << "AI_DEFAULT" << " / " << m_eAI_StateID << endl;
				break;
			}				
		}
	}
	//////////////////////////////////////////////////////////////////////////


	/*if(CKeyMgr::GetInstance()->GetStateKey('L'))
	{
	for(int i = 0; i < SUB_END; ++i)
	{
	const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->m_tRealStat.m_fCurHp -= 10.f;
	m_pStatus->m_tRealStat.m_fCurHp -= 5.f;
	}

	}*/
	/*if(CKeyMgr::GetInstance()->GetStateKey('K'))
	{
	for(int i = 0; i < SUB_END; ++i)
	{
	const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->m_tRealStat.m_fCurHp = const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->m_tRealStat.m_fMaxHp;
	m_pStatus->m_tRealStat.m_fCurHp = m_pStatus->m_tRealStat.m_fMaxHp;

	}
	}*/
}

void CPaladin::ChoiseHeal()
{
	if(!m_bMainPlayer)
	{
		m_wPrevTarget_Cnt = 99;
		m_vChoisePos = _vec3(0,0,0);
		Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_TARGETSELECT);
		return;
	}

	if(CKeyMgr::GetInstance()->GetKeyDown(VK_TAB))
	{
		if(m_wSubTarGet_Cnt >= 3)
			m_wSubTarGet_Cnt = 0;
		else
			++m_wSubTarGet_Cnt;
	}
	if(m_wSubTarGet_Cnt <= 2)
		m_pSubTarGet = m_pSubPlayer[m_wSubTarGet_Cnt];
	

	if(m_wPrevTarget_Cnt != m_wSubTarGet_Cnt)
	{
		if(m_wPrevTarget_Cnt == 99)
			m_wPrevTarget_Cnt = 0;
		else
			m_wPrevTarget_Cnt = m_wSubTarGet_Cnt;

		if(m_wPrevTarget_Cnt == 3)
		{
			Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_TARGETSELECT);
			Engine::Call_EtcFX(Engine::ETC_TARGETSELECT,&m_vChoisePos,true);
		}
		else		
		{
			Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_TARGETSELECT);
			Engine::Call_EtcFX(Engine::ETC_TARGETSELECT,&m_vChoisePos,true);
		}		
	}

	if(m_wPrevTarget_Cnt == 3)
		m_vChoisePos = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + 0.3f, m_pInfo->m_vPos.z);
	else		
		m_vChoisePos = _vec3(const_cast<Engine::CTransform*>(m_pSubPlayer[m_wPrevTarget_Cnt]->GetInfo())->m_vPos.x,const_cast<Engine::CTransform*>(m_pSubPlayer[m_wPrevTarget_Cnt]->GetInfo())->m_vPos.y + 0.3f, const_cast<Engine::CTransform*>(m_pSubPlayer[m_wPrevTarget_Cnt]->GetInfo())->m_vPos.z);


	
}

void CPaladin::RangeHeal()
{
	if(false == m_bRangeHeal)
		return;

	if(m_fRangeHeal_Time > 10.f)
	{
		m_bRangeHeal = false;
		m_fRangeHeal_Time = 0.f;
	}
	else
	{
		m_fRangeHeal_Time += m_fTimeDelta;

		for(int i = 0; i < SUB_END; ++i)
		{
			if(m_pSubPlayer[i] == NULL)
				continue;

			_myvec vSubDir,vMyDir;
			_vec3 vPrevPos,vTargetPos,vMyPos;
			vPrevPos = _vec3(m_vPrevPos.x,0.f,m_vPrevPos.z);
			vTargetPos = _vec3(const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_vPos.z);
			vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
			if(m_bMainPlayer)
			{
				vMyDir	= vPrevPos - vMyPos;
				vSubDir = vPrevPos - vTargetPos;
				{
					if(vSubDir.Length() < 3.5f) /// 이펙트 범위 이걸로 조절
					{
						const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->Healing(10.f * m_fTimeDelta);
						Engine::Call_PaladinFX(Engine::P_SKILL4_TARGET,&const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_vPos,true,NULL,&const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_fAngle[Engine::ANGLE_Y]);
						/// 여기다 사운드 넣음됨
					}

					if(vMyDir.Length() < 3.5f)
					{
						m_pStatus->Healing(10.f * m_fTimeDelta);
						Engine::Call_PaladinFX(Engine::P_SKILL4_TARGET,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
					}

				}
			}
			else
			{
				vSubDir = m_vHealPos - vTargetPos;
				vMyDir = m_vHealPos - vMyPos;

				if(vSubDir.Length() < 3.5f) /// 이펙트 범위 이걸로 조절
				{
					const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->Healing(10.f * m_fTimeDelta);
					Engine::Call_PaladinFX(Engine::P_SKILL4_TARGET,&const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_vPos,true,NULL,&const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_fAngle[Engine::ANGLE_Y]);
				}
				if(vMyDir.Length() < 3.5f)
				{				
					m_pStatus->Healing(10.f * m_fTimeDelta);
					Engine::Call_PaladinFX(Engine::P_SKILL4_TARGET,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				}
			}

		}
	}

}
HRESULT CPaladin::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_Paladin");
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

	return S_OK;
}
void CPaladin::Set_ContantTable(LPD3DXEFFECT	pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CPaladin* CPaladin::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPaladin*		pGameObject = new CPaladin(pDevice,JOB_PALADIN);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CPaladin::Free(void)
{
	CPlayers::Free();
	//Engine::CGameObject::Free();
}
////////////////////////////////////////////////////////////////////////// 애니메이션 관련 함수

void CPaladin::Ani_Run(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();


	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_Attack(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	_float fAttackDis = 0.f;
	if(m_bMainPlayer)
		fAttackDis = 0.5f;
	else
		fAttackDis = 0.4f;


	if(m_eTrackID == CPlayers::ATK_0)
	{
		if(fTrackTime < 0.25f)
			m_fSlowTime = 1.8f;
		else
		{
			m_fSlowTime = 0.7f;

			if(NULL != m_pTargetMonster)
			{
				if(0.6f >= fTrackTime && m_bCancel == false)
				{
					if(m_fCurrentDis < fAttackDis)
					{
						m_bSlowCheck = true;
						m_bCancel = true;
						m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(9,2),(CUnits*)this,Engine::CStatus::DMG_MELEE);
						const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
						Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
						_vec3 vEffectPos = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + 0.3f,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
						Engine::Call_EtcFX(Engine::ETC_HITCIRCLE,&vEffectPos,false);
						if(m_bMainPlayer)
							CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinAttHit.ogg", 0.5f);
						else
						{
							if(m_fSoundTargetDistance < 6.f)
								CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinAttHit.ogg", m_fSoundVolume);
						}

					}
				}
			}
		}
		if(m_bMainPlayer)
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinAtt1.ogg", 0.5f);
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinAttEffect1.ogg", 0.5f);
			}
		}
		else
		{

			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f && m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinAtt1.ogg", m_fSoundVolume);
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinAttEffect1.ogg", m_fSoundVolume);
			}

		}

		if(!m_bCoolTimeSet.test(SKILL_DEFAULT))
			m_bCoolTimeSet.set(SKILL_DEFAULT);
	}
	if(m_eTrackID == CPlayers::ATK_1)
	{
		if(fTrackTime < 0.25f)
			m_fSlowTime = 1.8f;
		else
			m_fSlowTime = 0.8f;

		if(m_bMainPlayer)
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinAtt2.ogg", 0.5f);
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinAttEffect1.ogg", 0.5f);
			}
		}
		else
		{

			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f && m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinAtt2.ogg", m_fSoundVolume);
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinAttEffect1.ogg", m_fSoundVolume);
			}

		}

		if(NULL != m_pTargetMonster)
		{
			if(fTrackTime >= 0.7f && m_bCancel == false)
			{
				if(m_fCurrentDis < fAttackDis)
				{
					m_bSlowCheck = true;
					m_bCancel = true;
					m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(1,3),(CUnits*)this,Engine::CStatus::DMG_MELEE);
					const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
					Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
					_vec3 vEffectPos = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + 0.3f,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
					Engine::Call_EtcFX(Engine::ETC_HITCIRCLE,&vEffectPos,false);
					if(m_bMainPlayer)
						CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinAttHit.ogg", 0.5f);
					else
					{
						if(m_fSoundTargetDistance < 6.f)
							CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinAttHit.ogg", m_fSoundVolume);
					}
				}
			}
		}

		if(!m_bCoolTimeSet.test(SKILL_DEFAULT))
			m_bCoolTimeSet.set(SKILL_DEFAULT);

	}
	if(m_eTrackID == CPlayers::ATK_2)
	{
		if(fTrackTime < 0.15f)
			m_fSlowTime = 2.2f;
		else
			m_fSlowTime = 0.8f;

		if(m_bMainPlayer)
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinAtt3.ogg", 0.5f);
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinAttEffect2.ogg", 0.5f);
			}
		}
		else
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f && m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinAtt3.ogg", m_fSoundVolume);
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinAttEffect2.ogg", m_fSoundVolume);
			}

		}

		if(m_bMainPlayer == true)
		{
			if(fTrackTime > 0.15f && fTrackTime < 0.35f)
			{
				//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(m_pInfo->m_vDir * m_fTimeDelta * 10.f),m_dwIndex,m_fHeight);
				//m_pInfo->m_vPos +=( m_pInfo->m_vDir * m_fTimeDelta * 10.f);
				MoveOnNaviMesh(&(m_pInfo->m_vDir * m_fTimeDelta * 10.f));
			}
		}
		if(NULL != m_pTargetMonster)
		{
			if(fTrackTime >= 0.7f && m_bCancel == false)
			{
				if(m_fCurrentDis < fAttackDis)
				{
					m_bSlowCheck = true;
					m_bCancel = true;
					m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(1,3),(CUnits*)this,Engine::CStatus::DMG_MELEE);
					const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
					Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
					_vec3 vEffectPos = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + 0.3f,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
					Engine::Call_EtcFX(Engine::ETC_HITCIRCLE,&vEffectPos,false);
					if(m_bMainPlayer)
						CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinAttHit.ogg", 0.5f);
					else
					{
						if(m_fSoundTargetDistance < 6.f)
							CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinAttHit.ogg", m_fSoundVolume);
					}
					//__debugbreak();
				}
			}
		}
		if(!m_bCoolTimeSet.test(SKILL_DEFAULT))
			m_bCoolTimeSet.set(SKILL_DEFAULT);
	}

	//m_ePrev_TrackID = m_eTrackID;

}
void CPaladin::Ani_Skill_Buff1(float fTrackTime)
{
	m_fSkillAngle_Effect[SKILL_1] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f));
	m_vEffectPos[SKILL_1] = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + 0.2f,m_pInfo->m_vPos.z);


	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();


	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(!m_bCoolTimeSet.test(SKILL_1))
		m_bCoolTimeSet.set(SKILL_1);

	if(m_bMainPlayer)
	{
		/// 탭을하든 타겟을 찍든.
		if(m_bCancel == false)
		{
			if(m_wSubTarGet_Cnt <= 2)
			{
				if(m_pSubTarGet != NULL)
				{
					const_cast<Engine::CStatus*>(m_pSubTarGet->GetStatus())->Healing(40.f);
					Engine::Call_PaladinFX(Engine::P_SKILL1_TARGET, &const_cast<Engine::CTransform*>(m_pSubTarGet->GetInfo())->m_vPos,true);
					/// 힐 받는 구간 
				}
			}
			else
			{
				m_pStatus->Healing(20.f);	
				Engine::Call_PaladinFX(Engine::P_SKILL1_TARGET, &m_pInfo->m_vPos,true);
			}
			CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinEffect1_1.ogg",0.5f);

			m_bCancel = true;
		}
	}
	else
	{
		if(m_pTargetStatus != NULL)
		{
			if(m_bCancel == false)
			{
				m_pTargetStatus->Healing(20.f);
				Engine::Call_PaladinFX(Engine::P_SKILL1_TARGET, &m_vSubPos,true);
				m_bCancel = true;
			}

		}
	}

	if(fTrackTime >= 0.05f)
	{
		if(m_bCancel == false)
		{
			Engine::Call_PaladinFX(Engine::P_SKILL1, &m_vEffectPos[SKILL_1], true);	
			if(m_bMainPlayer)
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinEffect1.ogg", 0.5f);
			else
			{
				if(m_fSoundTargetDistance < 6.f)
					CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinEffect1.ogg", m_fSoundVolume);
			}
			m_bCancel = true;
		}
	}

	if(fTrackTime >= 0.7f)
	{
		Engine::Call_PaladinFX(Engine::P_SKILL1_SUB, &m_vEffectPos[SKILL_1], true);		
		Engine::Call_PaladinFX(Engine::P_SKILL1_SUB, &m_pInfo->m_vPos, true);
	}

}
void CPaladin::Ani_Skill_Buff2(float fTrackTime)
{
	m_fSkillAngle_Effect[SKILL_2] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f));
	m_vEffectPos[SKILL_2] = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + 0.2f,m_pInfo->m_vPos.z);


	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();


	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(!m_bCoolTimeSet.test(SKILL_2))
		m_bCoolTimeSet.set(SKILL_2);

	if(fTrackTime >= 0.01f)
	{
		if(m_bCancel == false)
		{
			Engine::Call_PaladinFX(Engine::P_SKILL2, &m_vEffectPos[SKILL_2], false);

			for(int i = 0; i < SUB_END; ++i)
			{
				_myvec vDir;
				vDir = const_cast<Engine::CTransform*>(m_pSubPlayer[i]->GetInfo())->m_vPos - m_pInfo->m_vPos;
				
				Engine::Call_GuardianFX(Engine::G_SKILL1_TARGET,&m_pInfo->m_vPos,true);

				if(vDir.Length() < 5.f)
					Engine::Call_GuardianFX(Engine::G_SKILL1_TARGET,&const_cast<Engine::CTransform*>(m_pSubPlayer[i]->GetInfo())->m_vPos,true);
			}
			if(m_bMainPlayer)
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinEffect2.ogg");
			else
			{
				if(m_fSoundTargetDistance < 6.f)
					CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinEffect2.ogg", m_fSoundVolume);
			}
			m_bCancel = true;
		}
	}
}
void CPaladin::Ani_Skill_Buff3(float fTrackTime)
{
	m_fSkillAngle_Effect[SKILL_3] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] - D3DXToRadian(90.f));
	m_vEffectPos[SKILL_3] = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + 0.2f,m_pInfo->m_vPos.z);


	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();


	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(!m_bCoolTimeSet.test(SKILL_3))
		m_bCoolTimeSet.set(SKILL_3);

	if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
	{
		if(m_bMainPlayer)
		{
			CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinEffect3.ogg", 0.5f);
		}
		else
		{
			if(m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinEffct3.ogg", m_fSoundVolume);
			}
		}
	}

	if(fTrackTime >= 0.5f)
	{
		if(m_bCancel == false)
		{
			Engine::Call_PaladinFX(Engine::P_SKILL3_TARGET,&m_vEffectPos[SKILL_3],true);


			for(int i = 0; i < SUB_END; ++i)
			{
				_myvec vDir;
				vDir = const_cast<Engine::CTransform*>(m_pSubPlayer[i]->GetInfo())->m_vPos - m_pInfo->m_vPos;

				if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
				{
					CSoundMgr::GetInstance()->PlayPaladinSound2_1(L"PaladinEffect3.ogg", 0.5f);
				}

				if(vDir.Length() < 4.f)
					Engine::Call_PaladinFX(Engine::P_SKILL3_TARGET,&const_cast<Engine::CTransform*>(m_pSubPlayer[i]->GetInfo())->m_vPos,true);	
			}
			m_bCancel = true;
		}
	}

}

void CPaladin::Ani_Skill_Buff4(float fTrackTime)
{
	m_fSkillAngle_Effect[SKILL_4] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f));
	m_vEffectPos[SKILL_4] = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + g_fEffect_Y,m_pInfo->m_vPos.z);

	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(!m_bCoolTimeSet.test(SKILL_4))
		m_bCoolTimeSet.set(SKILL_4);


	if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
	{
		if(m_bMainPlayer)
		{
			CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinEffect4.ogg");
			CSoundMgr::GetInstance()->PlayPaladinSound2_1(L"PaladinEffect4_1.ogg");
		}
		else
		{
			if(m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinEffct4.ogg", m_fSoundVolume);
				CSoundMgr::GetInstance()->PlayPaladinSound2_1(L"PaladinEffect4_1.ogg", m_fSoundVolume);
			}
		}
	}

	if(m_bMainPlayer)
	{
		if(fTrackTime > 0.9f && m_bCancel == false)
		{
			m_vPrevPos = m_pInfo->m_vPos;
			m_bRangeHeal = true;
			m_bCancel = true;
			Engine::Call_PaladinFX(Engine::P_SKILL4, &m_vEffectPos[SKILL_4],false,NULL,&m_fSkillAngle_Effect[SKILL_4]);
		}
	}
	else
	{
		if(fTrackTime > 0.9f && m_bCancel == false)
		{
			m_vPrevPos = m_pInfo->m_vPos;
			m_bRangeHeal = true;
			m_bCancel = true;
			Engine::Call_PaladinFX(Engine::P_SKILL4, &m_vEffectPos[SKILL_4],false,NULL,&m_fSkillAngle_Effect[SKILL_4]);
		}
	}



	//공격전 (batk = false 한번만)->공격 -> 해당시간대 들어오면 batk = true -> 끝나면 batk = false 

}

void CPaladin::Ani_Skill_ATK1(float fTrackTime)
{
	m_fSkillAngle_Effect[SKILL_5] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f));
	m_vEffectPos[SKILL_5] = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + 0.2f,m_pInfo->m_vPos.z);


	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinSkill1.ogg");
	}
	else
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f && m_fSoundTargetDistance < 6.f)
			CSoundMgr::GetInstance()->PlayPaladinSound3(L"PaladinAttHit.ogg", m_fSoundVolume);
	}


	if(!m_bCoolTimeSet.test(SKILL_5))
		m_bCoolTimeSet.set(SKILL_5);

	_float fAttackDis = 0.f;
	if(m_bMainPlayer)
		fAttackDis = 0.5f;
	else
		fAttackDis = 0.4f;

	if(NULL != m_pTargetMonster)
	{
		if(fTrackTime >= 1.f && m_bCancel == false)
		{
			if(m_fCurrentDis < fAttackDis)
			{

				m_bSlowCheck = true;

				if(m_bCancel == false)
				{
					m_pTargetMonster->Set_bStunState(true);
					m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(3,3),(CUnits*)this,Engine::CStatus::DMG_MELEE);
					const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
					Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
					_vec3 vHitPos = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + 0.3f,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
					Engine::Call_EtcFX(Engine::ETC_HITCIRCLE,&vHitPos,false);

					_vec3 vEffectPos = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,
						const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + 0.2f,
						const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
					Engine::Call_PaladinFX(Engine::P_SKILL5,&m_vEffectPos[SKILL_5],false,NULL,&m_fSkillAngle_Effect[SKILL_5]);

					m_bCancel = true;
				}
			}
		}
	}
}
void CPaladin::Ani_Skill_Dash(float fTrackTime)
{
	m_fSkillAngle_Effect[SKILL_DASH] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f));
	m_vEffectPos[SKILL_DASH] = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + g_fEffect_Y,m_pInfo->m_vPos.z);


	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.2f && (float)m_pMesh->GetTrackPosition() < 0.25f)
		{
			CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinDash.ogg",0.5f);
			CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinDashEffect1.ogg",0.5f);
		}
	}
	else
	{
		if((float)m_pMesh->GetTrackPosition() > 0.2f && (float)m_pMesh->GetTrackPosition() < 0.25f && m_fSoundTargetDistance < 6.f)
		{
			CSoundMgr::GetInstance()->PlayPaladinSound1(L"PaladinDash.ogg",m_fSoundVolume);
			CSoundMgr::GetInstance()->PlayPaladinSound2(L"PaladinDashEffect1.ogg",m_fSoundVolume);
		}
	}

	if(!m_bCoolTimeSet.test(SKILL_DASH))
		m_bCoolTimeSet.set(SKILL_DASH);

	if(m_bActStateSet.test(ACT_DASH))
	{
		if(m_bMainPlayer)
		{
			if(fTrackTime < 0.5f)
			{
				m_fSlowTime = 2.2f;
				m_fAcc += 3.f * m_fTimeDelta ;
				//m_pInfo->m_vPos += m_pInfo->m_vDir * m_fTimeDelta * ((m_fSpeed)  * m_fAcc + m_fAcc * 0.1f - 0.1f);
				//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(m_pInfo->m_vDir * m_fTimeDelta * ((m_fSpeed)  * m_fAcc + m_fAcc * 0.1f - 0.1f)),m_dwIndex,m_fHeight);
				MoveOnNaviMesh(&(m_pInfo->m_vDir * m_fTimeDelta * ((m_fSpeed)  * m_fAcc + m_fAcc * 0.1f - 0.1f)));
			}
			else
			{
				m_fAcc = 0.f;
				m_fSlowTime = 1.f;

				if(fTrackTime >= 0.7f)
				{
					if(m_bCancel == false)
					{
						_vec3 vPos = _vec3(m_pInfo->m_vPos.x,0.f, m_pInfo->m_vPos.z);
						_float fDMG = (float)CRandom_Number::Get_RandomNum(2,3);
						Engine::Call_PaladinFX(Engine::P_DASH, &m_vEffectPos[SKILL_DASH], false,NULL,&m_fSkillAngle_Effect[SKILL_DASH]);						
						RangeSkill_State(SKILL_DASH,vPos,fDMG,2.f,this);

						m_bCancel = true;
					}

				}
			}

		}
		else
		{
			if(fTrackTime < 0.5f)
			{
				if(m_fCurrentDis > 0.3f)
				{
					m_fSlowTime = 2.2f;
					m_pInfo->m_vPos += (m_vTargetDir * (m_fCurrentDis * (m_fSpeed * 0.02f)) * m_fTimeDelta);
				}
			}
			else
			{
				m_fAcc = 0.f;
				m_fSlowTime = 1.f;

				if(fTrackTime >= 0.8f)
				{
					if(m_bCancel == false)
					{
						_vec3 vPos = _vec3(m_pInfo->m_vPos.x,0.f, m_pInfo->m_vPos.z);
						_float fDMG = (float)CRandom_Number::Get_RandomNum(2,3);
						RangeSkill_State(SKILL_DASH,vPos,fDMG,2.f,this);
						Engine::Call_PaladinFX(Engine::P_DASH, &m_vEffectPos[SKILL_DASH], false);

						m_bCancel = true;
					}

				}
			}
		}
	}


}

void CPaladin::Ani_Skill_Innate(float fTrackTime)
{
	m_fSkillAngle_Effect[SKILL_INNATE] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] - D3DXToRadian(90.f));
	m_vEffectPos[SKILL_INNATE] = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + g_fEffect_Y,m_pInfo->m_vPos.z);

	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(!m_bCoolTimeSet.test(SKILL_INNATE))
		m_bCoolTimeSet.set(SKILL_INNATE);

	if(m_bActStateSet.test(ACT_EVASION))
	{
		m_fSlowTime = 1.5f;
	}

	if(fTrackTime >= 0.1f)
	{
		if(m_bCancel == false)
		{
			if(m_bMainPlayer)
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"ShieldEffect.ogg",0.5f);
			m_bCancel = true;
		}
		else
		{
			if(m_fSoundTargetDistance < 6.f)
				CSoundMgr::GetInstance()->PlayPaladinSound2(L"ShieldEffect.ogg",m_fSoundVolume);
		}
	}
}

void CPaladin::Ani_Jump(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_Hit(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	if(fTrackTime <= 0.8f)
		MoveOnNaviMesh(&(-m_vTargetDir * (1.f) * m_fTimeDelta),this); 

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_Down(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_Die(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_DeadBody(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_Stand(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_DMG_B(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_DMG_F(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_Stun(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();


	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;
}

void CPaladin::Ani_Mount(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	m_fSlowTime = 1.3f;
}

void CPaladin::Movement_NonTarget()
{
	////////////////////////////////////////////////////////////////////////// 타겟이 없다

	Engine::CGameObject* pGameObject = Engine::Get_ObjList(LAYER_UI, L"QuestWin1")->front();
	if(pGameObject == NULL)
		return;

	if(m_pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
	{
		if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_PALADIN) == false)
		{
			m_eAI_StateID = AI_STAND;
			return;
		}
	}
	
	if(m_eAI_StateID == AI_ATTACK ||
		m_eAI_StateID == AI_DASH ||
		m_eAI_StateID == AI_SKILL ||
		m_eAI_StateID == AI_TARGET_MOVE)
	{
		m_eAI_StateID = AI_STAND;
	}

	_vec3 vFormation(m_vFormation.x,0.f,m_vFormation.z);
	_vec3 vPos(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);

	_myvec vDir = vFormation - vPos;
	_myvec vPlayerDir = m_pInfo->m_vDir;
	_float fDis = vDir.Length();

	vDir.y = 0.f;
	vPlayerDir.y = 0.f;

	vDir.Normalize();
	m_vTargetDir = vDir;
	vPlayerDir.Normalize();

	_float fDot = D3DXVec3Dot(&vDir, &m_vLook);
	m_fRadian = acosf(fDot);

	if(m_pInfo->m_vPos.z > m_vFormation.z)
		m_fRadian = -m_fRadian;
	else
		m_fRadian = m_fRadian;

	if(AI_MOVE == m_eAI_StateID)
	{
		if(fDis > 0.1f)
		{
			if(fDis > 0.1f && fDis < 0.12f)
			{
				//m_pInfo->m_vPos += vDir * 1.f * m_fTimeDelta;
				//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vDir * 1.f * m_fTimeDelta),m_dwIndex,m_fHeight);
				MoveOnNaviMesh(&(vDir * 1.f * m_fTimeDelta));
				m_eAI_StateID = AI_MOVE;
			}
			else
			{
				//m_pInfo->m_vPos += vDir * 1.1f * m_fTimeDelta;
				//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vDir * 1.1f * m_fTimeDelta),m_dwIndex,m_fHeight);
				MoveOnNaviMesh(&(vDir * 1.1f * m_fTimeDelta));
				m_pInfo->Update();
				m_eAI_StateID = AI_MOVE;
			}

			_float fTemp = m_pInfo->m_fAngle[Engine::ANGLE_Y];

			m_pInfo->m_fAngle[Engine::ANGLE_Y] = -m_fRadian;
			if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
				m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
		}
		else
		{
			if(m_pMainPlayer->Test_KeyBitSet(CPlayers::STAKEY_MOVE))
			{
				//m_pInfo->m_vPos += (vDir * 1.f * m_fTimeDelta);
				if(fDis < 0.1f)
					m_eAI_StateID = AI_STAND;

				else
				{
					//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vDir * 1.f * m_fTimeDelta),m_dwIndex,m_fHeight);
					MoveOnNaviMesh(&(vDir * 1.f * m_fTimeDelta));
					m_pInfo->m_fAngle[Engine::ANGLE_Y] = const_cast<Engine::CTransform*>(m_pMainPlayer->GetInfo())->m_fAngle[Engine::ANGLE_Y];
				}

			}
			else if(m_pMainPlayer->Get_TrackID() == CPlayers::STAND_NORMAL)
				m_eAI_StateID = AI_STAND;
		}				
	}
	if(AI_STAND == m_eAI_StateID)
	{
		if(NULL != m_pMainPlayer)
		{
			vDir.Normalize();

			if(fDis > 0.5f)
			{
				//m_pInfo->m_vPos += (vDir * 1.1f * m_fTimeDelta);
				//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(vDir * 1.1f * m_fTimeDelta),m_dwIndex,m_fHeight);
				MoveOnNaviMesh(&(vDir * 1.1f * m_fTimeDelta));

				_float fTemp = m_pInfo->m_fAngle[Engine::ANGLE_Y];

				m_pInfo->m_fAngle[Engine::ANGLE_Y] = -m_fRadian;
				if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
					m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;

				m_eAI_StateID = AI_MOVE;
			}
			else
			{
				m_pInfo->m_vPos = m_pInfo->m_vPos;
				m_eAI_StateID = AI_STAND;
			}
		}
	}
}

void CPaladin::Movement_Target()
{
	////////////////////////////////////////////////////////////////////////// 타겟이 있다
	_vec3 vTargetPos(((Engine::CTransform*)m_pTargetMonster->GetComponent(L"Transform"))->m_vPos.x, 0.f ,
		((Engine::CTransform*)m_pTargetMonster->GetComponent(L"Transform"))->m_vPos.z );		

	_vec3 vPos(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
	_vec3 vDir = vTargetPos - vPos;
	_vec3 vPlayerDir = _vec3(m_pInfo->m_vDir.x,0.f,m_pInfo->m_vDir.z);
	_vec3 vNormal = _vec3(vPlayerDir.z,0.f,-vPlayerDir.x);

	m_fCurrentDis = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir,&vDir);
	D3DXVec3Normalize(&vNormal,&vNormal);
	D3DXVec3Normalize(&vPlayerDir,&vPlayerDir);

	_float fScala = D3DXVec3Dot(&vDir,&vNormal);
	_float fDot	  = D3DXVec3Dot(&vDir,&vPlayerDir);
	m_fTargetAngle = D3DXToDegree(acosf(fDot));

	_float fTemp = m_pInfo->m_fAngle[Engine::ANGLE_Y];

	if(fScala > 0)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(m_fTargetAngle);
		if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}
	else
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(m_fTargetAngle);
		if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}

	m_fSaveAngle = m_pInfo->m_fAngle[Engine::ANGLE_Y];
	m_vSaveDir = m_vTargetDir;

	_vec3 vGuardianPos = _vec3(const_cast<Engine::CTransform*>(((CGuardian*)m_pSubPlayer[SUB_GUARDIAN])->GetInfo())->m_vPos.x
		,0.f,const_cast<Engine::CTransform*>(((CGuardian*)m_pSubPlayer[SUB_GUARDIAN])->GetInfo())->m_vPos.z);
	_vec3 vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
	_vec3 vSubPlayer_Dir = vGuardianPos - vMyPos;
	_float fLength = D3DXVec3Length(&vSubPlayer_Dir);
	_vec3 vMonster_Dir = m_vTargetDir;
	m_vSaveDir = vMonster_Dir;

	if(fLength < 0.25f)
	{
		_matrix matRotY;
		D3DXMatrixIdentity(&matRotY);

		D3DXVec3Normalize(&vSubPlayer_Dir,&vSubPlayer_Dir);
		D3DXVec3Normalize(&vMonster_Dir,&vMonster_Dir);
		_float fRadian = acosf(D3DXVec3Dot(&vSubPlayer_Dir,&vMonster_Dir));

		if(_finite(fRadian))
		{
			D3DXMatrixRotationY(&matRotY,-fRadian);
			D3DXVec3TransformNormal(&vMonster_Dir,&vMonster_Dir,&matRotY);
			MoveOnNaviMesh(&(vMonster_Dir * (1.f) * m_fTimeDelta));
			m_bGuardianCol = true;
		}
		//m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fRadian;

	}
	else
	{
		fTemp = m_pInfo->m_fAngle[Engine::ANGLE_Y];

		m_pInfo->m_fAngle[Engine::ANGLE_Y] = m_fSaveAngle;
		if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
			m_pInfo->m_fAngle[Engine::ANGLE_Y]  = fTemp;

		m_bGuardianCol = false;

	}
	if(m_fCurrentDis < m_fTargetDis)
	{
		/// 타겟이 범위 안에 들어와 있다
		AI_Combat();	
	}
}

void CPaladin::Movement_Heals()
{
	/// 범위힐

	if(CKeyMgr::GetInstance()->GetStateKey('6'))
	{
		for(int i = 0; i < SUB_END; ++i)
			const_cast<Engine::CStatus*>(((CRanger*)m_pSubPlayer[i])->GetStatus())->m_tRealStat.m_fCurHp -= 10.f;
	}

	if(const_cast<Engine::CStatus*>(((CGuardian*)m_pSubPlayer[SUB_GUARDIAN])->GetStatus())->Get_Percent(70.f) == true && 
		const_cast<Engine::CStatus*>(((CRanger*)m_pSubPlayer[SUB_RANGER])->GetStatus())->Get_Percent(70.f) == true && 
		const_cast<Engine::CStatus*>(((CMagician*)m_pSubPlayer[SUB_MAGICIAN])->GetStatus())->Get_Percent(70.f) == true)
	{
		if(!m_bCoolTimeSet.test(SKILL_4))
		{
			if(m_bMainPlayer == false)
			{
				if(m_pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
					return;
			}

			_vec3 vPos1 = const_cast<Engine::CTransform*>(((CGuardian*)m_pSubPlayer[SUB_GUARDIAN])->GetInfo())->m_vPos;
			_vec3 vPos2 = const_cast<Engine::CTransform*>(((CRanger*)m_pSubPlayer[SUB_RANGER])->GetInfo())->m_vPos;
			_vec3 vPos3 = const_cast<Engine::CTransform*>(((CMagician*)m_pSubPlayer[SUB_MAGICIAN])->GetInfo())->m_vPos;

			_myvec vCenterPos = _vec3((vPos1.x + vPos2.x + vPos3.x) / 3.f,
				(vPos1.y + vPos2.y + vPos3.y) / 3.f,
				(vPos1.z + vPos2.z + vPos3.z) / 3.f);

			m_vCenterPos = m_vHealPos = vCenterPos;

			_myvec vDir = vCenterPos - m_pInfo->m_vPos;
			_float fDis = vDir.Length();		

			if(!m_bSkillSet.test(SKILL_4))
			{
				if(m_bTrack == false)
				{
					m_bSkillSet.reset();
					m_bActStateSet.reset(ACT_DASH);
				}
				else
					return;	
			}
			if(fDis > 0.3f)
			{
				m_eAI_StateID = AI_MOVE;
				m_eTrackID = RUN_F;
			}
			else
			{
				m_bSkillSet.set(SKILL_4);
				m_eTrackID = SK_3;
				m_eAI_StateID = AI_SKILL;
			}
		}
	}
	else		/// 지정 힐
	{
		if(m_bCoolTimeSet.test(SKILL_1))	/// 쿨타임 false 일때
		{
			m_pTargetStatus = NULL;
			return;
		}
		for(int i = 0; i < SUB_END; ++i)
		{
			if(const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus())->Get_Percent(75.f) == true)
			{
				_myvec vDir = const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_vPos - m_pInfo->m_vPos;

				if(vDir.Length() < 4.f)
				{
					if(!m_bSkillSet.test(SKILL_1))
					{
						if(m_bTrack == false)
						{
							m_bSkillSet.reset();
							m_bActStateSet.reset(ACT_DASH);
						}
						else
							return;	
					}

					m_pTargetStatus = const_cast<Engine::CStatus*>(((CPlayers*)m_pSubPlayer[i])->GetStatus());
					m_vSubPos = const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_vPos;
					m_fSubAngle = const_cast<Engine::CTransform*>(((CPlayers*)m_pSubPlayer[i])->GetInfo())->m_fAngle[Engine::ANGLE_Y];
					m_eTrackID = SK_0;
					m_bSkillSet.set(SKILL_1);
					m_eAI_StateID = AI_SKILL;
					break;
				}
				else
					m_pTargetStatus = NULL;
			}
			else if (m_pStatus->Get_Percent(75.f) == true)
			{
				if(!m_bSkillSet.test(SKILL_1))
				{
					if(m_bTrack == false)
					{
						m_bSkillSet.reset();
						m_bActStateSet.reset(ACT_DASH);
					}
					else
						return;	
				}
				m_pTargetStatus = m_pStatus;
				m_vSubPos = m_pInfo->m_vPos;
				m_fSubAngle = m_pInfo->m_fAngle[Engine::ANGLE_Y];
				m_eTrackID = SK_0;
				m_bSkillSet.set(SKILL_1);
				m_eAI_StateID = AI_SKILL;
			}
		}
	}
}

void CPaladin::Movement_Wyvern(CMainPlayer* pMainPlayer)
{
	if(!pMainPlayer->Get_Event_Start())
	{
		m_bEventRootSet.set(EVENT_ROOT1);
		_myvec vDir;
		_vec3 vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
		vDir = m_vQuestEnd_Start_Pos - vMyPos;
		_float fDis = vDir.Length();
		vDir.Normalize();
		_float fRadian = acosf(vDir.Dot(&m_vLook));

		if(vMyPos.z > m_vQuestEnd_Start_Pos.z)
			fRadian = -fRadian;
		else
			fRadian = fRadian;

		if(fDis > 0.3f)
		{
			m_pInfo->m_vPos += vDir * (m_fSpeed * 0.01f) * m_fTimeDelta;
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fRadian;
			m_eTrackID = RUN_F;
		}
		else
		{
			m_eTrackID = STAND_NORMAL;
			m_bEventRootSet.reset(EVENT_ROOT1);
			m_bEventRootSet.set(EVENT_ROOT2);
		}
	}
	if(pMainPlayer->Get_Event_Start())
	{
		if(m_bEventRootSet.test(EVENT_ROOT2))
		{
			if(m_bActStateSet.any() ||
				m_bAtkComboSet.any() ||
				m_bSkillSet.any()) 
			{
				m_bSkillSet.reset();
				m_bActStateSet.reset();
				m_bAtkComboSet.reset();
			}
			Engine::Call_EtcFX(Engine::ETC_SMOKE, &m_pInfo->m_vPos, false);
			m_eTrackID = MOUNT_IDLE;
			m_pWyvern->Set_WyvernInfo(m_pInfo->m_vPos, m_pInfo->m_fAngle[Engine::ANGLE_Y]);
			m_pWyvern->Set_TrackID(CWyverns::WY_IDLE);
			m_pWyvern->Set_ActMount(true);
			m_pWyvern->Set_TargetPlayer(this);
			Set_ActBitset(ACT_MOUNT);
			m_bActStateSet.set(ACT_MOUNT);

			m_bEventRootSet.reset(EVENT_ROOT2);
			m_bEventRootSet.set(EVENT_ROOT3);
		}
	}
	if(m_bEventRootSet.test(EVENT_ROOT3))
	{
		if(m_fEventTime[0] <= 1.5f)
			m_fEventTime[0] += m_fTimeDelta;

		if(m_fEventTime[0] > 1.5f)
		{
			if(m_pWyvern->Get_Fly())
			{
				m_pWyvern->Set_TrackID(CWyverns::WY_FLYING_IDLE);
				m_bEventRootSet.reset(EVENT_ROOT3);
				m_bEventRootSet.set(EVENT_ROOT4);
			}
			else
			{
				m_pWyvern->Set_Jump(true);
				m_pWyvern->Set_TrackID(CWyverns::WY_RUN_JUMP_UP);
			}

		}
	}
	if(m_bEventRootSet.test(EVENT_ROOT4))
	{
		_myvec vDir,vNormal,vWyvernDir;
		_vec3 vMywyvern_Pos,vPoint;
		vPoint = m_vQuestEnd_End_Pos;
		vMywyvern_Pos = const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vPos;

		vDir = vPoint - vMywyvern_Pos;
		vWyvernDir = const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vDir;
		vNormal = _vec3(const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vDir.z,const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vDir.y,-const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vDir.x);

		if(m_fFirstDis == 0.f)
			m_fFirstDis = vDir.Length();

		_float fDis = vDir.Length();
		vDir.Normalize();
		vNormal.Normalize();
		_float	fRadian = D3DXToDegree(acosf(vDir.Dot(&vWyvernDir)));
		_float fScala = D3DXVec3Dot(&vDir,&vNormal);

		if(vMywyvern_Pos.z > vPoint.z)
			fRadian = -fRadian;
		else
			fRadian = fRadian;

		_float fTemp = const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y];

		if(fScala < 0.05f && fScala > -0.05f)
			const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y] = const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y];

		else
		{
			if(fScala > 0)
			{
				const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f) * m_fTimeDelta;
				if(!_finite(const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y]))
					const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y] = fTemp;
			}
			if(fScala < 0)
			{
				const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f) * m_fTimeDelta;
				if(!_finite(const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y]))
					const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_fAngle[Engine::ANGLE_Y] = fTemp;
			}
		}


		if(fDis < m_fFirstDis / 2.5f)
		{
			if(const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vPos.y > vPoint.y)
				const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vPos.y -= 1.f * m_fTimeDelta;

			m_pWyvern->Set_TrackID(CWyverns::WY_RUN_JUMP_LOOP);
		}
		if(fDis < m_fFirstDis / 8.f)
		{
			_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_vQuestEnd_End_Pos, L"Terrain");

			if(fHeight + 2.f <= const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vPos.y)
			{
				m_pWyvern->Set_Down(true);
			}
			else
			{
				m_bEventRootSet.reset(EVENT_ROOT4);
				m_bEventRootSet.set(EVENT_ROOT5);
				m_pWyvern->Set_TrackID(CWyverns::WY_JUMP_LAND);
			}
		}
		if(fDis > m_fFirstDis / 2.5f)
		{
			m_pWyvern->Set_TrackID(CWyverns::WY_FLYING_F_FLUTTER);

			if(const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vPos.y < vPoint.y + 20.f)
				const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vPos.y += 3.f * m_fTimeDelta;
		}
		m_pWyvern->Set_Point_Dir(vDir);
		m_pWyvern->Set_bPoint(true);
	}
	if(m_bEventRootSet.test(EVENT_ROOT5))
	{
		if(!m_pWyvern->Get_Fly())
		{
			m_pWyvern->Set_TrackID(CWyverns::WY_IDLE);

			if(m_fEventTime[1] < 1.f)
				m_fEventTime[1] += m_fTimeDelta;

			if(m_fEventTime[1] > 1.f)
			{
				if(m_bActStateSet.test(ACT_MOUNT))
				{
					Engine::Call_EtcFX(Engine::ETC_SMOKE, &const_cast<Engine::CTransform*>(m_pWyvern->GetInfo())->m_vPos, false);
					m_eTrackID = STAND_NORMAL;
					m_pWyvern->Set_TrackID(CWyverns::WY_IDLE);
					m_pWyvern->Set_ActMount(false);
					m_pWyvern->Set_Fly(false);
					m_pWyvern->Set_Jump(false);
					m_pWyvern->Set_JumpAcc_Init(0.f);
					m_bActStateSet.reset(ACT_MOUNT);

					m_bEventRootSet.reset(EVENT_ROOT5);
					m_bEventRootSet.set(EVENT_ROOT6);
				}
			}
		}
	}
	if(m_bEventRootSet.test(EVENT_ROOT6))
	{
		_myvec vDir , vPlayerDir;
		_vec3 vMyPos = m_pInfo->m_vPos;
		vDir = m_vQuestEnd_End_Pos - vMyPos;
		vPlayerDir = m_pInfo->m_vDir;
		_float fDis = vDir.Length();
		vDir.Normalize();
		vPlayerDir.Normalize();
		_float fRadian = acosf(vDir.Dot(&m_vLook));

		if(vMyPos.z > m_vQuestEnd_End_Pos.z)
			fRadian = -fRadian;
		else
			fRadian = fRadian;

		if(fDis > 0.3f)
		{
			m_pInfo->m_vPos += vDir * (m_fSpeed * 0.01f) * m_fTimeDelta;
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fRadian;
			m_eTrackID = RUN_F;
		}
		else
		{
			pMainPlayer->NaviReset();
			m_eTrackID = STAND_NORMAL;
			m_bEventRootSet.set(EVENT_ROOT7);
			m_bEventRootSet.set(EVENT_ROOT6);

		}
	}
	if(m_bEventRootSet.test(EVENT_ROOT7))
	{
		if(pMainPlayer->Get_Event_End())
		{
			m_bEventRootSet.reset();
			m_bEventEnd = true;
		}
	}

}
