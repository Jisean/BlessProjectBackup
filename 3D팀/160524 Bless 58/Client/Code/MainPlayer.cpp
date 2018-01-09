#include "stdafx.h"
#include "MainPlayer.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Calculator.h"
#include "Material.h"
#include "Status.h"
#include "KeyMgr.h"
#include "DynamicCamera.h"
#include "Stage.h"
#include "Sphere.h"
#include "QuestWin1.h"
#include "SoundMgr.h"
#include "BalianBollwerk.h"
#include "AIMgr.h"


CMainPlayer::CMainPlayer(LPDIRECT3DDEVICE9 pDevice)
: CPlayers(pDevice)
, m_pPlayer(NULL)
, m_eMainPlayer(JOB_GUARDIAN)
, m_pAim(NULL)
, m_bCheck(false)
, m_eQuestType(QUESTTYPE_MAIN)
, m_eQuestStep(QUEST_BEFORE)
, m_eScene(SCENE_END)
, m_bTargetEff(false)
, m_vEventPos_Start(_vec3(126.f,0.f,128.f))
, m_vEventPos_End(_vec3(132.f, 21.f, 17.f))
, m_bEvent_Start(false)
, m_bEvent_End(false)
, m_bNaviStart(false)
{
   ZeroMemory(m_pPlayerSet, sizeof(CPlayers*) * JOB_END);

   for(int i = 0; i < JOB_END; ++i)
      m_pPlayerSet[i] = NULL;

   for(int i = 0; i < 3; ++i)
      m_fSubAngle[i] = 0.f;

   for(int i = 0; i < 4; ++i)
      m_vNaviPos[i] = _vec3(0,0,0);

   for(int i = 0; i < 60; ++i)
      m_vCatMullromPos[i] = _vec3(0,0,0);
}

CMainPlayer::~CMainPlayer(void)
{
   

   m_pPlayer = NULL;
   m_pMainPlayer = NULL;
}

HRESULT CMainPlayer::Initialize(void)
{

   m_vNaviPos[0] = _vec3(m_vEventPos_Start.x, 2.65f, m_vEventPos_Start.z);
   m_vNaviPos[2] = _vec3(129.37f, 34.8f, 62.14f);
   m_vNaviPos[1] = _vec3(128.09f, 29.8f, 84.16f);
   m_vNaviPos[3] = m_vEventPos_End;
	
   m_bCtrlbitset.reset();


   return S_OK;
}

_int CMainPlayer::Update(const _float& fTimeDelta)
{
   if(Engine::GetDIMouseState(Engine::CInput::DIM_MBUTTON) & 0x80)
      NaviReset();

   if(m_eScene == SCENE_STAGE2)
   {
      if(CKeyMgr::GetInstance()->GetKeyDown(VK_SPACE))
      {
         vector<CNPCs*>::iterator iternpc = CAIMgr::GetInstance()->Get_NPCInfo()->begin();
         vector<CNPCs*>::iterator iternpc_end = CAIMgr::GetInstance()->Get_NPCInfo()->end();

         for(; iternpc != iternpc_end; ++iternpc)
         {
            _vec3 vDist = (*iternpc)->GetInfo()->m_vPos - m_pPlayer->GetInfo()->m_vPos;
            float fDist = D3DXVec3Length(&vDist);

            if(fDist <= 1.f)
			{
               (*iternpc)->Set_bDungeonNPC(true);
				
			   m_vStage2SaveQ = &const_cast<Engine::CTransform*>((*iternpc)->GetInfo())->m_vPos;
			   Engine::Call_EtcFX(Engine::ETC_QUEST, m_vStage2SaveQ, true);
			}
         }
      }
   }
   
   m_fTimeDelta = fTimeDelta;
	ControlPlayers();
   ChangePlayer();

   KeyInput_Static_Wyvern(fTimeDelta,m_pPlayer);

   if(m_pPlayer->Test_ActBitSet(ACT_MOUNT))
      KeyCheck_To_Wyvern(fTimeDelta,m_pPlayer);
   else
      KeyCheck(fTimeDelta, m_pPlayer);

   m_pPlayer->Set_TrackID(m_eTrackID);

   // debug용
   if(CKeyMgr::GetInstance()->GetKeyDown('7'))   
   {
      for(int i = 0; i < FOR_DEFAULT; ++i)
      {
         if(m_pPlayerSet[i] == m_pPlayer)
            continue;

         const_cast<Engine::CTransform*>(m_pPlayerSet[i]->GetInfo())->m_vPos = const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos;
      }
   }

   if(CKeyMgr::GetInstance()->GetKeyDown('8'))   
   {
	   const Engine::CTransform* pInfo = m_pPlayer->GetInfo();
	   const_cast<Engine::CTransform*>(pInfo)->m_vPos = D3DXVECTOR3(115.887f, 1.867f, 154.976f);      
   }


#ifdef _DEBUG
   
   if(CKeyMgr::GetInstance()->GetStateKey(VK_PRIOR))   
   {
      cout << m_pPlayer->Get_SpecPower() << endl;
      m_pPlayer->Set_SpecPower( 1.f * fTimeDelta);
   }
   if(CKeyMgr::GetInstance()->GetStateKey(VK_NEXT))
   {
      cout << m_pPlayer->Get_SpecPower() << endl;
      m_pPlayer->Set_SpecPower(-1.f * fTimeDelta);
   }
   if(CKeyMgr::GetInstance()->GetStateKey(VK_HOME))
   {
      cout << m_pPlayer->Get_SpecRange() << endl;
      m_pPlayer->Set_SpecRange((1.f  * fTimeDelta) * 2.f);
   }
   if(CKeyMgr::GetInstance()->GetStateKey(VK_END))
   {
      cout << m_pPlayer->Get_SpecRange() << endl;
      m_pPlayer->Set_SpecRange((-1.f * fTimeDelta) * 2.f);
   }
#endif

   if(m_eScene == CMainPlayer::SCENE_STAGE1)
   {
      if(m_eQuestType == QUESTTYPE_MAIN && m_eQuestStep == QUEST_COMPLETE)
         EventFunction();

      if(m_eQuestType == QUESTTYPE_MAIN && m_eQuestStep == QUEST_PROGRESS)
         NaviEffect();
   }
   
   AI_Position();
   MainQuestCheck();
   MainQuestAccept();

   _int iResult = 0;
   return iResult;
}
void CMainPlayer::AI_Position()
{
   for(int i = 0; i < 3; ++i)
   {
      _myvec vDir = ((Engine::CTransform*)m_pPlayer->GetComponent(L"Transform"))->m_vDir;
      vDir.Normalize();

      _mymat matRotY;
      _float fConstance = 0.f;
      _float fFixed = 0.f;
      matRotY.Identity();

      switch(i)
      {
      case 0:
         fConstance = 1.f;
         fFixed = 1.5f;
         break;
      case 1:
         fConstance = 1.4f;
         fFixed = 1.2f;
         break;
      case 2:
         fConstance = 2.f;
         fFixed = 1.f;
         break;
      default:
         break;
      }
   
      matRotY.RotY(D3DXToRadian(180.f) / fConstance);

      _vec3 vTargetPos(0,0,0);
      _vec3 vTargetDir(0,0,0);

      D3DXVec3TransformNormal(&vTargetDir,&vDir,&matRotY);

      if(i == 1)
         vTargetPos = ((Engine::CTransform*)m_pPlayer->GetComponent(L"Transform"))->m_vPos + vTargetDir / (fFixed);// * (2.f * fFixed); // 상수값은 중심기준으로의 인터벌 ..
      else
         vTargetPos = ((Engine::CTransform*)m_pPlayer->GetComponent(L"Transform"))->m_vPos + vTargetDir / (fFixed + 0.1f); //* (2.f / fFixed); // 상수값은 중심기준으로의 인터벌 ..
      
      m_vFormation[i] = vTargetPos;

      _vec3 vPlayerPos = ((Engine::CTransform*)m_pPlayer->GetComponent(L"Transform"))->m_vPos;
   }

   if(m_pPlayer->Get_PlayerJob() == CPlayers::JOB_GUARDIAN)
   {   
      m_pPlayerSet[JOB_RANGER]->SetFormation(m_vFormation[0]);
      m_pPlayerSet[JOB_PALADIN]->SetFormation(m_vFormation[2]);
      m_pPlayerSet[JOB_MAGICIAN]->SetFormation(m_vFormation[1]);


      if(m_pPlayer->Test_KeyBitSet(CPlayers::STAKEY_MOVE))
      {
         if(m_pPlayerSet[JOB_RANGER]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_PALADIN]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_MAGICIAN]->Get_AIstate() == CPlayers::AI_STAND )
         {
            if(false == m_pPlayer->Get_Hold())
               m_pPlayer->Set_Hold(true);
         }
         
      }
      else if(m_pPlayer->Get_TrackID() == CPlayers::STAND_NORMAL)
         m_pPlayer->Set_Hold(false);

      for(int i = 0; i < JOB_END; ++i)
      {
         if(i != CPlayers::JOB_GUARDIAN)
            m_pPlayerSet[i]->Set_MainPlayer(m_pPlayer);
      }
   }
   
   if(m_pPlayer->Get_PlayerJob() == CPlayers::JOB_PALADIN)
   {
      m_pPlayerSet[JOB_MAGICIAN]->SetFormation(m_vFormation[0]);
      m_pPlayerSet[JOB_RANGER]->SetFormation(m_vFormation[1]);
      m_pPlayerSet[JOB_GUARDIAN]->SetFormation(m_vFormation[2]);

      if(m_pPlayer->Test_KeyBitSet(CPlayers::STAKEY_MOVE))
      {
         if(m_pPlayerSet[JOB_MAGICIAN]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_RANGER]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_GUARDIAN]->Get_AIstate() == CPlayers::AI_STAND )
         {
            if(false == m_pPlayer->Get_Hold())
               m_pPlayer->Set_Hold(true);
         }
      }
      else if(m_pPlayer->Get_TrackID() == CPlayers::STAND_NORMAL)
         m_pPlayer->Set_Hold(false);

      for(int i = 0; i < JOB_END; ++i)
      {
         if(i != CPlayers::JOB_PALADIN)
            m_pPlayerSet[i]->Set_MainPlayer(m_pPlayer);
      }

   }
   if(m_pPlayer->Get_PlayerJob() == CPlayers::JOB_MAGICIAN)
   {
      m_pPlayerSet[JOB_RANGER]->SetFormation(m_vFormation[0]);
      m_pPlayerSet[JOB_PALADIN]->SetFormation(m_vFormation[1]);
      m_pPlayerSet[JOB_GUARDIAN]->SetFormation(m_vFormation[2]);

      if(m_pPlayer->Test_KeyBitSet(CPlayers::STAKEY_MOVE))
      {
         if(m_pPlayerSet[JOB_RANGER]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_PALADIN]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_GUARDIAN]->Get_AIstate() == CPlayers::AI_STAND )
         {
            if(false == m_pPlayer->Get_Hold())
               m_pPlayer->Set_Hold(true);
         }
      }
      else if(m_pPlayer->Get_TrackID() == CPlayers::STAND_NORMAL)
         m_pPlayer->Set_Hold(false);


      for(int i = 0; i < JOB_END; ++i)
      {
         if(i != CPlayers::JOB_MAGICIAN)
            m_pPlayerSet[i]->Set_MainPlayer(m_pPlayer);      
      }
   }
   if(m_pPlayer->Get_PlayerJob() == CPlayers::JOB_RANGER)
   {
      m_pPlayerSet[JOB_MAGICIAN]->SetFormation(m_vFormation[0]);
      m_pPlayerSet[JOB_PALADIN]->SetFormation(m_vFormation[1]);
      m_pPlayerSet[JOB_GUARDIAN]->SetFormation(m_vFormation[2]);

      if(m_pPlayer->Test_KeyBitSet(CPlayers::STAKEY_MOVE))
      {
         if(m_pPlayerSet[JOB_MAGICIAN]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_PALADIN]->Get_AIstate() == CPlayers::AI_STAND &&
            m_pPlayerSet[JOB_GUARDIAN]->Get_AIstate() == CPlayers::AI_STAND )
         {
            if(false == m_pPlayer->Get_Hold())
               m_pPlayer->Set_Hold(true);
         }
      }
      else if(m_pPlayer->Get_TrackID() == CPlayers::STAND_NORMAL)
         m_pPlayer->Set_Hold(false);


      for(int i = 0; i < JOB_END; ++i)
      {
         if(i != CPlayers::JOB_RANGER)
            m_pPlayerSet[i]->Set_MainPlayer(m_pPlayer);
      }
   }
}

void CMainPlayer::NaviReset(void)
{
   for (_uint i = 0; i < JOB_END; ++i)
   {
      if(m_pPlayerSet[i])
      {
         Engine::CTransform* pInfo = const_cast<Engine::CTransform*>(m_pPlayerSet[i]->GetInfo());
         m_pPlayerSet[i]->Set_NaviIndex(Engine::CNaviMgr::GetInstance()->GetCellIndex(&pInfo->m_vPos));
      }
   }
}

void CMainPlayer::Set_ContantTable(LPD3DXEFFECT   pEffect)
{
   const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
   const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

   CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

void CMainPlayer::SetMainPlayer(CPlayers* pPlayer)
{
   if(NULL != m_pPlayer)
   {
      if(m_pPlayer->Get_MainPlayer() == true)
      {
         if(m_pPlayer->Get_TrackID() != STAND_NORMAL)
         {
            if(m_pPlayer->Test_ActBitSet(CPlayers::ACT_MOUNT))
            {
               m_pPlayer->Reset_KeyBitset(CPlayers::STAKEY_END,true);
               m_pPlayer->Reset_AtkBitset(CPlayers::ATTACK_END,true);
               m_pPlayer->Reset_SkillBitset(CPlayers::SKILL_END,true);
               m_pPlayer->Set_TagetMonster(NULL);
            }
            else
            {
               m_pPlayer->Set_TrackID(STAND_NORMAL);
               m_pPlayer->Reset_ActBitset(CPlayers::ACT_END,true);
               m_pPlayer->Reset_KeyBitset(CPlayers::STAKEY_END,true);
               m_pPlayer->Reset_AtkBitset(CPlayers::ATTACK_END,true);
               m_pPlayer->Reset_SkillBitset(CPlayers::SKILL_END,true);
               m_pPlayer->Set_TagetMonster(NULL);
            }

            //m_pPlayer->TargetSetting(m_pPlayer);
         }
      }

      m_pPlayer->ResetMainPlayer();
   }
   m_pPlayer = pPlayer;
   Engine::CComponent* pComponent = NULL;

   pComponent = const_cast<Engine::CComponent*>(m_pPlayer->GetComponent(L"Transform"));
   m_pInfo = dynamic_cast<Engine::CTransform*>(pComponent);
   if(m_bStart)
      m_htComponent.Insert_Table(L"Transform", pComponent);
   else
      m_htComponent.Change_TableData(L"Transform", pComponent);

   pComponent = const_cast<Engine::CComponent*>(m_pPlayer->GetComponent(L"Mesh"));
   m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
   if(m_bStart)
      m_htComponent.Insert_Table(L"Mesh", pComponent);
   else
      m_htComponent.Change_TableData(L"Mesh", pComponent);

   pComponent = const_cast<Engine::CComponent*>(m_pPlayer->GetComponent(L"Com_Status"));
   m_pStatus = dynamic_cast<Engine::CStatus*>(pComponent);
   if(m_bStart)
      m_htComponent.Insert_Table(L"Com_Status", pComponent);
   else
      m_htComponent.Change_TableData(L"Com_Status", pComponent);


   if(m_pPlayer->Get_MainPlayer() == false)
   {
      if(m_pPlayer->Get_TrackID() != STAND_NORMAL)
      {
         if(m_pPlayer->Test_ActBitSet(CPlayers::ACT_MOUNT))
         {

            m_pPlayer->Reset_KeyBitset(CPlayers::STAKEY_END,true);
            m_pPlayer->Reset_AtkBitset(CPlayers::ATTACK_END,true);
            m_pPlayer->Reset_SkillBitset(CPlayers::SKILL_END,true);
            m_pPlayer->Set_TagetMonster(NULL);
         }
         else
         {
            m_pPlayer->Set_TrackID(STAND_NORMAL);
            m_pPlayer->Reset_ActBitset(CPlayers::ACT_END,true);
            m_pPlayer->Reset_KeyBitset(CPlayers::STAKEY_END,true);
            m_pPlayer->Reset_AtkBitset(CPlayers::ATTACK_END,true);
            m_pPlayer->Reset_SkillBitset(CPlayers::SKILL_END,true);
            m_pPlayer->Set_TagetMonster(NULL);
         }

      }
   }

   m_pPlayer->SetMainPlayer();
}

void CMainPlayer::StageChange(void)
{
   for (_uint i = 0; i < JOB_END; ++i)
   {
      Engine::CTransform* pInfo = const_cast<Engine::CTransform*>(m_pPlayerSet[i]->GetInfo());
      pInfo->m_vPos = _vec3(86.f, 0.f, 14.f);
      pInfo->Update();
      m_pPlayerSet[i]->Set_NaviIndex(0);
   }
}

void CMainPlayer::ChangePlayer(void)
{
   Engine::CGameObject* pGameObject = Engine::Get_ObjList(LAYER_UI, L"QuestWin1")->front();

   if(CKeyMgr::GetInstance()->GetKeyDown(VK_NUMPAD1))
   {
      if(m_pPlayer != m_pPlayerSet[JOB_GUARDIAN])
         SetMainPlayer(m_pPlayerSet[JOB_GUARDIAN]);
      ((CDynamicCamera*)(*(Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->begin())))->RenewalCamera();
      m_pPlayer->Set_NaviIndex(Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos));

      Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_PLAYTARGET);
      Engine::CTransform* pInfo = const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_GUARDIAN]->GetInfo());
      Engine::Call_EtcFX(Engine::ETC_PLAYTARGET, &pInfo->m_vPos, true);
   }
   else if(CKeyMgr::GetInstance()->GetKeyDown(VK_NUMPAD2))
   {
      if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck(JOB_PALADIN)  || m_eScene == SCENE_STAGE2)
      {
         if(m_pPlayer != m_pPlayerSet[JOB_PALADIN])
            SetMainPlayer(m_pPlayerSet[JOB_PALADIN]);
         ((CDynamicCamera*)(*(Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->begin())))->RenewalCamera();
         m_pPlayer->Set_NaviIndex(Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos));

         Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_PLAYTARGET);
         Engine::CTransform* pInfo = const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_PALADIN]->GetInfo());
         Engine::Call_EtcFX(Engine::ETC_PLAYTARGET, &pInfo->m_vPos, true);
      }
   }
   else if(CKeyMgr::GetInstance()->GetKeyDown(VK_NUMPAD3))
   {
      if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck(JOB_MAGICIAN) || m_eScene == SCENE_STAGE2)
      {
         if(m_pPlayer != m_pPlayerSet[JOB_MAGICIAN])
            SetMainPlayer(m_pPlayerSet[JOB_MAGICIAN]);
         ((CDynamicCamera*)(*(Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->begin())))->RenewalCamera();
         m_pPlayer->Set_NaviIndex(Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos));

         Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_PLAYTARGET);
         Engine::CTransform* pInfo = const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_MAGICIAN]->GetInfo());
         Engine::Call_EtcFX(Engine::ETC_PLAYTARGET, &pInfo->m_vPos, true);
      }
   }
   else if(CKeyMgr::GetInstance()->GetKeyDown(VK_NUMPAD4))
   {
      if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck(JOB_RANGER) || m_eScene == SCENE_STAGE2)
      {
         if(m_pPlayer != m_pPlayerSet[JOB_RANGER])
            SetMainPlayer(m_pPlayerSet[JOB_RANGER]);
         ((CDynamicCamera*)(*(Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->begin())))->RenewalCamera();
         m_pPlayer->Set_NaviIndex(Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos));

         Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_PLAYTARGET);
         Engine::CTransform* pInfo = const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_RANGER]->GetInfo());
         Engine::Call_EtcFX(Engine::ETC_PLAYTARGET, &pInfo->m_vPos, true);
      }
   }

}

void CMainPlayer::RangerAim()
{
   if(NULL == m_pAim)
   {
      m_pAim = ((CAim*)Engine::Get_ObjList(LAYER_UI,L"Aim_UI")->front());
      if(m_pAim == NULL)
         MSG_BOX(L"에임 생성실패");
   }

   if(m_pPlayer == m_pPlayerSet[JOB_RANGER])
      m_pAim->Set_ActRanger(true);
   else
      m_pAim->Set_ActRanger(false);
}

CMainPlayer* CMainPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
   CMainPlayer*      pGameObject = new CMainPlayer(pDevice);
   if(FAILED(pGameObject->Initialize()))
      Engine::Safe_Release(pGameObject);

   return pGameObject;
}

void CMainPlayer::Free(void)
{
   CUnits::Free();
   size_t iTableSize =   m_htComponent.Get_TableSize();
   for(size_t i = 0; i < iTableSize; ++i)
   {
      m_htComponent.Erase_Slot(i);
   }
   m_htComponent.Erase_Table();

   Engine::Safe_Release(m_pDevice);
}

void CMainPlayer::SetPlayerSet(WORD iPlayerIndex, CPlayers* pPlayer)
{
   m_pPlayerSet[iPlayerIndex] = pPlayer;

   if(m_bStart)
   {
      SetMainPlayer(m_pPlayerSet[iPlayerIndex]);
      m_bStart = false;
   }
}

void CMainPlayer::MainQuestCheck(void)
{
   if(m_eScene != SCENE_STAGE1)
      return;

   if(m_eQuestType != QUESTTYPE_MAIN || m_eQuestStep != QUEST_PROGRESS)
      return;

   Engine::CGameObject* pGameObject = Engine::Get_ObjList(LAYER_UI, L"QuestWin1")->front();

   // 몬스터 카운트
   if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck(CPlayers::JOB_END, true) == false)
   {
      int iTotalMonsterCnt = m_pPlayer->Get_vecMonsters()->size();

      int iDeadMonsterCnt = 0;
      for(int i = 0; i < iTotalMonsterCnt; ++i)
      {
         if((*m_pPlayer->Get_vecMonsters())[i]->Get_bDieState() == true)
            ++iDeadMonsterCnt;
      }

      float fHuntMonster = (float)iDeadMonsterCnt / (float)iTotalMonsterCnt;

      // 몬스터 처치 비율
      if(fHuntMonster > 0.4f)
         ((CQuestWin1*)pGameObject)->Set_MainQuestCheck(CPlayers::JOB_END, true);
   }

   // 플레이어 초대
   if(CKeyMgr::GetInstance()->GetKeyDown(VK_SPACE))
   {
      float fDis[JOB_END];

      for(int i = 0; i < JOB_END; ++i)
         fDis[i] = -1.f;

      // MainPlayer와의 거리 구하기
      for(int i = 0; i < JOB_END; ++i)
      {
         if(m_eMainPlayer == i)
            continue;

         CPlayers* pPlayer = Get_PlayerSet(i);
         if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)i))
            continue;

         fDis[i] = D3DXVec3Length(&(pPlayer->GetInfo()->m_vPos - Get_TargetPlayer()->GetInfo()->m_vPos));
      }

      // 최소값 가져오기
      float fMinDis = 100.f;
      for(int i = 0; i < JOB_END; ++i)
      {
         if(m_eMainPlayer == i)
            continue;

         CPlayers* pPlayer = Get_PlayerSet(i);
         if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)i))
            continue;

         fMinDis = min(fDis[i], fMinDis);
      }

      // 최소값을 가진 직업이 뭔지..
      for(int i = 0; i < JOB_END; ++i)
      {
         if(m_eMainPlayer == i)
            continue;

         CPlayers* pPlayer = Get_PlayerSet(i);
         if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)i))
            continue;

         if(fMinDis > 0.5f)
            return;

         if(fMinDis == fDis[i])
         {
            ((CQuestWin1*)pGameObject)->Set_MainQuestCheck((CPlayers::PLAYERJOB)i);
            break;
         }
      }
   }
}

void CMainPlayer::MainQuestAccept(void)
{
   if(m_eScene != SCENE_STAGE1)
      return;

   if(m_eQuestType != QUESTTYPE_MAIN || m_eQuestStep != QUEST_BEFORE)
      return;

   if(g_bMainQuest == true)
      return;

   CBalianBollwerk* pQuestNpc = (CBalianBollwerk*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"CBalianBollwerk")->front());

   if(CKeyMgr::GetInstance()->GetKeyDown(VK_SPACE))
   {
      float fDis = D3DXVec3Length(&(pQuestNpc->GetInfo()->m_vPos - Get_TargetPlayer()->GetInfo()->m_vPos));

      if(fDis <= 0.5f)
      {
         g_bMainQuest = true;
         Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
         ((CDynamicCamera*)pCamera)->Set_MouseFix(false);
      }

   }
}
void CMainPlayer::ControlPlayers()
{
   if(CKeyMgr::GetInstance()->GetKeyDown(VK_F1))
   {
		if(!m_bCtrlbitset.test(CTRL_F1))
			m_bCtrlbitset.set(CTRL_F1);
		else
			m_bCtrlbitset.reset();
   }
}

void CMainPlayer::NaviEffect()
{
   Engine::CGameObject* pGameObject = Engine::Get_ObjList(LAYER_UI, L"QuestWin1")->front();
   if(pGameObject == NULL)
      return;

   if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_MAGICIAN) == false ||
      ((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_PALADIN) == false ||
      ((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_RANGER) == false)
   {
      Engine::Call_EtcFX(Engine::ETC_NAVIBODY,&m_pInfo->m_vPos,true);
   }
   if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_MAGICIAN) == true &&
      ((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_PALADIN) == true &&
      ((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_RANGER) == true)
   {
      Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_NAVIBODY);
   }
   if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_MAGICIAN) == false)
   {
      _vec3 vMyPos , vMagicianPos;
      _myvec vDir;
      vMyPos = _vec3(const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.z);
      vMagicianPos = _vec3(const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_MAGICIAN]->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_MAGICIAN]->GetInfo())->m_vPos.z);

      vDir = vMagicianPos - vMyPos;
      vDir.Normalize();

      _float fRadian = acosf(vDir.Dot(&m_vLook));

      if(vMyPos.z > vMagicianPos.z)
         fRadian = -fRadian;
      else
         fRadian = fRadian;

      m_fSubAngle[0] = -fRadian;

      Engine::Call_EtcFX(Engine::ETC_NAVIARROW_ORANGE,&m_pInfo->m_vPos,true,NULL,&m_fSubAngle[0]);
   }
   else
      Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_NAVIARROW_ORANGE);

   if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_PALADIN) == false)
   {
      _vec3 vMyPos , vMagicianPos;
      _myvec vDir;
      vMyPos = _vec3(const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.z);
      vMagicianPos = _vec3(const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_PALADIN]->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_PALADIN]->GetInfo())->m_vPos.z);

      vDir = vMagicianPos - vMyPos;
      vDir.Normalize();

      _float fRadian = acosf(vDir.Dot(&m_vLook));

      if(vMyPos.z > vMagicianPos.z)
         fRadian = -fRadian;
      else
         fRadian = fRadian;

      m_fSubAngle[1] = -fRadian;

      Engine::Call_EtcFX(Engine::ETC_NAVIARROW_BLUE,&m_pInfo->m_vPos,true,NULL,&m_fSubAngle[1]);
   }
   else
      Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_NAVIARROW_BLUE);

   if(((CQuestWin1*)pGameObject)->Get_MainQuestCheck((CPlayers::PLAYERJOB)JOB_RANGER) == false)
   {
      _vec3 vMyPos , vMagicianPos;
      _myvec vDir;
      vMyPos = _vec3(const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.z);
      vMagicianPos = _vec3(const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_RANGER]->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pPlayerSet[JOB_RANGER]->GetInfo())->m_vPos.z);

      vDir = vMagicianPos - vMyPos;
      vDir.Normalize();

      _float fRadian = acosf(vDir.Dot(&m_vLook));

      if(vMyPos.z > vMagicianPos.z)
         fRadian = -fRadian;
      else
         fRadian = fRadian;

      m_fSubAngle[2] = -fRadian;

      Engine::Call_EtcFX(Engine::ETC_NAVIARROW_RED,&m_pInfo->m_vPos,true,NULL,&m_fSubAngle[2]);
   }
   else
      Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_NAVIARROW_RED);
}

void CMainPlayer::EventFunction()
{
   if(m_bEvent_Start == false)
   {
      _vec3 vPoint,vMainPos;
      _myvec vDir;
      vMainPos =   _vec3(const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos.z);
      vPoint   = m_vEventPos_Start;

      vDir = vPoint - vMainPos;
      _float fDis = vDir.Length();
      vDir.Normalize();

      if(fDis < 1.f)
      {
         Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_QUESTPOINT);
         m_bEvent_Start = true;
      }
      else
         Engine::Call_EtcFX(Engine::ETC_QUESTPOINT,&_vec3(m_vEventPos_Start.x,3.65f, m_vEventPos_Start.z),false);
   }

   if(m_bEvent_Start)
   {
      _vec3 vPoint,vMainPos;
      _myvec vDir;
      vMainPos =   const_cast<Engine::CTransform*>(m_pPlayer->GetInfo())->m_vPos;
      vPoint   = m_vEventPos_End;

      vDir = vPoint - vMainPos;
      _float fDis = vDir.Length();
      vDir.Normalize();

      if(fDis < 0.5f)
      {
         Engine::CEffect_Manager::GetInstance()->Off_EtcFX(Engine::ETC_QUESTPOINT);
         m_bEvent_End = true;
      }
      else
      {
         if(m_bNaviStart == false)
         {
            for(int i = 0; i < 20; ++i)
            {
               
               float S =  (float)i / 20;
               D3DXVec3CatmullRom(&m_vCatMullromPos[i],&m_vNaviPos[0],&m_vNaviPos[0],&m_vNaviPos[1],&m_vNaviPos[2],S);
               D3DXVec3CatmullRom(&m_vCatMullromPos[i + 20],&m_vNaviPos[0],&m_vNaviPos[1],&m_vNaviPos[2],&m_vNaviPos[3],S);
               D3DXVec3CatmullRom(&m_vCatMullromPos[i + 40],&m_vNaviPos[1],&m_vNaviPos[2],&m_vNaviPos[3],&m_vNaviPos[3],S);
               Engine::Call_EtcFX(Engine::ETC_STARPOINT,&m_vCatMullromPos[i],false,NULL,NULL,NULL,_vec3(0.01f,0.01f,0.01f));
               Engine::Call_EtcFX(Engine::ETC_STARPOINT,&m_vCatMullromPos[i + 20],false,NULL,NULL,NULL,_vec3(0.01f,0.01f,0.01f));
               Engine::Call_EtcFX(Engine::ETC_STARPOINT,&m_vCatMullromPos[i + 40],false,NULL,NULL,NULL,_vec3(0.01f,0.01f,0.01f));
               
            }
            m_bNaviStart = true;
         }
         if(m_bEvent_End == false)
            Engine::Call_EtcFX(Engine::ETC_QUESTPOINT,&_vec3(m_vEventPos_End.x,m_vEventPos_End.y + 0.3f,m_vEventPos_End.z),false);
      }
   }
}

//0 -> 1 -> 2 -> 3 =      1 ~ 2 의 사이