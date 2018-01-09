#include "stdafx.h"
#include "Ranger.h"

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
#include "KeyMgr.h"
#include "Magician.h"
#include "Ranger.h"
#include "Transform.h"
#include "WyvernLand.h"
#include "WyvernTwins.h"
#include "DmgView.h"
#include "MainPlayer.h"
#include "SoundMgr.h"
#include "BulletMgr.h"
#include "QuestWin1.h"

CMagician::CMagician(LPDIRECT3DDEVICE9 pDevice,PLAYERJOB eTypeJob)
: CPlayers(pDevice)
, m_bPush(false)
, m_pSphere(NULL)
, m_bConstance(false)
, m_bFirst_Atk(false)
, m_bSecond_Atk(false)
, m_fSkillFour_Time(0.f)
, m_bThunderBolt(false)
, m_vTargetPos(_vec3(0,0,0))
, m_iSkillFour_Cnt(0)
, m_fSkillOne_Time(0.f)
, m_bMeteo(false)
, m_fSkillFive_Time(0.f)
, m_bStarfall(false)
, m_iSkillFive_Cnt(0)
, m_pMainPlayer(NULL)
, m_bStop(false)
{
	m_eTypeJob = eTypeJob;
	m_bCoolTimeSet.reset();
}

CMagician::~CMagician(void)
{
	m_pWyvern = NULL;
	m_pDynamicCamera = NULL;
	m_pMainPlayer = NULL;
	m_pTargetMonster = NULL;
	m_pSphere = NULL;
}


HRESULT CMagician::Initialize(void)
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
	m_pInfo->m_vPos = _vec3(106.f, 0.f, 124.f);

	m_vQuestEnd_Start_Pos = _vec3(124.f,0.f,124.f);
	m_vQuestEnd_End_Pos = _vec3(131.f,21.f,12.f);
	m_fFirstDis = 0.f;

	m_fRange = 25.f;

	m_dwIndex = 35;			/// 네비메쉬 인덱스
	m_fTargetDis = 3.5f;	/// 몬스터 타겟 잡는 거리
	m_pTargetMonster = NULL; /// 타겟 몬스터 포인터
	m_fTargetAngle = 0.f;
	m_bSlowCheck = false;
	m_fAttackSlow = 0.f;
	m_bCancel = false;

	// 위치 잡음
	m_pInfo->m_vPos = _vec3(119.f,2.65f,142.f);
	m_dwIndex = 24;

	D3DXMatrixIdentity(&m_matSaddle);

	//////////////////////////////////////////////////////////////////////////  스킬 쿨타임 맥스치를 정해준다

	for(int i = 0; i < SKILL_END; ++i)
	{
		m_fSkillCooltime[i] = 0.f;
		m_fSkillCooltime_Max[i] = 0.f;
	}

	for(int i = 0; i < 5; ++i)
		m_fConsoleTime[i] = 0.f;

	m_fSkillCooltime_Max[SKILL_1] =	7.f;		/// 메테오
	m_fSkillCooltime_Max[SKILL_2] = 10.f;		/// 고드름
	m_fSkillCooltime_Max[SKILL_3] = 11.f;		/// 노바
	m_fSkillCooltime_Max[SKILL_4] = 15.f;		/// 블러드스톰
	m_fSkillCooltime_Max[SKILL_5] = 13.f;		/// 스타폴
	m_fSkillCooltime_Max[SKILL_DASH] = 15.f;	/// 점멸
	m_fSkillCooltime_Max[SKILL_INNATE] = 10.f;	/// 마나쉴드
	m_fSkillCooltime_Max[SKILL_DEFAULT] = 1.f;  /// 기본공격 3타 //

	return S_OK;
}

_int CMagician::Update(const _float& fTimeDelta)
{
	if(m_bBossCam)
		m_fSpeed = 40.f;
	else if(m_fSpeed != 130.f)
		m_fSpeed = 130.f;

	m_pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front());
	if(m_pMainPlayer == NULL)
		return 0;

	if(m_pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
	{
		if(*m_pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_MAIN && *m_pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_BEFORE)
			return 0;

		if(*m_pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_MAIN && *m_pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_COMPLETE)
		{

		}
	}

	if(S_OK == CBulletMgr::GetInstance()->ReserveSize_Check(CBulletMgr::BTYPE_SPHERE))
		CBulletMgr::GetInstance()->Reserve_Bullet(m_pDevice,CBulletMgr::BTYPE_SPHERE,1);

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
	Thunderbolt();
	Meteo();
	Starfall();
	//ConsoleDebug();

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
		pGameObject = CWyvernTwins::Create(m_pDevice);
		m_pWyvern = ((CWyvernTwins*)pGameObject);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"WyvernLesser 생성 실패");	
		Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Wyvern_3",pGameObject);
	}
	else
		m_matSaddle = *(const_cast<D3DXMATRIX*>(((Engine::CDynamicMesh*)m_pWyvern->GetComponent(L"Mesh"))->FindFrame("Saddle")));

	/// 여기까지 함
	if(!m_bMainPlayer)
	{
		if(!m_pMainPlayer->m_bCtrlbitset.test(CMainPlayer::CTRL_F1))
			CPlayers::TargetSetting(this);
		else
			m_pTargetMonster = NULL;
	}
	else
		CPlayers::TargetSetting(this);

	if(!m_bActStateSet.test(ACT_MOUNT))
	{
		if(m_bActStateSet.test(ACT_STUN))
		{
			if(!m_bActStateSet.test(ACT_DMG_HIT) &&
				!m_bActStateSet.test(ACT_HIT_UP))
				m_eTrackID = CPlayers::STUN;
		}
		if(m_bActStateSet.test(ACT_DMG_HIT))
		{
			if(!m_bActStateSet.test(ACT_STUN) &&
				!m_bActStateSet.test(ACT_HIT_UP))
				m_eTrackID = CPlayers::DMG_F;
		}
		if(m_bActStateSet.test(ACT_HIT_UP))
		{
			if(!m_bActStateSet.test(ACT_STUN) &&
				!m_bActStateSet.test(ACT_DMG_HIT))
				m_eTrackID = CPlayers::HIT_UP_ALL;
		}
	}

	if(m_bMainPlayer)
	{
		UpdateAnimation(m_eTrackID,fTimeDelta);
	}
	else
	{
		// 여다가 AI 체크 던지면됨
		if(m_eAI_StateID == AI_DIE)
		{
			if(m_pTargetMonster != NULL)
				m_pTargetMonster = NULL;

			m_bDead = true;
		}
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

void CMagician::Render(void)
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

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);

		pEffect->SetFloat("g_fSpecRange", 0.5f);	// 범위 설정 값이 높을 수록 범위가 축소된다.
		pEffect->SetFloat("g_fSpecPow", 0.1f);		// 파워 설정 값이 높을 수록 빛의 세기가 세진다.

		m_pMesh->RenderMesh_ForShader(pEffect);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

void CMagician::UpdateAnimation(TRACKID eTrack ,float fTrackTime)
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
		Ani_Skill_01(0.f);
		break;
	case CPlayers::SK_1:
		Ani_Skill_02(0.f);
		break;
	case CPlayers::SK_2:
		Ani_Skill_03(0.f);
		break;
	case CPlayers::SK_3:
		Ani_Skill_04(0.f);
		break;
	case CPlayers::SK_4:
		Ani_Skill_05(0.f);
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
	case CPlayers::SK_CONSTANCE:
		Ani_Constance(0.f);
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
			m_bFirst_Atk = false;
			m_bSecond_Atk = true;
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
		case SK_CONSTANCE:
			m_eTrackID = STAND_NORMAL;
			m_fPeriod = 0.3f;
			m_fAcc = 0.f;
			m_fSlowTime = 1.f;
			m_bConstance = false;

			if(m_bActStateSet.test(ACT_DASH))
				m_bActStateSet.reset(ACT_DASH);

			if(m_bSkillSet.test(SKILL_DASH))
				m_bSkillSet.reset(SKILL_DASH);

			break;
		}
		m_pMesh->SetAnimationSet(m_eTrackID);
	}
}

void CMagician::AI_Stand()
{
	if(NULL != m_pTargetMonster)
	{

	}
	else
		m_eTrackID = STAND_NORMAL;
}
void CMagician::AI_Patrol()
{
	if(m_bSkillSet.test(SKILL_1) ||
		m_bSkillSet.test(SKILL_4) || 
		m_bSkillSet.test(SKILL_2) ||
		m_bSkillSet.test(SKILL_3))
		return;

	if(m_pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
	{
		Engine::CGameObject* pGameObject = Engine::Get_ObjList(LAYER_UI, L"QuestWin1")->front();
		if(pGameObject == NULL)
			return;

		if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_MAGICIAN) == false)
			m_bStop = true;
		else
			m_bStop = false;
	}

	if(m_pTargetMonster == NULL)
		Movement_NonTarget();

	if(m_pTargetMonster != NULL)
		Movement_Target();

}
void CMagician::AI_Combat()
{
	
	/// 전투 상태

	/// 어떤 공격을 할 건지 판단을 거리로 체크한다

	/// TargetDis  현재 플레이어 타겟 과의 고정 거리
	/// CurrentDis 실제 플레이어와 타겟의 거리 

	


	if(true == m_bConstance)
	{
		m_eTrackID = SK_CONSTANCE;
		return;
	}
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
	}
	if(m_bSkillSet.test(SKILL_1) ||
		m_bSkillSet.test(SKILL_2) ||
		m_bSkillSet.test(SKILL_3) ||
		m_bSkillSet.test(SKILL_4) ||
		m_bSkillSet.test(SKILL_5))
	{
		if(m_bAtkComboSet.count() != 0)
			m_bAtkComboSet.reset();

		return;
	}

	if(m_eAI_StateID == AI_DASH)
	{
		if(m_bTrack == true)
			return;
	}

	if(m_fCurrentDis <= m_fTargetDis)
	{
		if(m_fCurrentDis < 2.f)
		{
			if(m_bAtkComboSet.any())
			{
				if(m_bTrack == true)
					return;
				else
					m_bAtkComboSet.reset();
			}

			if(m_bStop == false)
			{
				if(!m_bCoolTimeSet.test(SKILL_4))	/// 
				{
					//m_bCoolTimeSet.set(SKILL_5);	/// 쿨타임 적용
					m_bSkillSet.set(SKILL_4);  /// 상태 적용
					m_eTrackID = SK_3;				/// 트랙 적용
					m_eAI_StateID = AI_SKILL;
				}
				else
				{
					if(m_bTrack == true)
						return;

					m_eAI_StateID = AI_TARGET_MOVE;
					return;
				}
			}
			
		}
		if(m_fCurrentDis > 2.5f && m_fCurrentDis <= m_fTargetDis)
		{
			if(!m_bCoolTimeSet.test(SKILL_INNATE))
			{
				if(m_bTrack == true)
					return;

				m_eTrackID = HIDDEN_SKILL;
				m_eAI_StateID = AI_SKILL;
				m_bSkillSet.set(SKILL_INNATE);
			}
		}
		if(m_fCurrentDis <= 2.5f)
		{
			if(!m_bSkillSet.any() || !m_bCoolTimeSet.any())
			{
				if(m_bSkillSet.test(SKILL_1))
				{
					if(m_bTrack == true)
						return;
				}
				if(m_bTrack == true)
					return;

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
		if(m_fCurrentDis < 2.5f)
		{
			if(m_bTrack == true)
				return;

			if(m_bAtkComboSet.any())
			{
				if(!m_bCoolTimeSet.test(SKILL_DEFAULT))	/// 공격중 중간에 캔슬 났을때
					m_bAtkComboSet.reset();

				if(m_bCoolTimeSet.count() > 3)			/// 한번에 스킬 연속으로 몇개나 나가게 할지 
					return;
			}
			if(!m_bSkillSet.test(SKILL_INNATE))
			{
				if(!m_bCoolTimeSet.test(SKILL_5))	/// 
				{
					if(!m_bSkillSet.test(SKILL_2))
					{
						//m_bCoolTimeSet.set(SKILL_5);	/// 쿨타임 적용
						m_bSkillSet.set(SKILL_5);  /// 상태 적용
						m_eTrackID = SK_4;				/// 트랙 적용
						m_eAI_StateID = AI_SKILL;
					}
				}
				if(!m_bCoolTimeSet.test(SKILL_3))	
				{
					if(!m_bSkillSet.test(SKILL_5))
					{
						//m_bCoolTimeSet.set(SKILL_5);	/// 쿨타임 적용
						m_bSkillSet.set(SKILL_3);  /// 상태 적용
						m_eTrackID = SK_2;				/// 트랙 적용
						m_eAI_StateID = AI_SKILL;
					}
				}
				if(!m_bCoolTimeSet.test(SKILL_2))	
				{
					if(!m_bSkillSet.test(SKILL_3))
					{
						//m_bCoolTimeSet.set(SKILL_5);	/// 쿨타임 적용
						m_bSkillSet.set(SKILL_2);		/// 상태 적용
						m_eTrackID = SK_1;				/// 트랙 적용
						m_eAI_StateID = AI_SKILL;
					}
				}
				if(!m_bCoolTimeSet.test(SKILL_1))	
				{
					if(!m_bSkillSet.test(SKILL_2))
					{
						m_eTrackID = SK_0;
						m_eAI_StateID = AI_SKILL;
						m_bSkillSet.set(SKILL_1);
					}
				}
			}
		}
		if(!m_bSkillSet.any() && m_fCurrentDis > 2.5f && m_fCurrentDis <= m_fTargetDis)
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
				if(m_bTrack == true)
					return;

				m_eAI_StateID = AI_TARGET_MOVE;
			}
		}
	}

}

void CMagician::AI_Move()
{
	if(m_eAI_StateID ==	AI_TARGET_MOVE)
	{
		//m_dwIndex = Engine::MoveOnNaviMesh(&m_pInfo->m_vPos,&(m_vTargetDir * (m_fSpeed * 0.01f) * m_fTimeDelta),m_dwIndex,m_fHeight);
		//m_pInfo->m_vPos += (m_vTargetDir * (m_fSpeed * 0.01f) * m_fTimeDelta);
		if(m_fCurrentDis > 2.5f)
		{
			MoveOnNaviMesh(&(m_vTargetDir * (m_fSpeed * 0.01f) * m_fTimeDelta));
			m_eTrackID = RUN_F;
		}
		if(m_fCurrentDis < 2.f)
		{
			MoveOnNaviMesh(&(-m_vTargetDir * (m_fSpeed * 0.005f) * m_fTimeDelta));
			m_eTrackID = RUN_B;
		}
	}
	if(m_eAI_StateID == AI_MOVE)
		m_eTrackID = RUN_F;


}
void CMagician::AI_State()
{

}

void CMagician::UpdateState_AI()
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

void CMagician::ConsoleDebug()
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
}
void CMagician::Thunderbolt()
{
	if(m_bThunderBolt == true)
	{
		if(m_fSkillFour_Time == 0.f)
		{
			if(m_iSkillFour_Cnt < 6)
			{
				_float fDMG = (_float)CRandom_Number::Get_RandomNum(2,3);
				RangeSkill_State(SKILL_4,m_vEffectPos[SKILL_4],fDMG,3.f,this);
				Engine::Call_MagicianFX(Engine::M_SKILL4,&m_vEffectPos[SKILL_4],false);
			}
			else
			{
				m_fSkillFour_Time = 0.f;
				m_iSkillFour_Cnt = 0;
				m_bThunderBolt = false;
			}
		}
		if(m_fSkillFour_Time > 0.5f)
		{
			++m_iSkillFour_Cnt;
			m_fSkillFour_Time = 0.f;
		}
		else
			m_fSkillFour_Time += m_fTimeDelta;	

	}
}

void CMagician::Meteo()
{

	if(m_bMeteo == true)
	{
		if(m_fSkillOne_Time >= 2.5f)
		{
			_float fDMG = (_float)CRandom_Number::Get_RandomNum(5,3);
			RangeSkill_State(SKILL_3,m_vEffectPos[SKILL_1],fDMG,3.f,this);
			Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_vEffectPos[SKILL_1],fDMG));
			m_fSkillOne_Time = 0.f;
			m_bMeteo = false;
		}
		else
			m_fSkillOne_Time += m_fTimeDelta;
	}	 
}

void CMagician::Starfall()
{
	if(m_bStarfall == true)
	{
		if(m_fSkillFive_Time == 0.f)
		{
			if(m_iSkillFive_Cnt <= 5)
			{
				_float fDMG = (_float)CRandom_Number::Get_RandomNum(1,3);
				RangeSkill_State(SKILL_5,m_vEffectPos[SKILL_5],fDMG,2.5f,this);
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_vEffectPos[SKILL_5],fDMG));
				Engine::Call_MagicianFX(Engine::M_SKILL5,&m_vEffectPos[SKILL_5],false);
			}
			else
			{
				m_fSkillFive_Time = 0.f;
				m_iSkillFive_Cnt = 0;
				m_bStarfall = false;
			}
		}
		if(m_fSkillFive_Time > 0.5f)
		{
			++m_iSkillFive_Cnt;
			m_fSkillFive_Time = 0.f;
		}
		else
			m_fSkillFive_Time += m_fTimeDelta;
	}
}

bool CMagician::Get_First_Atk()
{
	return m_bFirst_Atk;
}

bool CMagician::Get_Second_Atk()
{
	return m_bSecond_Atk;
}

HRESULT CMagician::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_Magician");
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
	pBaseStat->m_fHealth = 250.f;
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
void CMagician::Set_ContantTable(LPD3DXEFFECT	pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CMagician* CMagician::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMagician*		pGameObject = new CMagician(pDevice,JOB_MAGICIAN);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CMagician::Free(void)
{
	CPlayers::Free();
	//Engine::CGameObject::Free();
}
////////////////////////////////////////////////////////////////////////// 애니메이션 관련 함수

void CMagician::Ani_Run(float fTrackTime)
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

void CMagician::Ani_Attack(float fTrackTime)
{//마법사 평타 완성되면 가디언이나 팔라딘 평타처럼 사운드 위치 변경바람.
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

	if(m_eTrackID == CPlayers::ATK_0)
	{
		if(fTrackTime < 0.25f)
			m_fSlowTime = 1.5f;
		else
			m_fSlowTime = 0.7f;

		if(fTrackTime >= 0.25f)
		{
			if(m_bCancel == false && m_pTargetMonster != NULL)
			{
				WORD wCnt = CBulletMgr::GetInstance()->ReadyBullet_Count(CBulletMgr::BTYPE_SPHERE);
				CBulletMgr::GetInstance()->Sphere_Trans(CBulletMgr::BTYPE_SPHERE,0,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos,m_pTargetMonster);
				
				if(m_bSecond_Atk == true)
					m_bSecond_Atk = false;
				if(m_bFirst_Atk == true)
					m_bFirst_Atk = false;

				m_bFirst_Atk = true;
				m_bCancel = true;
			}
		}

		if(m_bMainPlayer)
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			{
				CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianAtt1.ogg", 0.5f);
				CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianAttEffect1.ogg", 0.5f);
			}
		}
		else
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f && m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianAtt1.ogg", m_fSoundVolume);
				CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianAttEffect1.ogg", m_fSoundVolume);
			}
		}
	}
	if(m_eTrackID == CPlayers::ATK_1)
	{
		if(fTrackTime < 0.5f)
			m_fSlowTime = 1.8f;
		else
			m_fSlowTime = 0.8f;

		if(fTrackTime >= 0.8f)
		{
			if(m_bCancel == false && m_pTargetMonster != NULL)
			{
				m_bFirst_Atk = false;
				m_bCancel = true;
			}
		}
		if(m_bMainPlayer)
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
			{
				CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianAtt2.ogg", 0.5f);
				CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianAttEffect2.ogg", 0.5f);
			}
		}
		else
		{

			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f && m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianAtt2.ogg", m_fSoundVolume);
				CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianAttEffect2.ogg", m_fSoundVolume);
			}
		}
	}
	if(m_eTrackID == CPlayers::ATK_2)
	{
		if(fTrackTime < 0.35f)
			m_fSlowTime = 2.2f;
		else
			m_fSlowTime = 1.f;

		if(m_bSecond_Atk == false)
			m_bSecond_Atk = true;

		if(fTrackTime >= 0.5f)
		{
			if(m_bCancel == false && m_pTargetMonster != NULL)
			{
				m_bCancel = true;
			}
		}
		

		if(m_bMainPlayer)
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
				CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianAtt3.ogg", 0.5f);
		}
		else
		{
			if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f && m_fSoundTargetDistance < 6.f)
			{
				CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianAtt3.ogg", m_fSoundVolume);
			}
		}
	}

	//m_ePrev_TrackID = m_eTrackID;

}
void CMagician::Ani_Skill_01(float fTrackTime)
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

	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill1_1.ogg", 0.5f);
	}
	else
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f && m_fSoundTargetDistance < 6.f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill1.ogg", m_fSoundVolume);
		}
	}

	if(!m_bCoolTimeSet.test(SKILL_1))
		m_bCoolTimeSet.set(SKILL_1);

	if(fTrackTime >= 0.5f && m_bCancel == false)
	{
		if(NULL != m_pTargetMonster)
		{
			if(m_bCancel == false)
			{
				m_fSkillAngle_Effect[SKILL_1] = const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f);
				m_vEffectPos[SKILL_1] = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,
					const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + g_fEffect_Y,
					const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);

				m_bMeteo = true;

				Engine::Call_MagicianFX(Engine::M_SKILL1,&m_vEffectPos[SKILL_1] ,false);
				if(m_bMainPlayer)
				{
					CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect1_1.ogg", 0.5f);
				}
				else
				{
					if(m_fSoundTargetDistance < 6.f)
						CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect1_1.ogg", m_fSoundVolume);
				}
				m_bCancel = true;
				m_bSlowCheck = true;
			}
		}
	}


}
void CMagician::Ani_Skill_02(float fTrackTime)
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
	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill2.ogg", 0.5f);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect2.ogg", 0.5f);
		}
	}
	else
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f && m_fSoundTargetDistance < 6.f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill2.ogg", m_fSoundVolume);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect2.ogg", m_fSoundVolume);
		}
	}

	if(!m_bCoolTimeSet.test(SKILL_2))
		m_bCoolTimeSet.set(SKILL_2);


	if(fTrackTime >= 0.3f && m_bCancel == false)
	{
		if(NULL != m_pTargetMonster)
		{
			if(m_bCancel == false)
			{
				_float fDMG = (_float)CRandom_Number::Get_RandomNum(3,3);
				_vec3 vPos = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + g_fEffect_Y,m_pInfo->m_vPos.z);

				m_fSkillAngle_Effect[SKILL_2] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f));
				
				m_pTargetMonster->Set_DMG_Check(fDMG,(CUnits*)this,Engine::CStatus::DMG_MELEE);
				const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &vPos,fDMG));
				Engine::Call_MagicianFX(Engine::M_SKILL2,&vPos,false, NULL, &m_fSkillAngle_Effect[SKILL_2]);

				if(m_bMainPlayer)
				{
					CSoundMgr::GetInstance()->PlayMagicianSound3(L"MagicianEffectHit2.ogg", 0.5f);
				}
				else
				{
					if(m_fSoundTargetDistance < 6.f)
						CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect2.ogg", m_fSoundVolume);

				}
				m_bCancel = true;
				m_bSlowCheck = true;
			}
		}
	}


}
void CMagician::Ani_Skill_03(float fTrackTime)
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

	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill3.ogg", 0.5f);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect3.ogg", 0.5f);
		}
	}
	else
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f && m_fSoundTargetDistance < 6.f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill3.ogg", m_fSoundVolume);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect3.ogg", m_fSoundVolume);
		}
	}

	if(!m_bCoolTimeSet.test(SKILL_3))
		m_bCoolTimeSet.set(SKILL_3);

	if(fTrackTime >= 0.2f && m_bCancel == false)
	{
		if(NULL != m_pTargetMonster)
		{
			if(m_bCancel == false)
			{

				_float fDMG = (_float)CRandom_Number::Get_RandomNum(3,3);

				_vec3 vPos = _vec3(m_pInfo->m_vPos.x,m_pInfo->m_vPos.y + g_fEffect_Y,m_pInfo->m_vPos.z);
				RangeSkill_State(SKILL_3,vPos,fDMG,2.f,this);
				Engine::Call_MagicianFX(Engine::M_SKILL3,&vPos,false);

				if(m_bMainPlayer)
					CSoundMgr::GetInstance()->PlayMagicianSound3(L"MagicianEffectHit3.ogg", 0.5f);
				else
				{
					if(m_fSoundTargetDistance < 6.f)
						CSoundMgr::GetInstance()->PlayMagicianSound3(L"MagicianEffect3.ogg", m_fSoundVolume);
				}
				m_bCancel = true;
				m_bSlowCheck = true;
			}
		}
	}

}

void CMagician::Ani_Skill_04(float fTrackTime)
{
	if(m_eOriTrack != m_eTrackID)
	{
		if(m_bCancel == true)
			m_bCancel = false;

		if((float)m_pMesh->GetTrackPosition() == 0.f)
			fTrackTime = (float)m_pMesh->GetTrackPosition();

		if(m_pTargetMonster != NULL)
		{
			m_vEffectPos[SKILL_4] = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,
				const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + g_fEffect_Y,
				const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
		}
		else
		{
			_vec3 vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
			m_vEffectPos[SKILL_4] = vMyPos + m_pInfo->m_vDir * 20.f;
		}

		Engine::Call_MagicianFX(Engine::M_SKILL4_FLOOR,&m_vEffectPos[SKILL_4],false);
		
		if(m_bMainPlayer)
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect4.ogg", 0.5f);
		else
		{
			if(m_fSoundTargetDistance < 6.f)
				CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect4.ogg", m_fSoundVolume);
		}
	}
	else
		fTrackTime = (float)m_pMesh->GetTrackPosition();

	m_pMesh->SetAnimationSet(m_eTrackID);
	m_bTrack = true;

	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.2f && (float)m_pMesh->GetTrackPosition() < 0.25f)
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill4_1.ogg", 0.5f);
		if((float)m_pMesh->GetTrackPosition() > 5.8f && (float)m_pMesh->GetTrackPosition() < 5.85f)
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill4_2.ogg", 0.5f);
	}
	else
	{
		if((float)m_pMesh->GetTrackPosition() > 0.2f && (float)m_pMesh->GetTrackPosition() < 0.25f && m_fSoundTargetDistance < 6.f)
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill4_1.ogg", m_fSoundVolume);
		if((float)m_pMesh->GetTrackPosition() > 5.8f && (float)m_pMesh->GetTrackPosition() < 5.85f && m_fSoundTargetDistance < 6.f)
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill4_2.ogg", m_fSoundVolume);
	}

	if(!m_bCoolTimeSet.test(SKILL_4))
		m_bCoolTimeSet.set(SKILL_4);


	if(fTrackTime >= 5.5f && m_bCancel == false)
	{
		if(NULL != m_pTargetMonster)
		{
			if(m_bCancel == false)
			{
				m_bThunderBolt = true;
				m_bCancel = true;
				m_bSlowCheck = true;
			}
		}
	}


	//공격전 (batk = false 한번만)->공격 -> 해당시간대 들어오면 batk = true -> 끝나면 batk = false 

}

void CMagician::Ani_Skill_05(float fTrackTime)
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

	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill5.ogg", 0.5f);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect5.ogg", 0.5f);
		}
	}
	else
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.1f && m_fSoundTargetDistance < 6.f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianSkill5.ogg", m_fSoundVolume);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect5.ogg", m_fSoundVolume);
		}
	}


	if(!m_bCoolTimeSet.test(SKILL_5))
		m_bCoolTimeSet.set(SKILL_5);

	if(fTrackTime >= 0.1f && m_bCancel == false)
	{
		if(NULL != m_pTargetMonster)
		{
			if(m_bCancel == false)
			{
				m_fSkillAngle_Effect[SKILL_5] = (m_pInfo->m_fAngle[Engine::ANGLE_Y] + D3DXToRadian(90.f));
				m_vEffectPos[SKILL_5] = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,
					const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.y + g_fEffect_Y,
					const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
				
				m_bStarfall = true;

				if(m_bMainPlayer)
					CSoundMgr::GetInstance()->PlayMagicianSound3(L"MagicianEffectHit5.ogg", 0.5f);
				else
				{
					if(m_fSoundTargetDistance < 6.f)
						CSoundMgr::GetInstance()->PlayMagicianSound3(L"MagicianEffectHit5.ogg", m_fSoundVolume);

				}
				m_bCancel = true;
				m_bSlowCheck = true;
			}
		}
	}

}
void CMagician::Ani_Skill_Dash(float fTrackTime)
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

	if(m_bMainPlayer)
	{
		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianDash.ogg", 0.5f);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect6.ogg", 0.5f);
		}
	}
	else
	{

		if((float)m_pMesh->GetTrackPosition() > 0.f && (float)m_pMesh->GetTrackPosition() < 0.05f && m_fSoundTargetDistance < 6.f)
		{
			CSoundMgr::GetInstance()->PlayMagicianSound1(L"MagicianDash.ogg", m_fSoundVolume);
			CSoundMgr::GetInstance()->PlayMagicianSound2(L"MagicianEffect6.ogg", m_fSoundVolume);
		}
	}

	if(!m_bCoolTimeSet.test(SKILL_DASH))
		m_bCoolTimeSet.set(SKILL_DASH);

	if(m_bActStateSet.test(ACT_DASH))
	{
		m_fSlowTime = 1.3f;
		//m_pInfo->m_vPos += (m_pInfo->m_vDir * m_fTimeDelta *  (m_fSpeed * 4.f));

		if(m_bEvasion_Check == false)
			MoveOnNaviMesh(&(m_pInfo->m_vDir * m_fTimeDelta * (m_fSpeed * 4.f)));
	}


}

void CMagician::Ani_Skill_Innate(float fTrackTime)
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

	if(m_bCancel == false)
	{
		Engine::Call_MagicianFX(Engine::M_INNATE,&m_pInfo->m_vPos,true,NULL,NULL,NULL);
		m_bCancel = true;
	}

	if(!m_bCoolTimeSet.test(SKILL_INNATE))
		m_bCoolTimeSet.set(SKILL_INNATE);

	if(m_bActStateSet.test(ACT_EVASION))
	{
		m_fSlowTime = 1.5f;
	}
}

void CMagician::Ani_Jump(float fTrackTime)
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

void CMagician::Ani_Hit(float fTrackTime)
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

	if(fTrackTime <= 0.8f)
		MoveOnNaviMesh(&(-m_vTargetDir * (1.f) * m_fTimeDelta),this); 
}

void CMagician::Ani_Down(float fTrackTime)
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

void CMagician::Ani_Die(float fTrackTime)
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

void CMagician::Ani_DeadBody(float fTrackTime)
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

void CMagician::Ani_Stand(float fTrackTime)
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

void CMagician::Ani_DMG_B(float fTrackTime)
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

void CMagician::Ani_DMG_F(float fTrackTime)
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

void CMagician::Ani_Stun(float fTrackTime)
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

void CMagician::Ani_Mount(float fTrackTime)
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

void CMagician::Ani_Constance(float fTrackTime)
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

void CMagician::Movement_NonTarget()
{
	////////////////////////////////////////////////////////////////////////// 타겟이 없다


	Engine::CGameObject* pGameObject = Engine::Get_ObjList(LAYER_UI, L"QuestWin1")->front();
	if(pGameObject == NULL)
		return;

	if(m_pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
	{
		if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_MAGICIAN) == false)
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
	vPlayerDir.Normalize();
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

void CMagician::Movement_Target()
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
	if(m_fCurrentDis < m_fTargetDis)
	{
		/// 타겟이 범위 안에 들어와 있다
		AI_Combat();	
	}
}

void CMagician::Movement_Wyvern(CMainPlayer* pMainPlayer)
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
