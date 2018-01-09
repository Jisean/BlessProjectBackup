#include "stdafx.h"
#include "Stage2.h"

#include "Engine_Include.h"
#include "Include.h"
#include "Export_Function.h"
#include "Layer.h"
#include "Terrain.h"
#include "Loading.h"
#include "DynamicCamera.h"
#include "Bow.h"
#include "SkyBox.h"
#include "UI.h"
#include "Transform.h"
#include "DungeonObject.h"
#include "PlayerBar.h"
#include "SkillSlot.h"
#include "HighIcon.h"
#include "PlayerInfo.h"
#include "MainPlayer.h"
#include "Paladin.h"
#include "Mesh_Effect.h"
#include "Water.h"
#include "SkyDome.h"
#include "KeyMgr.h"
#include "Aim.h"
#include "Guardian.h"
#include "Ranger.h"
#include "Magician.h"
#include "CityGuard.h"
#include "Mace.h"
#include "Sword.h"
#include "Staff.h"
#include "Shield_Paladin.h"
#include "MotionArrow.h"
#include "Shield_Guardian.h"
#include "Sun.h"

#include "Skel_Spear.h"
#include "Mummy_1.h"

#include "AIMgr.h"
#include "Light.h"
#include "Option.h"
#include "Ghost_Knight.h"
#include "Skel_Archer.h"
#include "S_Skel_Archer.h"
#include "S_Skel_Spear.h"
#include "MonsterBar.h"
#include "CastingBar.h"
#include "Party.h"
#include "SkeletonWolf.h"
#include "SoulEater.h"
#include "BulletMgr.h"
#include "QuestWin1.h"
#include "SkeletonWarrior.h"
#include "Slime.h"
#include "Skill_Range.h"
#include "Skill_Range_1.h"
#include "SoundMgr.h"
#include "Trail_Effect.h"
#include "S_Slime.h"
#include "MoveQComplete.h"
#include "QuestWin2.h"
#include "BossQComplete.h"
#include "BossQStart.h"
#include "Soldier_1.h"
#include "NPC_Sword.h"
#include "NPC_Shield.h"
#include "QuestMark.h"
#include "SaveQMark.h"


CStage2::CStage2(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_bCheck(false)
, m_fTimeDelta(0.f)
, m_bBossBgm(false)
, m_bClearBgm(false)
, m_bBossCamReady(true)
{
	m_htAniInfo.Ready_Table(199);
	m_vecMonsters.clear();
	m_vecPlayer_Npc_Info.clear();
	LoadingAni(L"../bin/Resources/Data/Ani/MonsterAni.txt");
	m_pDevice->AddRef();
}

CStage2::~CStage2(void)
{

}

HRESULT CStage2::InitScene(void)
{
	HRESULT		hr = NULL;

	*(Engine::Set_RendererLightPower()) = 0.4f;
	*(Engine::SetRendererBloomPower()) = 1.0f;
	*(Engine::SetRendererBloomRange()) = 2.f;

	FAILED_CHECK_RETURN_MSG(Engine::Ready_Occlusion(m_pDevice), E_FAIL, L"Occlusion Add false");
	//FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Effect_Layer(), E_FAIL, L"UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Light Add false");
	FAILED_CHECK_RETURN_MSG(Init_NaviMesh(), E_FAIL, L"Navi Mesh Add false");

	CSoundMgr::GetInstance()->PlayBgm(L"Dungeon.ogg", 0.8f);


	return S_OK;
}

_int CStage2::Update(const _float& fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;

	KeyCheck(fTimeDelta);
	Check_UI();

	if(false == m_bCheck)
	{
		((CPlayers*)(Engine::Get_ObjList(LAYER_GAMELOGIC,L"Paladin")->front()))->Set_vecMonsters(m_vecMonsters);
		((CPlayers*)(Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front()))->Set_vecMonsters(m_vecMonsters);
		((CPlayers*)(Engine::Get_ObjList(LAYER_GAMELOGIC,L"Guardian")->front()))->Set_vecMonsters(m_vecMonsters);
		((CPlayers*)(Engine::Get_ObjList(LAYER_GAMELOGIC,L"Magician")->front()))->Set_vecMonsters(m_vecMonsters);

		CAIMgr::GetInstance()->Set_MonstersInfo(&m_vecMonsters);
		CAIMgr::GetInstance()->Set_Player_NPC_Info(&m_vecPlayer_Npc_Info);
		CAIMgr::GetInstance()->Set_NPC_Info(&m_vecNpc_Info);
		CAIMgr::GetInstance()->Set_PlayerInfo(&m_vecPlayerInfo);
	}

	_uint iResult = Engine::CScene::Update(fTimeDelta);

	if(m_fTimeDelta > 1.f)
	{
		if(((COption*)(Engine::Get_ObjList(LAYER_UI ,L"Option")->front()))->Get_LightOption())
			Occ_Trigger();
		m_fTimeDelta = 0.f;
	}

	if(!m_bCheck)
	{
		Engine::Set_LightRender(L"Start", false);
		Engine::Set_LightRender(L"StartStreet", false);
		Engine::Set_LightRender(L"NormalField", false);
		Engine::Set_LightRender(L"NormalStreet", false);
		Engine::Set_LightRender(L"MidBoss", false);
		Engine::Set_LightRender(L"BossStreet", false);
		Engine::Set_LightRender(L"Boss", false);

		m_bCheck = true;
	}


	D3DXVECTOR3 vPos = ((CDynamicCamera*)Engine::Get_ObjList(LAYER_UI,L"DynamicCamera")->front())->GetEye();

	return iResult;
}

void CStage2::Render(void)
{
	Render_FPS();

	if(CKeyMgr::GetInstance()->GetToggleKey('0'))
		Print_KeyFont();
	else
		Engine::Render_Font(L"맑은고딕S", _vec3(WINCX - 150.f, 20.f, 0.f), L"0 단축키 도움말", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

#ifdef _DEBUG
	if(g_bBoundingBoxRender)
	{
		Engine::Render_OccBox();
		Engine::Render_Navi();
	}
#endif
}

HRESULT CStage2::Add_Environment_Layer(void)
{
	/*Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIRONMENT, pLayer));*/
	return S_OK;
}

HRESULT CStage2::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	LoadOcclusionBox();

	pGameObject = CTerrain::Create(m_pDevice, ST_STAGE2);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Terrain 생성 실패");
	pLayer->AddObject(L"Terrain" , pGameObject);

	LoadPlayerSet(pLayer);

	LoadObject(pLayer);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CStage2::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CQuestMark::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"QuestMark Create Error");
	pLayer->AddObject(L"QuestMark",pGameObject);

	pGameObject = CSkill_Range::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"SkillRange 생성 실패");
	pLayer->AddObject(L"CSkill_Range" , pGameObject);

	pGameObject = CSkill_Range_1::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"SkillRange_1 생성 실패");
	pLayer->AddObject(L"CSkill_Range_1" , pGameObject);

	pGameObject = CDynamicCamera::Create(m_pDevice, &D3DXVECTOR3(0.f, 5.f, -10.f), &D3DXVECTOR3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"DynamicCamera 생성 실패");
	pLayer->AddObject(L"DynamicCamera" , pGameObject);

	pGameObject = CPlayerBar::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"Player Bar_UI Create Error");
	pLayer->AddObject(L"PlayerBar",pGameObject);

	pGameObject = CSkillSlot::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"Skill Slot_UI Create Error");
	pLayer->AddObject(L"SkillSlot",pGameObject);

	pGameObject = CPlayerInfo::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"Player Info_UI Create Error");
	pLayer->AddObject(L"PlayerInfo",pGameObject);

	pGameObject = CAim::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"Player Info_UI Create Error");
	pLayer->AddObject(L"Aim_UI",pGameObject);

	pGameObject = COption::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"Option Create Error");
	pLayer->AddObject(L"Option",pGameObject);
	((COption*)pGameObject)->Set_Option(COption::GROUP_HDR, true);
	((COption*)pGameObject)->Set_Option(COption::GROUP_LIGHT, true);
	((COption*)pGameObject)->Adjust_Light();

	pGameObject = CMonsterBar::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"MonsterBar Create Error");
	pLayer->AddObject(L"MonsterBar",pGameObject);

	pGameObject = CCastingBar::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"CastingBar Create Error");
	pLayer->AddObject(L"CastingBar",pGameObject);

	pGameObject = CParty::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"Party Create Error");
	pLayer->AddObject(L"Party",pGameObject);
	((CParty*)pGameObject)->Add_PlayerParty(CPlayers::JOB_RANGER, 0);
	((CParty*)pGameObject)->Add_PlayerParty(CPlayers::JOB_PALADIN, 1);
	((CParty*)pGameObject)->Add_PlayerParty(CPlayers::JOB_MAGICIAN, 2);

	pGameObject = CQuestWin1::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"QuestWin1 Create Error");
	pLayer->AddObject(L"QuestWin1",pGameObject);

	pGameObject = CMoveQComplete::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"MoveQComplete Create Error");
	pLayer->AddObject(L"MoveQComplete",pGameObject);

	pGameObject = CQuestWin2::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"QuestWin2 Create Error");
	pLayer->AddObject(L"QuestWin2",pGameObject);

	pGameObject = CBossQComplete::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"BossQComplete Create Error");
	pLayer->AddObject(L"BossQComplete",pGameObject);

	pGameObject = CBossQStart::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"BossQStart Create Error");
	pLayer->AddObject(L"BossQStart",pGameObject);

	pGameObject = CSaveQMark::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject,E_FAIL,L"SaveQMark Create Error");
	pLayer->AddObject(L"SaveQMark",pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

HRESULT CStage2::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.36f, 0.36f, 0.36f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(1.f, -1.f, 1.f);
	LightInfo.Position = _vec3(-76.f, 384.f, 3.f);
	LightAdd(m_pDevice, &LightInfo, L"Directional");

	// 점광원
	StartPoint();
	StartStreet();
	NormalField();
	NormalStreet();
	MidBoss();
	BossStreet();
	Boss();

	// 스타트지점 켜기/끄기
	Engine::Set_LightRender(L"Start", false);
	Engine::Set_LightRender(L"StartStreet", false);
	Engine::Set_LightRender(L"NormalField", false);
	Engine::Set_LightRender(L"NormalStreet", false);
	Engine::Set_LightRender(L"MidBoss", false);
	Engine::Set_LightRender(L"BossStreet", false);
	Engine::Set_LightRender(L"Boss", false);

	return S_OK;
}

CStage2* CStage2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage2* pStage = new CStage2(pDevice);

	if(FAILED(pStage->InitScene()))
	{
		Engine::Safe_Release(pStage);
	}

	return pStage;
}

void CStage2::KeyCheck(const _float& fTimeDelta)
{
	if(TRUE == CKeyMgr::GetInstance()->GetToggleKey(VK_NUMPAD8))
		g_bBoundingBoxRender = true;
	else
		g_bBoundingBoxRender = false;

	Engine::Switch_DebugBuffer(CKeyMgr::GetInstance()->GetToggleKey(VK_NUMPAD7));
}

void	CStage2::LoadingAni(const wstring& wstrPath)
{
	HANDLE	hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	while(true)
	{
		int iObjSize = 0;
		ReadFile(hFile, &iObjSize, sizeof(int), &dwByte, NULL);


		for(int i = 0; i < iObjSize; ++i)
		{
			int iSize = 0;
			ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

			vector<Engine::ANI_INFO*>* vecData = new vector<Engine::ANI_INFO*>; 

			for(int i = 0; i < iSize; ++i)
			{
				Engine::ANI_INFO* pInfo = new Engine::ANI_INFO;
				ZeroMemory(pInfo, sizeof(Engine::ANI_INFO));
				ReadFile(hFile, pInfo, sizeof(Engine::ANI_INFO), &dwByte, NULL);
				vecData->push_back(pInfo);
			}
			vector<Engine::ANI_INFO*>::iterator itervec = vecData->begin();

			m_htAniInfo.Insert_Table(((*itervec)->tName), vecData);
		}

		if(dwByte == 0)

			break;
	}

	CloseHandle(hFile);
}

void	CStage2::LoadingBox(const wstring& wstrPath)
{
	HANDLE	hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	while(true)
	{
		int iObjSize = 0;
		ReadFile(hFile, &iObjSize, sizeof(int), &dwByte, NULL);

		for(int i = 0; i < iObjSize; ++i)
		{
			int iSize = 0;
			ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

			vector<Engine::BOX_INFO*>* vecData = new vector<Engine::BOX_INFO*>; 

			for(int i = 0; i < iSize; ++i)
			{
				Engine::BOX_INFO* pInfo = new Engine::BOX_INFO;
				ZeroMemory(pInfo, sizeof(Engine::BOX_INFO));
				ReadFile(hFile, pInfo, sizeof(Engine::BOX_INFO), &dwByte, NULL);
				vecData->push_back(pInfo);
			}
			vector<Engine::BOX_INFO*>::iterator itervec = vecData->begin();

			m_htBoxInfo.Insert_Table(((*itervec)->tKeyName), vecData);
		}

		if(dwByte == 0)
			break;
	}

	CloseHandle(hFile);
}

void CStage2::SetAni_Box_Info(const TCHAR* pName, Engine::CGameObject* pObj)
{
	vector<Engine::ANI_INFO*>* pData = (vector<Engine::ANI_INFO*>*)m_htAniInfo.Search_TableData(pName);
	((CUnits*)(pObj))->SetAniInfo(*pData);
}



void CStage2::LoadObject(Engine::CLayer* pLayer)
{
	CGhost_Knight*         pMainBoss = NULL;
	CSoulEater*			   pSubBoss = NULL;

	HANDLE	hFile = CreateFile(L"..\\bin\\Resources\\Data\\Dungeon\\SampleDungeonObject.dat", GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	int iObjSize = 0;
	ReadFile(hFile, &iObjSize, sizeof(int), &dwByte, NULL);

	int iIndex = 0;
	for(int i = 0; i < iObjSize; ++i)
	{
		TCHAR pObjectKey[50];
		ZeroMemory(pObjectKey, sizeof(TCHAR) * 50);

		ReadFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);

		int iNum;
		ReadFile(hFile, &iNum, sizeof(int), &dwByte, NULL);
		if(0 == iNum)
			continue;

		Engine::CGameObject*	pGameObject = NULL;

		for(int i = 0; i < iNum; ++i)
		{
			pGameObject = CDungeonObject::Create(m_pDevice, pObjectKey, iIndex, iNum);
			const_cast<Engine::CLayer*>(pLayer)->AddObject(pObjectKey , pGameObject);

			const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_fAngle, sizeof(float) * Engine::ANGLE_END, &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);

			((Engine::CTransform*)pComponent)->Update();

			++iIndex;
		}
	}

	int iDynamicSize = 0;
	ReadFile(hFile, &iDynamicSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i < iDynamicSize; ++i)
	{
		TCHAR pObjectKey[50];
		ZeroMemory(pObjectKey, sizeof(TCHAR) * 50);

		ReadFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);

		int iNum;
		ReadFile(hFile, &iNum, sizeof(int), &dwByte, NULL);
		if(0 == iNum)
			continue;

		Engine::CGameObject*	pGameObject = NULL;

		for(int i = 0; i < iNum; ++i)
		{
			if(!lstrcmp(L"Ghost_Knight", pObjectKey))
			{
				pGameObject = CGhost_Knight::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"Ghost_Knight" , pGameObject);
				m_vecMonsters.push_back((CMonsters*)pGameObject);

				pMainBoss = ((CGhost_Knight*)pGameObject);

				///////
				_vec3  vCenterPos = _vec3(89.f, 1.48f, 154.f);
				vector<int> vecOverlap; 
				int iRand_Z = 0;
				Engine::CGameObject*	pSummonObject = NULL;
				for(int j = 0; j < 6; ++j)
				{
					pSummonObject = CS_Skel_Spear::Create(m_pDevice);
					NULL_CHECK_RETURN_MSG(pSummonObject, , L"S_Skel_Spear 생성 실패");

					const Engine::CComponent* pComponent = pSummonObject->GetComponent(L"Transform");

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

					((Engine::CTransform*)pComponent)->m_vPos.x = vCenterPos.x/*pMainBoss->GetInfo()->m_vPos.x*/ + (iRand_X / 100.f - 1);
					((Engine::CTransform*)pComponent)->m_vPos.z = vCenterPos.z/*pMainBoss->GetInfo()->m_vPos.z*/ + (iRand_Z / 100.f - 1);

					// 소환물 지정
					((CS_Skel_Spear*)pSummonObject)->Set_bSummon_Type(true);

					pLayer->AddObject(L"S_Skel_Spear" , pSummonObject);
					m_vecMonsters.push_back((CMonsters*)pSummonObject);

					((CS_Skel_Spear*)(pSummonObject))->SetBossInfo(pMainBoss);
					pMainBoss->Get_Monster_Call()->push_back((CMonsters*)pSummonObject);
					((CS_Skel_Spear*)pGameObject)->Set_MyIndex(j);

					// 애니 박스 정보 셋팅
					SetAni_Box_Info(L"Skel_Spear", pSummonObject);
				}

				for(int j = 0; j < 6; ++j)
				{
					pSummonObject = CS_Skel_Archer::Create(m_pDevice);
					NULL_CHECK_RETURN_MSG(pSummonObject, , L"S_Skel_Archer 생성 실패");

					const Engine::CComponent* pComponent = pSummonObject->GetComponent(L"Transform");

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

					((Engine::CTransform*)pComponent)->m_vPos.x = vCenterPos.x/*pMainBoss->GetInfo()->m_vPos.x*/ + (iRand_X / 100.f - 1);
					((Engine::CTransform*)pComponent)->m_vPos.z = vCenterPos.z/*pMainBoss->GetInfo()->m_vPos.z*/ + (iRand_Z / 100.f - 1);

					// 소환물 지정
					((CS_Skel_Archer*)pSummonObject)->Set_bSummon_Type(true);

					pLayer->AddObject(L"S_Skel_Archer" , pSummonObject);
					m_vecMonsters.push_back((CMonsters*)pSummonObject);
					((CS_Skel_Archer*)pGameObject)->Set_MyIndex(j);

					((CS_Skel_Archer*)(pSummonObject))->SetBossInfo(pMainBoss);
					pMainBoss->Get_Monster_Call()->push_back((CMonsters*)pSummonObject);

					// 애니 박스 정보 셋팅
					SetAni_Box_Info(L"Skel_Archer", pSummonObject);
				}
			}
			else if(!lstrcmp(L"Mummy_1", pObjectKey))
			{
				pGameObject = CMummy_1::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"Mummy_1" , pGameObject);
				((CMummy_1*)pGameObject)->Set_MyIndex(i);
				m_vecMonsters.push_back((CMonsters*)pGameObject);
			}
			else if(!lstrcmp(L"Skel_Archer", pObjectKey))
			{
				pGameObject = CSkel_Archer::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"Skel_Archer" , pGameObject);
				((CSkel_Archer*)pGameObject)->Set_MyIndex(i);
				m_vecMonsters.push_back((CMonsters*)pGameObject);
			}
			else if(!lstrcmp(L"Skel_Spear", pObjectKey))
			{
				pGameObject = CSkel_Spear::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"Skel_Spear" , pGameObject);
				((CSkel_Spear*)pGameObject)->Set_MyIndex(i);
				m_vecMonsters.push_back((CMonsters*)pGameObject);
			}
			else if(!lstrcmp(L"SoulEater", pObjectKey))
			{
				pGameObject = CSoulEater::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"SoulEater" , pGameObject);
				m_vecMonsters.push_back((CMonsters*)pGameObject);

				pSubBoss = (CSoulEater*)pGameObject;

				// SubBoss Summon
				Engine::CGameObject*	pSummonObject = NULL;
				Engine::CGameObject*	pSkillRange = NULL;
				for(int j = 0; j < 10; ++j)
				{
					pSummonObject = CS_Slime::Create(m_pDevice);
					NULL_CHECK_RETURN_MSG(pSummonObject, , L"S_Slime 생성 실패");
					const Engine::CComponent* pComponent = pSummonObject->GetComponent(L"Transform");
					((Engine::CTransform*)pComponent)->m_vPos = _vec3(113 + j * 1.f, 0.f, 125.f);

					// 소환물 지정
					((CS_Slime*)pSummonObject)->Set_bSummon_Type(true);
					((CS_Slime*)pSummonObject)->Set_bDieState(true);
					((CS_Slime*)pSummonObject)->SetDead(true);
					((CS_Slime*)pSummonObject)->Set_MyIndex(j);

					pLayer->AddObject(L"S_Slime", pSummonObject);
					m_vecMonsters.push_back((CMonsters*)pSummonObject);

					((CS_Slime*)(pSummonObject))->SetBossInfo(pSubBoss);

					pSubBoss->Get_Monster_Call()->push_back((CMonsters*)pSummonObject);

					pSubBoss->Get_AniIdx();   
					// 애니 박스 정보 셋팅
					SetAni_Box_Info(L"Slime", pSummonObject);

					// 범위 스킬 장판
					TCHAR   szObjName[30] = L"";
					wsprintf(szObjName, L"Skill_Range_%d", j);

					TCHAR   szObjName2[30] = L"";
					wsprintf(szObjName2, L"Skill_Range_1_%d", j);

					pSkillRange = CSkill_Range::Create(m_pDevice);
					NULL_CHECK_RETURN_MSG(pSkillRange, , L"SkillRange 생성 실패");
					pLayer->AddObject(szObjName , pSkillRange);

					pSkillRange = CSkill_Range_1::Create(m_pDevice);
					NULL_CHECK_RETURN_MSG(pSkillRange, , L"SkillRange_1 생성 실패");
					pLayer->AddObject(szObjName2 , pSkillRange);
				}
			}
			else if(!lstrcmp(L"SkeletonWarrior", pObjectKey))
			{
				pGameObject = CSkeletonWarrior::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"SkeletonWarrior" , pGameObject);
				((CSkeletonWarrior*)pGameObject)->Set_MyIndex(i);
				m_vecMonsters.push_back((CMonsters*)pGameObject);
			}
			else if(!lstrcmp(L"SkeletonWolf", pObjectKey))
			{
				pGameObject = CSkeletonWolf::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"SkeletonWolf" , pGameObject);
				((CSkeletonWolf*)pGameObject)->Set_MyIndex(i);
				m_vecMonsters.push_back((CMonsters*)pGameObject);
			}
			else if(!lstrcmp(L"Slime", pObjectKey))
			{
				pGameObject = CSlime::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(L"Slime" , pGameObject);
				((CSlime*)pGameObject)->Set_MyIndex(i);
				m_vecMonsters.push_back((CMonsters*)pGameObject);
			}
			
			// Soldier_1  생성
			else if(!lstrcmp(L"NPC_Solder", pObjectKey))
			{
				TCHAR   szObjName[30] = L"";
				wsprintf(szObjName, L"Soldier_1_%d", i);
				pGameObject = CSoldier_1::Create(m_pDevice);
				const_cast<Engine::CLayer*>(pLayer)->AddObject(szObjName , pGameObject);
				m_vecPlayer_Npc_Info.push_back((CUnits*)pGameObject);
				m_vecNpc_Info.push_back((CNPCs*)pGameObject);
			}

			const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_fAngle, sizeof(float) * Engine::ANGLE_END, &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);

			if(!lstrcmp(L"Ghost_Knight", pObjectKey))
			{
				SetAni_Box_Info(L"Ghost_Knight", pGameObject);
			}
			else if(!lstrcmp(L"Mummy_1", pObjectKey))
			{
				SetAni_Box_Info(L"Mummy_1", pGameObject);
			}
			else if(!lstrcmp(L"Skel_Archer", pObjectKey))
			{
				SetAni_Box_Info(L"Skel_Archer", pGameObject);
			}
			else if(!lstrcmp(L"Skel_Spear", pObjectKey))
			{
				SetAni_Box_Info(L"Skel_Spear", pGameObject);
			}
			else if(!lstrcmp(L"SoulEater", pObjectKey))
			{
				SetAni_Box_Info(L"SoulEater", pGameObject);
			}
			else if(!lstrcmp(L"SkeletonWarrior", pObjectKey))
			{
				SetAni_Box_Info(L"SkeletonWarrior", pGameObject);
			}
			else if(!lstrcmp(L"SkeletonWolf", pObjectKey))
			{
				SetAni_Box_Info(L"SkeletonWolf", pGameObject);
			}
			else if(!lstrcmp(L"Slime", pObjectKey))
			{
				SetAni_Box_Info(L"Slime", pGameObject);
			}

			// Soldier_1 무기 방패 생성
			else if(!lstrcmp(L"NPC_Solder", pObjectKey))
			{
				//SetAni_Box_Info(L"Skel_Spear", pGameObject);
				TCHAR   szObjName[30] = L"";
				wsprintf(szObjName, L"Soldier_1_%d", i);

				CSoldier_1*  pNpc = ((CSoldier_1*)pGameObject);

				pGameObject = CNPC_Sword::Create(m_pDevice);
				((CNPC_Sword*)pGameObject)->m_iIndex = i;
				((CNPC_Sword*)pGameObject)->m_pmatWeaponRef = pNpc->m_pHand_R;
				((CNPC_Sword*)pGameObject)->m_pObjName = L"Soldier_1_%d";
				((CNPC_Sword*)pGameObject)->m_pNpc = pNpc;
				NULL_CHECK_MSG(pGameObject,L"CSWord 생성 실패");

				wsprintf(szObjName, L"Sword2_%d", i);
				pLayer->AddObject(szObjName , pGameObject);

				pGameObject = CNPC_Shield::Create(m_pDevice);
				((CNPC_Shield*)pGameObject)->m_iIndex = i;
				((CNPC_Shield*)pGameObject)->m_pmatWeaponRef = pNpc->m_pHand_L;
				((CNPC_Shield*)pGameObject)->m_pObjName = L"Soldier_1_%d";
				((CNPC_Shield*)pGameObject)->m_pNpc = pNpc;
				NULL_CHECK_MSG(pGameObject, L"CShield 생성 실패");

				wsprintf(szObjName, L"Shield2_%d", i);
				pLayer->AddObject(szObjName , pGameObject); 
			}
		}
	}

	Engine::CGameObject*	pGameObject = NULL;

	// 해골 보스 장판용
	pGameObject = CSkill_Range::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, , L"SkillRange 생성 실패");
	pLayer->AddObject(L"CSkill_Range" , pGameObject);

	pGameObject = CSkill_Range_1::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, , L"SkillRange_1 생성 실패");
	pLayer->AddObject(L"CSkill_Range_1" , pGameObject);

	// 거미 보스 장판용
	pGameObject = CSkill_Range::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, , L"Skill_Range_SoulBoss 생성 실패");
	pLayer->AddObject(L"Skill_Range_SoulBoss" , pGameObject);

	pGameObject = CSkill_Range_1::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, , L"Skill_Range_SoulBoss_1 생성 실패");
	pLayer->AddObject(L"Skill_Range_SoulBoss_1" , pGameObject);

	CloseHandle(hFile);
}

HRESULT CStage2::Add_Effect_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject;

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_DASH.dat");
	Engine::Add_GuardianFX(Engine::G_DASH, pGameObject);
	pLayer->AddObject(L"G_DASH", pGameObject);

	for(int i = 0; i < 20; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL1.dat");
		Engine::Add_GuardianFX(Engine::G_SKILL1, pGameObject);
		pLayer->AddObject(L"G_SKILL1", pGameObject);
	}
	for(int i = 0; i < 6; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL1_TARGET.dat");
		Engine::Add_GuardianFX(Engine::G_SKILL1_TARGET, pGameObject);
		pLayer->AddObject(L"G_SKILL1_TARGET", pGameObject);
	}
	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL2_EXPLO.dat");
	Engine::Add_GuardianFX(Engine::G_SKILL2, pGameObject);
	pLayer->AddObject(L"G_SKILL2", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL2_ROOT.dat");
	Engine::Add_GuardianFX(Engine::G_SKILL2_ROOT, pGameObject);
	pLayer->AddObject(L"G_SKILL2_ROOT", pGameObject);	

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL4.dat");
	Engine::Add_GuardianFX(Engine::G_SKILL4, pGameObject);
	pLayer->AddObject(L"G_SKILL4", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_INNATE.dat");
	Engine::Add_GuardianFX(Engine::G_INNATE, pGameObject);
	pLayer->AddObject(L"G_INNATE", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL3.dat");
	Engine::Add_GuardianFX(Engine::G_SKILL3, pGameObject);
	pLayer->AddObject(L"G_SKILL3", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL4_SUB.dat");
	Engine::Add_GuardianFX(Engine::G_SKILL4_SUB, pGameObject);
	pLayer->AddObject(L"G_SKILL4_SUB", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Guardian/G_SKILL5.dat");
	Engine::Add_GuardianFX(Engine::G_SKILL5, pGameObject);
	pLayer->AddObject(L"G_SKILL5", pGameObject);

	//Magician


	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_SKILL1.dat");
	Engine::Add_MagicianFX(Engine::M_SKILL1, pGameObject);
	pLayer->AddObject(L"M_SKILL1", pGameObject);	

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_SKILL2.dat");
	Engine::Add_MagicianFX(Engine::M_SKILL2, pGameObject);
	pLayer->AddObject(L"M_SKILL2", pGameObject);	

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_SKILL3.dat");
	Engine::Add_MagicianFX(Engine::M_SKILL3, pGameObject);
	pLayer->AddObject(L"M_SKILL3", pGameObject);	

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_SKILL4.dat");
	Engine::Add_MagicianFX(Engine::M_SKILL4, pGameObject);
	pLayer->AddObject(L"M_SKILL4", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_SKILL5.dat");
	Engine::Add_MagicianFX(Engine::M_SKILL5, pGameObject);
	pLayer->AddObject(L"M_SKILL5", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_DEFAULT_2_FIRST.dat");
	Engine::Add_MagicianFX(Engine::M_DEFAULT_2_FIRST, pGameObject);
	pLayer->AddObject(L"M_DEFAULT_2_FIRST", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_DEFAULT_2_SECOND.dat");
	Engine::Add_MagicianFX(Engine::M_DEFAULT_2_SECOND, pGameObject);
	pLayer->AddObject(L"M_DEFAULT_2_SECOND", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_DEFAULT1.dat");
	Engine::Add_MagicianFX(Engine::M_DEFAULT1, pGameObject);
	pLayer->AddObject(L"M_DEFAULT1", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_SKILL4_FLOOR.dat");
	Engine::Add_MagicianFX(Engine::M_SKILL4_FLOOR, pGameObject);
	pLayer->AddObject(L"M_SKILL4_FLOOR", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Magician/M_INNATE.dat");
	Engine::Add_MagicianFX(Engine::M_INNATE, pGameObject);
	pLayer->AddObject(L"M_INNATE", pGameObject);

	//Paladin

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_DASH.dat");
	Engine::Add_PaladinFX(Engine::P_DASH, pGameObject);
	pLayer->AddObject(L"P_DASH", pGameObject);	

	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_DASH_TARGET.dat");
		Engine::Add_PaladinFX(Engine::P_DASH_TARGET, pGameObject);
		pLayer->AddObject(L"P_DASH_TARGET", pGameObject);
	}

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_INNATE.dat");
	Engine::Add_PaladinFX(Engine::P_INNATE, pGameObject);
	pLayer->AddObject(L"P_INNATE", pGameObject);	


	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1.dat");
	Engine::Add_PaladinFX(Engine::P_SKILL1, pGameObject);
	pLayer->AddObject(L"P_SKILL1", pGameObject);		


	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1_SUB.dat");
	Engine::Add_PaladinFX(Engine::P_SKILL1_SUB, pGameObject);
	pLayer->AddObject(L"P_SKILL1_SUB", pGameObject);

	for(int i = 0; i < 5; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1_TARGET.dat");
		Engine::Add_PaladinFX(Engine::P_SKILL1_TARGET, pGameObject);
		pLayer->AddObject(L"P_SKILL1_TARGET", pGameObject);	
	}


	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1_WEAPON.dat");
	Engine::Add_PaladinFX(Engine::P_SKILL1_WEAPON, pGameObject);
	pLayer->AddObject(L"P_SKILL1_WEAPON", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL2_BODY.dat");
	Engine::Add_PaladinFX(Engine::P_SKILL2_BODY, pGameObject);
	pLayer->AddObject(L"P_SKILL2_BODY", pGameObject);

	for(int i = 0; i < 5; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL3_TARGET.dat");
		Engine::Add_PaladinFX(Engine::P_SKILL3_TARGET, pGameObject);
		pLayer->AddObject(L"P_SKILL3_TARGET", pGameObject);
	}


	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL4.dat");
	Engine::Add_PaladinFX(Engine::P_SKILL4, pGameObject);
	pLayer->AddObject(L"P_SKILL4", pGameObject);	

	for(int i = 0; i < 5; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Paladin/P_SKILL4_TARGET.dat");
		Engine::Add_PaladinFX(Engine::P_SKILL4_TARGET, pGameObject);
		pLayer->AddObject(L"P_SKILL4_TARGET", pGameObject);	
	}

	//Ranger

	for(int i = 0; i < 10; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL1.dat");
		Engine::Add_RangerFX(Engine::R_SKILL1, pGameObject);
		pLayer->AddObject(L"R_SKILL1", pGameObject);	
	}
	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL2_ARROW.dat");
		Engine::Add_RangerFX(Engine::R_SKILL2_ARROW, pGameObject);
		pLayer->AddObject(L"R_SKILL2_ARROW", pGameObject);
	}
	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL2_BODY.dat");
	Engine::Add_RangerFX(Engine::R_SKILL2_BODY, pGameObject);
	pLayer->AddObject(L"R_SKILL5", pGameObject);

	for(int i = 0; i < 10; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL3.dat");
		Engine::Add_RangerFX(Engine::R_SKILL3, pGameObject);
		pLayer->AddObject(L"R_SKILL3", pGameObject);	
	}

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL3_ARROW.dat");
	Engine::Add_RangerFX(Engine::R_SKILL3_ARROW, pGameObject);
	pLayer->AddObject(L"R_SKILL3_ARROW", pGameObject);
	
	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL3_TARGETHIT.dat");
		Engine::Add_RangerFX(Engine::R_SKILL3_TARGETHIT, pGameObject);
		pLayer->AddObject(L"R_SKILL3_TARGETHIT", pGameObject);
	}
	for(int i = 0; i < 10; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL5.dat");
		Engine::Add_RangerFX(Engine::R_SKILL5, pGameObject);
		pLayer->AddObject(L"R_SKILL5", pGameObject);	
	}
	for(int i = 0; i < 15; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILL5_TARGETHIT.dat");
		Engine::Add_RangerFX(Engine::R_SKILL5_TARGETHIT, pGameObject);
		pLayer->AddObject(L"R_SKILL5_TARGETHIT", pGameObject);
	}
	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Ranger/R_SKILLSHOT.dat");
	Engine::Add_RangerFX(Engine::R_SKILLSHOT, pGameObject);
	pLayer->AddObject(L"R_SKILLSHOT", pGameObject);


	// Monster

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_EXPLO.dat");
	Engine::Add_MonsterFX(Engine::SKULL_EXPLO, pGameObject);
	pLayer->AddObject(L"SKULL_EXPLO", pGameObject);   

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_SLASH.dat");
	Engine::Add_MonsterFX(Engine::SKULL_SLASH, pGameObject);
	pLayer->AddObject(L"SKULL_SLASH", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_SUMMON.dat");
	Engine::Add_MonsterFX(Engine::SKULL_SUMMON, pGameObject);
	pLayer->AddObject(L"SKULL_SUMMON", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_ORA.dat");
	Engine::Add_MonsterFX(Engine::SKULL_ORA, pGameObject);
	pLayer->AddObject(L"SKULL_ORA", pGameObject);

	for(int i = 0 ; i < 15; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_SUMMON_TARGET.dat");
		Engine::Add_MonsterFX(Engine::SKULL_SUMMON_TARGET, pGameObject);
		pLayer->AddObject(L"SKULL_SUMMON_TARGET", pGameObject);
	}

	for(int i = 0; i < 15; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_THORN.dat");
		Engine::Add_MonsterFX(Engine::SKULL_THORN, pGameObject);
		pLayer->AddObject(L"SKULL_THORN", pGameObject);
	}

	for(int i = 0 ; i < 15; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_SUMMON_TARGET.dat");
		Engine::Add_MonsterFX(Engine::SKULL_SUMMON_TARGET, pGameObject);
		pLayer->AddObject(L"SKULL_SUMMON_TARGET", pGameObject);
	}


	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SPIDER_SMOKE.dat");
	Engine::Add_MonsterFX(Engine::SPIDER_SMOKE, pGameObject);
	pLayer->AddObject(L"SPIDER_SMOKE", pGameObject);

	for(int i = 0; i < 10; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SPIDER_EXPLO.dat");
		Engine::Add_MonsterFX(Engine::SPIDER_EXPLO, pGameObject);
		pLayer->AddObject(L"SPIDER_EXPLO", pGameObject);
	}


	for(int i = 0; i < 60; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SPIDER_SMOKE_FLOOR.dat");
		Engine::Add_MonsterFX(Engine::SPIDER_SMOKE_FLOOR, pGameObject);
		pLayer->AddObject(L"SPIDER_SMOKE_FLOOR", pGameObject);
	}

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_FIREEXPLO.dat");
	Engine::Add_MonsterFX(Engine::SKULL_FIREEXPLO, pGameObject);
	pLayer->AddObject(L"SKULL_FIREEXPLO", pGameObject);

	for(int i = 0 ; i < 15; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_FIREEXPLO2.dat");
		Engine::Add_MonsterFX(Engine::SKULL_FIREEXPLO2, pGameObject);
		pLayer->AddObject(L"SKULL_FIREEXPLO2", pGameObject);
	}

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_FIREEXPLOSINGLE.dat");
	Engine::Add_MonsterFX(Engine::SKULL_FIREEXPLOSINGLE, pGameObject);
	pLayer->AddObject(L"SKULL_FIREEXPLOSINGLE", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_THORN_FLOOR.dat");
	Engine::Add_MonsterFX(Engine::SKULL_THORN_FLOOR, pGameObject);
	pLayer->AddObject(L"SKULL_THORN_FLOOR", pGameObject);

	for(int i = 0 ; i < 20; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_METEO.dat");
		Engine::Add_MonsterFX(Engine::SKULL_METEO, pGameObject);
		pLayer->AddObject(L"SKULL_METEO", pGameObject);
	}

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/Monster/SKULL_SHIELD.dat");
	((Engine::CMesh_Effect*)pGameObject)->SetShield();
	((Engine::CMesh_Effect*)pGameObject)->ShieldInit();
	Engine::Add_MonsterFX(Engine::SKULL_SHIELD, pGameObject);	
	pLayer->AddObject(L"SKULL_SHIELD", pGameObject);


	// Etc FX

	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_HIT.dat");
		Engine::Add_EtcFX(Engine::ETC_HIT, pGameObject);
		pLayer->AddObject(L"ETC_HIT", pGameObject);
	}
	
	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_HIT_2.dat");
		Engine::Add_EtcFX(Engine::ETC_HIT2, pGameObject);
		pLayer->AddObject(L"ETC_HIT2", pGameObject);
	}
	
	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_HIT_BLOOD.dat");
		Engine::Add_EtcFX(Engine::ETC_HITBLOOD, pGameObject);
		pLayer->AddObject(L"ETC_HITBLOOD", pGameObject);
	}
	
	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_HIT_CIRCLE.dat");
		Engine::Add_EtcFX(Engine::ETC_HITCIRCLE, pGameObject);
		pLayer->AddObject(L"ETC_HITCIRCLE", pGameObject);
	}
	
	for(int i = 0; i < 30; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_HITSLASH.dat");
		Engine::Add_EtcFX(Engine::ETC_HITSLASH, pGameObject);
		pLayer->AddObject(L"ETC_HITSLASH", pGameObject);
	}
	

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_SMOKE.dat");
	Engine::Add_EtcFX(Engine::ETC_SMOKE, pGameObject);
	pLayer->AddObject(L"ETC_SMOKE", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_TARGET.dat");
	Engine::Add_EtcFX(Engine::ETC_TARGET, pGameObject);
	pLayer->AddObject(L"ETC_TARGET", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_PLAYTARGET.dat");
	Engine::Add_EtcFX(Engine::ETC_PLAYTARGET, pGameObject);
	pLayer->AddObject(L"ETC_PLAYTARGET", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_TARGETSELECT.dat");
	Engine::Add_EtcFX(Engine::ETC_TARGETSELECT, pGameObject);
	pLayer->AddObject(L"ETC_TARGETSELECT", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_Quest.dat");
	Engine::Add_EtcFX(Engine::ETC_QUEST, pGameObject);
	pLayer->AddObject(L"ETC_QUEST", pGameObject);

	for(_uint i = 0; i < 48; ++i)
	{
		pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_TORCH.dat");
		Engine::Add_EtcFX(Engine::ETC_TORCH, pGameObject);
		pLayer->AddObject(L"ETC_TORCH", pGameObject);
	}

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_PORTAL.dat");
	Engine::Add_EtcFX(Engine::ETC_PORTAL, pGameObject);
	pLayer->AddObject(L"ETC_PORTAL", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_QUESTPOINT.dat");
	Engine::Add_EtcFX(Engine::ETC_QUESTPOINT, pGameObject);
	pLayer->AddObject(L"E_QUESTPOINT", pGameObject);

	pGameObject =  LoadEffectCreate(L"../bin/Resources/Data/Effect/EX/E_STARPOINT.dat");
	Engine::Add_EtcFX(Engine::ETC_STARPOINT, pGameObject);
	pLayer->AddObject(L"ETC_STARPOINT", pGameObject);


	//트레일 

	CEquipment* pTarget = (CEquipment*)(m_mapLayer[LAYER_GAMELOGIC]->GetObjList(L"Sword")->front());

	pGameObject = CTrail_Effect::Create(m_pDevice, CTrail_Effect::TRAIL_SWORD);
	((CTrail_Effect*)pGameObject)->SetTarget(pTarget);	

	Engine::Add_TrailFX(Engine::TRAIL_SWORD, pGameObject);

	pLayer->AddObject(L"Trail_Sword", pGameObject);

	pTarget = (CEquipment*)(m_mapLayer[LAYER_GAMELOGIC]->GetObjList(L"Mace")->front());

	pGameObject = CTrail_Effect::Create(m_pDevice, CTrail_Effect::TRAIL_MACE);
	((CTrail_Effect*)pGameObject)->SetTarget(pTarget);	

	Engine::Add_TrailFX(Engine::TRAIL_MACE, pGameObject);

	pLayer->AddObject(L"Trail_Mace", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_EFFECT, pLayer));

	return S_OK;
}

Engine::CGameObject* CStage2::LoadEffectCreate(const wstring& wstrPath)
{
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = Engine::CreateMeshEffect(m_pDevice);

	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte = 0;

	while(true)
	{		
		Engine::SAVEDATA tSaveData;	

		ReadFile(hFile, &tSaveData.szMeshKey							,sizeof(tSaveData.szMeshKey),		&dwByte, NULL);
		ReadFile(hFile, &tSaveData.szMeshPath							,sizeof(tSaveData.szMeshPath),		&dwByte, NULL);
		ReadFile(hFile, &tSaveData.szTextureKey							,sizeof(tSaveData.szTextureKey),	&dwByte, NULL);
		ReadFile(hFile, &tSaveData.szTexturePath						,sizeof(tSaveData.szTexturePath),	&dwByte, NULL);
		ReadFile(hFile, &tSaveData.szMaskKey							,sizeof(tSaveData.szMaskKey),		&dwByte, NULL);
		ReadFile(hFile, &tSaveData.szMaskPath							,sizeof(tSaveData.szMaskPath),		&dwByte, NULL);	
		ReadFile(hFile, &tSaveData.szNormalKey							,sizeof(tSaveData.szNormalKey),		&dwByte, NULL);
		ReadFile(hFile, &tSaveData.szNormalPath							,sizeof(tSaveData.szNormalPath),	&dwByte, NULL);		

		for(int iIndexCount = 0; iIndexCount < Engine::ACT_END; ++iIndexCount)
		{
			ReadFile(hFile, &tSaveData.iActionMaxIndex[iIndexCount]			,sizeof(int),					&dwByte, NULL);
			ReadFile(hFile, &tSaveData.bUnLimited[iIndexCount]				,sizeof(bool),					&dwByte, NULL);
		}

		for(int iActCount = 0; iActCount < tSaveData.iActionMaxIndex[Engine::ACT_POS]; ++iActCount)
		{
			Engine::POSACTION	tPosAction;

			ReadFile(hFile, &tPosAction.vPosValue					,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tPosAction.fStart_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tPosAction.fEnd_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tPosAction.fAcc						,sizeof(float),					&dwByte, NULL);
			tSaveData.vecPosAction.push_back(tPosAction);
		}

		for(int iActCount = 0; iActCount < tSaveData.iActionMaxIndex[Engine::ACT_ROT]; ++iActCount)
		{		
			Engine::ROTACTION	tRotAction;		

			ReadFile(hFile, &tRotAction.vRotValue					,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tRotAction.fStart_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tRotAction.fEnd_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tRotAction.fAcc						,sizeof(float),					&dwByte, NULL);			
			tSaveData.vecRotAction.push_back(tRotAction);			
		}

		for(int iActCount = 0; iActCount < tSaveData.iActionMaxIndex[Engine::ACT_SCALE]; ++iActCount)
		{
			Engine::SCALEACTION	tScaleAction;			

			ReadFile(hFile, &tScaleAction.vScaleValue					,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tScaleAction.fStart_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tScaleAction.fEnd_Time						,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tScaleAction.fAcc							,sizeof(float),					&dwByte, NULL);
			tSaveData.vecScaleAction.push_back(tScaleAction);
		}

		for(int iActCount = 0; iActCount < tSaveData.iActionMaxIndex[Engine::ACT_FADE]; ++iActCount)
		{
			Engine::FADEACTION	tFadeAction;			

			ReadFile(hFile, &tFadeAction.fFadeValue						,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tFadeAction.fStart_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tFadeAction.fEnd_Time						,sizeof(float),					&dwByte, NULL);
			tSaveData.vecFadeAction.push_back(tFadeAction);
		}

		for(int iActCount = 0; iActCount < tSaveData.iActionMaxIndex[Engine::ACT_UV]; ++iActCount)
		{
			Engine::UVACTION	tUVAction;	

			ReadFile(hFile, &tUVAction.fU								,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fV								,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fStart_Time						,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fEnd_Time						,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fAccU							,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fAccV							,sizeof(float),					&dwByte, NULL);
			tSaveData.vecUVAction.push_back(tUVAction);
		}

		for(int iActCount = 0; iActCount < tSaveData.iActionMaxIndex[Engine::ACT_UVSIZE]; ++iActCount)
		{
			Engine::UVSIZEACTION	tUVSizeAction;			

			ReadFile(hFile, &tUVSizeAction.fUSize								,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVSizeAction.fVSize								,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVSizeAction.fStart_Time							,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVSizeAction.fEnd_Time							,sizeof(float),					&dwByte, NULL);		
			tSaveData.vecUVSizeAction.push_back(tUVSizeAction);
		}

		for(int iActCount = 0; iActCount < tSaveData.iActionMaxIndex[Engine::ACT_COLOR]; ++iActCount)
		{
			Engine::COLORACTION	tColorAction;			

			ReadFile(hFile, &tColorAction.vRGB							,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tColorAction.fNext_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tColorAction.fChange_Time					,sizeof(float),					&dwByte, NULL);
			tSaveData.vecColorAction.push_back(tColorAction);
		}


		ReadFile(hFile, &tSaveData.vOriPos								,sizeof(D3DXVECTOR3),			&dwByte, NULL);
		ReadFile(hFile, &tSaveData.vOriRot								,sizeof(D3DXVECTOR3),			&dwByte, NULL);
		ReadFile(hFile, &tSaveData.vOriScale							,sizeof(D3DXVECTOR3),			&dwByte, NULL);

		ReadFile(hFile, &tSaveData.iOriCycle							,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.iPass								,sizeof(int),					&dwByte, NULL);

		ReadFile(hFile, &tSaveData.iSpriteMaxCount						,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.iSpriteX								,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.iSpriteY								,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.fSpriteTotalTime						,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &tSaveData.fFade								,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.fOriFade								,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &tSaveData.fAliveStart_Time						,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.fAliveEnd_Time						,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &tSaveData.fOriUPos								,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.fOriVPos								,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &tSaveData.fOriUSize							,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.fOriVSize							,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &tSaveData.bColorOn								,sizeof(bool),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.bBillOn								,sizeof(bool),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.bHoldOn								,sizeof(bool),					&dwByte, NULL);
		ReadFile(hFile, &tSaveData.bSingleBillOn						,sizeof(bool),					&dwByte, NULL);
		//============================	

		Engine::MESHEFFECTINFO* pEffectInfo = new Engine::MESHEFFECTINFO;

		_mymat matScale, matRotX, matRotY, matRotZ,  matTrans;
		matTrans.Trans(tSaveData.vOriPos);
		matRotX.RotX(D3DXToRadian(tSaveData.vOriRot.x));
		matRotY.RotY(D3DXToRadian(tSaveData.vOriRot.y));
		matRotZ.RotZ(D3DXToRadian(tSaveData.vOriRot.z));
		matScale.Scale(tSaveData.vOriScale);

		pEffectInfo->matWorld						= matScale * matRotX * matRotY * matRotZ * matTrans;
		pEffectInfo->vPos							= tSaveData.vOriPos;
		pEffectInfo->vOriPos						= tSaveData.vOriPos;
		pEffectInfo->vRot							= tSaveData.vOriRot;
		pEffectInfo->vOriRot						= tSaveData.vOriRot;
		pEffectInfo->vScale							= tSaveData.vOriScale;
		pEffectInfo->vOriScale						= tSaveData.vOriScale;

		if(tSaveData.bColorOn == true)
			pEffectInfo->vRGB						= tSaveData.vecColorAction.front().vRGB;
		else
			pEffectInfo->vRGB						= D3DXVECTOR3(0.f, 0.f, 0.f);

		pEffectInfo->vRGBDifference					= D3DXVECTOR3(0.f, 0.f, 0.f);

		pEffectInfo->iPass							= tSaveData.iPass;

		for(int iIndexCount = 0; iIndexCount < Engine::ACT_END; ++iIndexCount)
		{
			pEffectInfo->iActionMaxIndex[iIndexCount]	= tSaveData.iActionMaxIndex[iIndexCount];
			pEffectInfo->iActionIndex[iIndexCount]		= 0;
			pEffectInfo->bUnLimited[iIndexCount]		= tSaveData.bUnLimited[iIndexCount];
			pEffectInfo->fUnLimitedTime[iIndexCount]	= 0.f;
		}	

		pEffectInfo->iSpriteCount					= 0;
		pEffectInfo->iSpriteMaxCount				= tSaveData.iSpriteMaxCount;
		pEffectInfo->iSpriteX						= tSaveData.iSpriteX;
		pEffectInfo->iSpriteY						= tSaveData.iSpriteY;

		pEffectInfo->fAliveStart_Time				= tSaveData.fAliveStart_Time;
		pEffectInfo->fAliveEnd_Time					= tSaveData.fAliveEnd_Time;

		pEffectInfo->fSpriteTotalTime				= tSaveData.fSpriteTotalTime;
		pEffectInfo->fSpriteTime					= 0.f;

		pEffectInfo->fColorTime						= 0.f;
		pEffectInfo->fColorChangeTime				= 0.f;

		pEffectInfo->fFade							= tSaveData.fFade;
		pEffectInfo->fOriFade						= tSaveData.fOriFade;
		pEffectInfo->bAliveOn						= true;
		pEffectInfo->bColorOn						= tSaveData.bColorOn;
		pEffectInfo->bBillOn						= tSaveData.bBillOn;
		pEffectInfo->bHoldOn						= tSaveData.bHoldOn;
		pEffectInfo->fUSize							= tSaveData.fOriUSize;
		pEffectInfo->fVSize							= tSaveData.fOriVSize;
		pEffectInfo->fOriUSize						= tSaveData.fOriUSize;
		pEffectInfo->fOriVSize						= tSaveData.fOriVSize;
		pEffectInfo->fUPos							= tSaveData.fOriUPos;
		pEffectInfo->fVPos							= tSaveData.fOriVPos;
		pEffectInfo->fOriUPos						= tSaveData.fOriUPos;
		pEffectInfo->fOriVPos						= tSaveData.fOriVPos;
		pEffectInfo->iOriCycle						= tSaveData.iOriCycle;
		pEffectInfo->iCycle							= tSaveData.iOriCycle;
		pEffectInfo->bSingleBillOn					= tSaveData.bSingleBillOn;
		pEffectInfo->fGetTime						= 0.f;		
		pEffectInfo->bEnd							= false;


		for(unsigned int iActCount = 0; iActCount < tSaveData.vecPosAction.size(); ++iActCount)
			pEffectInfo->vecPosAction.push_back( tSaveData.vecPosAction[iActCount]);	

		for(unsigned int iActCount = 0; iActCount < tSaveData.vecRotAction.size(); ++iActCount)
			pEffectInfo->vecRotAction.push_back(  tSaveData.vecRotAction[iActCount]);

		for(unsigned int iActCount = 0; iActCount < tSaveData.vecScaleAction.size(); ++iActCount)
			pEffectInfo->vecScaleAction.push_back( tSaveData.vecScaleAction[iActCount]);

		for(unsigned int iActCount = 0; iActCount < tSaveData.vecFadeAction.size(); ++iActCount)
			pEffectInfo->vecFadeAction.push_back( tSaveData.vecFadeAction[iActCount]);

		for(unsigned int iActCount = 0; iActCount < tSaveData.vecUVAction.size(); ++iActCount)
			pEffectInfo->vecUVAction.push_back( tSaveData.vecUVAction[iActCount]);

		for(unsigned int iActCount = 0; iActCount < tSaveData.vecUVSizeAction.size(); ++iActCount)
			pEffectInfo->vecUVSizeAction.push_back( tSaveData.vecUVSizeAction[iActCount]);

		for(unsigned int iActCount = 0; iActCount < tSaveData.vecColorAction.size(); ++iActCount)
			pEffectInfo->vecColorAction.push_back( tSaveData.vecColorAction[iActCount]);

		if(dwByte == 0)
		{		
			Engine::Safe_Delete(pEffectInfo);
			break;
		}

		((Engine::CMesh_Effect*)pGameObject)->Add_Data(tSaveData.szMeshKey, tSaveData.szTextureKey, tSaveData.szMaskKey, tSaveData.szNormalKey, pEffectInfo);
	}	

	CloseHandle(hFile);

	((Engine::CMesh_Effect*)pGameObject)->Off_FX();

	return pGameObject;		
}

void CStage2::Print_KeyFont(void)
{
#ifdef _DEBUG
	Engine::Render_Font(L"맑은고딕S", _vec3(WINCX - 150.f, 110.f, 0.f), L"NUMPAD7 렌더타겟On/Off", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Engine::Render_Font(L"맑은고딕S", _vec3(WINCX - 150.f, 125.f, 0.f), L"NUMPAD8 바운딩박스 On/Off", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif

}

HRESULT CStage2::LoadPlayerSet(Engine::CLayer* pLayer)
{
	Engine::CGameObject*	pGameObject = NULL;

	CMainPlayer* pMainPlayer = NULL;
	pGameObject = pMainPlayer = CMainPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Player 생성 실패");	
	pLayer->AddObject(L"Player" , pGameObject);
	pMainPlayer->SetScene(CMainPlayer::SCENE_STAGE2);

	pGameObject = CGuardian::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Guardian 생성 실패");
	pLayer->AddObject(L"Guardian" , pGameObject);
	pMainPlayer->SetPlayerSet(0, (CPlayers*)pGameObject);
	m_vecPlayer_Npc_Info.push_back((CUnits*)pGameObject);
	m_vecPlayerInfo.push_back((CPlayers*)pGameObject);

	pGameObject = CPaladin::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Paladin 생성 실패");
	pLayer->AddObject(L"Paladin" , pGameObject);
	pMainPlayer->SetPlayerSet(1, (CPlayers*)pGameObject);
	m_vecPlayer_Npc_Info.push_back((CUnits*)pGameObject);
	m_vecPlayerInfo.push_back((CPlayers*)pGameObject);

	pGameObject = CMagician::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Magician 생성 실패");	
	pLayer->AddObject(L"Magician" , pGameObject);
	pMainPlayer->SetPlayerSet(2, (CPlayers*)pGameObject);
	m_vecPlayer_Npc_Info.push_back((CUnits*)pGameObject);
	m_vecPlayerInfo.push_back((CPlayers*)pGameObject);

	pGameObject = CRanger::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Ranger 생성 실패");	
	pLayer->AddObject(L"Ranger" , pGameObject);
	pMainPlayer->SetPlayerSet(3, (CPlayers*)pGameObject);
	m_vecPlayer_Npc_Info.push_back((CUnits*)pGameObject);
	m_vecPlayerInfo.push_back((CPlayers*)pGameObject);

	pMainPlayer->StageChange();
	////장비//////////////////////////////////////////////////////////////////////////

	pGameObject = CMace::Create(m_pDevice);		// 조정필요
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Mace 생성 실패");	
	pLayer->AddObject(L"Mace" , pGameObject);

	pGameObject = CSword::Create(m_pDevice);		
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Sword 생성 실패");	
	pLayer->AddObject(L"Sword" , pGameObject);

	pGameObject = CStaff::Create(m_pDevice);		
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Staff 생성 실패");	
	pLayer->AddObject(L"Staff" , pGameObject);

	pGameObject = CBow::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Bow 생성 실패");	
	pLayer->AddObject(L"Bow" , pGameObject);

	pGameObject = CShield_Paladin::Create(m_pDevice);		
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Shield_Paladin 생성 실패");	
	pLayer->AddObject(L"Shield_Paladin" , pGameObject);

	pGameObject = CMotionArrow::Create(m_pDevice);		
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"MotionArrow 생성 실패");	
	pLayer->AddObject(L"MotionArrow" , pGameObject);

	pGameObject = CShield_Guardian::Create(m_pDevice);		
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Shield_Guardian 생성 실패");	
	pLayer->AddObject(L"Shield_Guardian" , pGameObject);



	return S_OK;
}

HRESULT CStage2::LoadMonsterSet(Engine::CLayer* pLayer)
{
	Engine::CGameObject*	pGameObject = NULL;

	for(int i = 0; i < 1; ++i)
	{
		pGameObject = CSkel_Spear::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Skel_Spear 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_vPos = _vec3(112.f + i * 1.f, 0.f, 123.f);

		//((Engine::CTransform*)pComponent)->m_vPos = _vec3(112 + i * 1.f, 0.f, 123.f);
		pLayer->AddObject(L"Skel_Spear" , pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		// 애니 박스 정보 셋팅
		SetAni_Box_Info(L"Skel_Spear", pGameObject);
	}

	for(int i = 0; i < 0; ++i)
	{
		pGameObject = CMummy_1::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Mummy_1 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_vPos = _vec3(116 + i * 1.f, 0.f, 122.f);
		pLayer->AddObject(L"Mummy_1" , pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		// 애니 박스 정보 셋팅
		SetAni_Box_Info(L"Mummy_1", pGameObject);
	}


	for(int i = 0; i < 0; ++i)
	{
		pGameObject = CSkeletonWolf::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"SkeletonWolf 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_vPos = _vec3(117 + i * 1.f, 0.f, 125.f);
		pLayer->AddObject(L"SkeletonWolf" , pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		// 애니 박스 정보 셋팅
		//SetAni_Box_Info(L"SkeletonWolf", pGameObject);
	}

	for(int i = 0; i < 0; ++i)
	{
		pGameObject = CSkel_Archer::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Skel_Archer 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_vPos = _vec3(112 + i * 1.f, 0.f, 124.f);
		pLayer->AddObject(L"Skel_Archer" , pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		// 애니 박스 정보 셋팅
		SetAni_Box_Info(L"Skel_Archer", pGameObject);
	}

	return S_OK;
}

HRESULT CStage2::Init_NaviMesh(void)
{
	HANDLE	hFile = CreateFile(L"..\\bin\\Resources\\Data\\Dungeon\\SampleDungeonNavi.dat", GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	int iNaviNum;
	ReadFile(hFile, &iNaviNum, sizeof(int), &dwByte, NULL);
	Engine::Reserve_CellContainerSize(m_pDevice, iNaviNum);

	for(int i = 0; i < iNaviNum; ++i)
	{
		D3DXVECTOR3 vPoint[3];
		ReadFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);
		DWORD dwType;
		ReadFile(hFile, &dwType, sizeof(DWORD), &dwByte, NULL);

		D3DXVECTOR3 vComparePoint[3];

		vComparePoint[0] = ((vPoint[0].x < vPoint[1].x) ? vPoint[0] : vPoint[1]);
		vPoint[0] = ((vComparePoint[0].x < vPoint[2].x) ? vComparePoint[0] : vPoint[2]);

		if(vComparePoint[0] == vPoint[0])
		{
			vComparePoint[1] = ((vPoint[1].x < vPoint[2].x) ? vPoint[1] : vPoint[2]);
			vComparePoint[2] = ((vPoint[1].x > vPoint[2].x) ? vPoint[1] : vPoint[2]);
		}
		else if(vComparePoint[0] == vPoint[1])
		{
			vComparePoint[0] = ((vPoint[0].x < vPoint[2].x) ? vPoint[0] : vPoint[2]);
			vComparePoint[2] = ((vPoint[0].x > vPoint[2].x) ? vPoint[0] : vPoint[2]);
		}
		else
		{
			vComparePoint[0] = ((vPoint[0].x < vPoint[1].x) ? vPoint[0] : vPoint[1]);
			vComparePoint[1] = ((vPoint[0].x > vPoint[1].x) ? vPoint[0] : vPoint[1]);
		}

		D3DXVECTOR3 vTemp[3];

		vTemp[0] = vComparePoint[0];
		vTemp[1] = vComparePoint[1];
		vTemp[2] = vComparePoint[2];

		if(vTemp[1].z < vTemp[2].z)
		{	
			_vec3 vTemp2;

			vTemp2 = vTemp[1];
			vTemp[1] = vTemp[2];
			vTemp[2] = vTemp2;

			if(vTemp[0].z < vTemp[2].z)
			{
				D3DXVECTOR3 vTempDir[3];

				vTempDir[0] = D3DXVECTOR3(0, 0, 0);
				vTempDir[1] = D3DXVECTOR3(vTemp[2] - vTemp[0]);
				vTempDir[2] = D3DXVECTOR3(vTemp[1] - vTemp[0]);

				float fGradient = vTempDir[1].z / vTempDir[1].x;

				float fZ = fGradient * (vTempDir[2].x);

				if(fZ > vTempDir[2].z)
				{
					if(vTemp[0].z < vTemp[2].z)
					{
						_vec3 vTemp2;

						vTemp2 = vTemp[1];
						vTemp[1] = vTemp[2];
						vTemp[2] = vTemp2;
					}				
				}	
			}
		}
		else
		{
			if(vTemp[0].z > vTemp[2].z)
			{
				D3DXVECTOR3 vTempDir[3];

				vTempDir[0] = D3DXVECTOR3(0, 0, 0);
				vTempDir[1] = D3DXVECTOR3(vTemp[1] - vTemp[0]);
				vTempDir[2] = D3DXVECTOR3(vTemp[2] - vTemp[0]);

				float fGradient = vTempDir[1].z / vTempDir[1].x;

				float fZ = fGradient * (vTempDir[2].x);

				if(fZ < vTempDir[2].z)
				{
					_vec3 vTemp2;

					vTemp2 = vTemp[1];
					vTemp[1] = vTemp[2];
					vTemp[2] = vTemp2;			
				}	
			}
		}

		vPoint[0] = vTemp[0];
		vPoint[1] = vTemp[1];
		vPoint[2] = vTemp[2];


		Engine::Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2], dwType);
	}
	Engine::Link_Cell();

	CloseHandle(hFile);

	return S_OK;
}

void CStage2::LoadOcclusionBox(void)
{
	HANDLE	hFile = CreateFile(L"..\\bin\\Resources\\Data\\Dungeon\\OccBoxDungeon.dat", GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;
	int iCount = 0;
	ReadFile(hFile, &iCount, sizeof(int), &dwByte, NULL);

	for(int i = 0; i < iCount; ++i)
	{
		D3DXMATRIX matWorld;
		ReadFile(hFile, &matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);
		Engine::Add_OccBox(&matWorld);
		Engine::CTransform* pOccBox = Engine::Get_OccBox(i);

		ReadFile(hFile, &pOccBox->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, &pOccBox->m_fAngle, sizeof(float) * 3, &dwByte, NULL);
		ReadFile(hFile, &pOccBox->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, &pOccBox->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
		pOccBox->Update();
		Engine::Set_OccVtxPos(i);
	}

	CloseHandle(hFile);
}

HRESULT CStage2::LightAdd(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, TCHAR* pKeyName, int iRepeat /*= 1*/)
{
	HRESULT		hr = NULL;
	for(int i = 0; i < iRepeat; ++i)
	{
		hr = Engine::Add_Light(m_pDevice, pLightInfo, pKeyName);
		FAILED_CHECK(hr);
	}

	return S_OK;
}


void CStage2::Check_UI(void)
{
	if(g_bOption)
		g_bPause = true;
	else
		g_bPause = false;

	if(CKeyMgr::GetInstance()->GetKeyDown('O'))
	{
		g_bOption = !g_bOption;

		Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();

		if(g_bOption == true)
		{
			ShowCursor(TRUE);
			((CDynamicCamera*)pCamera)->Set_MouseFix(false);
		}
		else
		{
			ShowCursor(FALSE);
			((CDynamicCamera*)pCamera)->Set_MouseFix(true);
		}
	}
}


HRESULT CStage2::LoadBossSet(Engine::CLayer* pLayer)
{
	Engine::CGameObject*   pGameObject = NULL;
	CGhost_Knight*         pMainBoss = NULL;
	CSoulEater*			   pSubBoss = NULL;

	// Main Boss
	for(int i = 0; i < 1; ++i)
	{
		pGameObject = CGhost_Knight::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Ghost_Knight 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_vPos = _vec3(112 + i * 1.f, 0.f, 120.f);
		pLayer->AddObject(L"Ghost_Knight", pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		pMainBoss = ((CGhost_Knight*)pGameObject);

		// 애니 박스 정보 셋팅
		SetAni_Box_Info(L"Ghost_Knight", pGameObject);
	}

	// SubBoss
	for(int i = 0; i < 0; ++i)
	{
		pGameObject = CSoulEater::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"SoulEater 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_vPos = _vec3(113 + i * 1.f, 0.f, 120.f);
		pLayer->AddObject(L"SoulEater", pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		pSubBoss = ((CSoulEater*)pGameObject);

		// 애니 박스 정보 셋팅
		//SetAni_Box_Info(L"Ghost_Knight", pGameObject);
	}

	vector<int> vecOverlap; 
	int iRand_Z = 0;
	for(int i = 0; i < 0; ++i)
	{
		pGameObject = CS_Skel_Spear::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"S_Skel_Spear 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

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

		((Engine::CTransform*)pComponent)->m_vPos.x = pMainBoss->GetInfo()->m_vPos.x + (iRand_X / 100.f - 1);
		((Engine::CTransform*)pComponent)->m_vPos.z = pMainBoss->GetInfo()->m_vPos.z + (iRand_Z / 100.f - 1);

		// 소환물 지정
		((CS_Skel_Spear*)pGameObject)->Set_bSummon_Type(true);

		pLayer->AddObject(L"S_Skel_Spear" , pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		((CS_Skel_Spear*)(pGameObject))->SetBossInfo(pMainBoss);
		pMainBoss->Get_Monster_Call()->push_back((CMonsters*)pGameObject);

		// 애니 박스 정보 셋팅
		SetAni_Box_Info(L"Skel_Spear", pGameObject);
	}

	for(int i = 0; i < 1; ++i)
	{
		pGameObject = CS_Skel_Archer::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"S_Skel_Archer 생성 실패");

		const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");

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

		((Engine::CTransform*)pComponent)->m_vPos.x = pMainBoss->GetInfo()->m_vPos.x + (iRand_X / 100.f - 1);
		((Engine::CTransform*)pComponent)->m_vPos.z = pMainBoss->GetInfo()->m_vPos.z + (iRand_Z / 100.f - 1);

		// 소환물 지정
		((CS_Skel_Archer*)pGameObject)->Set_bSummon_Type(true);

		pLayer->AddObject(L"S_Skel_Archer" , pGameObject);
		m_vecMonsters.push_back((CMonsters*)pGameObject);

		((CS_Skel_Archer*)(pGameObject))->SetBossInfo(pMainBoss);
		pMainBoss->Get_Monster_Call()->push_back((CMonsters*)pGameObject);

		// 애니 박스 정보 셋팅
		SetAni_Box_Info(L"Skel_Archer", pGameObject);
	}
	return S_OK;
}

HRESULT CStage2::StartPoint(void)
{
	D3DLIGHT9      LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	HRESULT      hr = NULL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Range = 1.6f;   

	int iRepeat = 1;

	LightInfo.Position = _vec3(88.0585f, 3.70585f, 8.65026f);
	LightAdd(m_pDevice, &LightInfo, L"Start", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(88.0585f, 3.70585f, 8.65026f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(84.1586f, 3.70585f, 8.65026f);
	LightAdd(m_pDevice, &LightInfo, L"Start", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(84.1586f, 3.70585f, 8.65026f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(83.6586f, 3.60585f, 29.0503f);
	LightAdd(m_pDevice, &LightInfo, L"Start", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(83.3586f, 3.60585f, 28.0503f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(88.0585f, 3.60585f, 29.0503f);
	LightAdd(m_pDevice, &LightInfo, L"Start", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(87.8585f, 3.60585f, 28.0503f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(105.858f, 3.60585f, 17.9503f);
	LightAdd(m_pDevice, &LightInfo, L"Start", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(104.358f, 3.60585f, 17.6503f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(67.1589f, 3.60585f, 17.9503f);
	LightAdd(m_pDevice, &LightInfo, L"Start", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(67.1589f, 3.60585f, 17.9503f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	return S_OK;
}

HRESULT CStage2::StartStreet(void)
{
	D3DLIGHT9      LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	HRESULT      hr = NULL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Range = 1.6f;   

	int iRepeat = 1;

	LightInfo.Position = _vec3(83.5586f, 5.00585f, 47.0501f);
	LightAdd(m_pDevice, &LightInfo, L"StartStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(83.5586f, 5.00585f, 47.0501f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(87.2586f, 5.00585f, 47.0501f);
	LightAdd(m_pDevice, &LightInfo, L"StartStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(87.4586f, 5.00585f, 47.0501f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	return S_OK;
}

HRESULT CStage2::NormalField(void)
{
	D3DLIGHT9      LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	HRESULT      hr = NULL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Range = 1.6f;   

	int iRepeat = 1;

	LightInfo.Position = _vec3(87.9586f, 3.00585f, 66.5498f);
	LightAdd(m_pDevice, &LightInfo, L"NormalField", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(87.9586f, 3.00585f, 66.5498f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(83.5587f, 3.00585f, 66.5498f);
	LightAdd(m_pDevice, &LightInfo, L"NormalField", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(83.5587f, 3.00585f, 66.5498f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(88.0586f, 3.00585f, 82.2496f);
	LightAdd(m_pDevice, &LightInfo, L"NormalField", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(88.0586f, 3.00585f, 81.2496f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(83.7587f, 3.00585f, 82.2496f);
	LightAdd(m_pDevice, &LightInfo, L"NormalField", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(83.7587f, 3.00585f, 81.2496f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	return S_OK;
}

HRESULT CStage2::NormalStreet(void)
{
	D3DLIGHT9      LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	HRESULT      hr = NULL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Range = 1.6f;   

	int iRepeat = 1;

	LightInfo.Position = _vec3(84.5587f, 2.40585f, 98.2494f);
	LightAdd(m_pDevice, &LightInfo, L"NormalStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(84.2587f, 2.30585f, 98.2494f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(74.5588f, 3.90585f, 100.049f);
	LightAdd(m_pDevice, &LightInfo, L"NormalStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(74.5588f, 3.90585f, 100.049f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(84.5587f, 2.40585f, 103.549f);
	LightAdd(m_pDevice, &LightInfo, L"NormalStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(84.2587f, 2.30585f, 103.549f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(90.8586f, 3.00585f, 118.549f);
	LightAdd(m_pDevice, &LightInfo, L"NormalStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(90.8586f, 3.00585f, 119.049f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(90.7586f, 3.10585f, 115.749f);
	LightAdd(m_pDevice, &LightInfo, L"NormalStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(90.7586f, 3.10585f, 114.749f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	return S_OK;
}

HRESULT CStage2::MidBoss(void)
{
	D3DLIGHT9      LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	HRESULT      hr = NULL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Range = 1.6f;   

	int iRepeat = 1;

	LightInfo.Position = _vec3(121.758f, 3.60585f, 114.349f);
	LightAdd(m_pDevice, &LightInfo, L"MidBoss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(121.758f, 3.60585f, 114.349f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(121.758f, 3.60585f, 119.549f);
	LightAdd(m_pDevice, &LightInfo, L"MidBoss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(121.758f, 3.60585f, 119.549f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));


	LightInfo.Position = _vec3(120.758f, 1.90585f, 127.349f);
	LightAdd(m_pDevice, &LightInfo, L"MidBoss", iRepeat + 1);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(120.258f, 1.90585f, 127.349f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(120.758f, 1.90585f, 106.549f);
	LightAdd(m_pDevice, &LightInfo, L"MidBoss", iRepeat + 1);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(120.258f, 1.90585f, 106.549f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));


	LightInfo.Position =  _vec3(137.859f, 3.50585f, 119.349f);
	LightAdd(m_pDevice, &LightInfo, L"MidBoss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(138.859f, 3.50585f, 119.349f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position =  _vec3(137.859f, 3.50585f, 114.849f);
	LightAdd(m_pDevice, &LightInfo, L"MidBoss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(138.859f, 3.50585f, 114.849f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	return S_OK;
}

HRESULT CStage2::BossStreet(void)
{
	D3DLIGHT9      LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	HRESULT      hr = NULL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Range = 1.6f;   

	int iRepeat = 1;

	LightInfo.Position = _vec3(152.46f, 2.50585f, 130.249f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(151.76f, 2.50585f, 130.249f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(152.46f, 2.50585f, 135.649f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(151.76f, 2.50585f, 135.649f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(162.06f, 4.00585f, 133.049f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(161.36f, 4.00585f, 133.049f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));



	LightInfo.Position = _vec3(141.659f, 3.20585f, 157.75f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(141.659f, 3.20585f, 156.85f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(141.659f, 3.20585f, 153.15f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(141.659f, 3.20585f, 152.95f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(122.658f, 3.00585f, 157.85f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(122.658f, 3.00585f, 156.85f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(122.558f, 2.90585f, 153.25f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(122.558f, 3.20585f, 152.95f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	
	LightInfo.Position = _vec3(110.858f, 4.10585f, 156.25f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(110.858f, 4.05585f, 156.15f), false, NULL, NULL, NULL, D3DXVECTOR3(0.03f, 0.03f, 0.03f));

	LightInfo.Position = _vec3(110.958f, 4.10585f, 154.15f);
	LightAdd(m_pDevice, &LightInfo, L"BossStreet", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(110.858f, 4.05585f, 154.05f), false, NULL, NULL, NULL, D3DXVECTOR3(0.03f, 0.03f, 0.03f));

	return S_OK;
}

HRESULT CStage2::Boss(void)
{
	D3DLIGHT9      LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	HRESULT      hr = NULL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.2f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Range = 2.8f;   

	int iRepeat = 1;

	/*LightInfo.Position = _vec3(106.858f, 4.00585f, 149.95f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(106.858f, 4.00585f, 149.95f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	/*LightInfo.Position = _vec3(102.658f, 4.00585f, 149.95f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(102.658f, 4.00585f, 149.95f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));


	/*LightInfo.Position = _vec3(96.3581f, 4.00585f, 146.75f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(96.3581f, 4.00585f, 146.75f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	/*LightInfo.Position = _vec3(93.2581f, 4.00585f, 146.75f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(93.2581f, 4.00585f, 146.75f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));


	/*LightInfo.Position = _vec3(92.2582f, 4.00585f, 162.351f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(92.2582f, 4.00585f, 161.751f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	/*LightInfo.Position = _vec3(95.7581f, 4.00585f, 162.351f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(95.7581f, 4.00585f, 161.751f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));


	/*LightInfo.Position = _vec3(102.658f, 4.00585f, 160.751f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(102.658f, 4.00585f, 160.151f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	/*LightInfo.Position = _vec3(106.758f, 4.00585f, 160.751f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);*/
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(106.758f, 4.00585f, 160.151f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Range = 3.6f;   
	LightInfo.Position = _vec3(87.8583f, 3.70585f, 162.551f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(87.8583f, 3.70585f, 161.951f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(84.1583f, 3.70585f, 162.551f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(84.1583f, 3.70585f, 161.951f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(84.7583f, 3.80585f, 147.05f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(84.7583f, 3.80585f, 147.05f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	LightInfo.Position = _vec3(88.9583f, 3.80585f, 147.05f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(88.9583f, 3.80585f, 147.05f), false, NULL, NULL, NULL, D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(81.7583f, 3.40585f, 160.65f), false, NULL, NULL, NULL, D3DXVECTOR3(0.06f, 0.07f, 0.07f));
	Engine::Call_EtcFX(Engine::ETC_TORCH, &D3DXVECTOR3(81.7583f, 3.40585f, 147.65f), false, NULL, NULL, NULL, D3DXVECTOR3(0.07f, 0.07f, 0.07f));

	LightInfo.Range = 5.5f; 
	LightInfo.Position = _vec3(81.7583f, 3.40585f, 156.15f);
	LightAdd(m_pDevice, &LightInfo, L"Boss", iRepeat);

	return S_OK;
}

void CStage2::Free(void)
{
	Engine::CScene::Free();

	size_t iTableSize =	m_htAniInfo.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		vector<Engine::ANI_INFO*>* pvecAniInfo = (vector<Engine::ANI_INFO*>*)m_htAniInfo.Erase_Slot(i);

		if(NULL != pvecAniInfo)
		{
			_uint iVecSize = pvecAniInfo->size();
			vector<Engine::ANI_INFO*>::iterator iter = pvecAniInfo->begin();
			vector<Engine::ANI_INFO*>::iterator iter_end = pvecAniInfo->end();
			for(iter; iter != iter_end; ++iter)
			{
				Safe_Delete(*iter);
			}
			Safe_Delete(pvecAniInfo);
		}
	}
	m_htAniInfo.Erase_Table();

	CBulletMgr::GetInstance()->DestroyInstance();
	CAIMgr::GetInstance()->DestroyInstance();
	Engine::Safe_Release(m_pDevice);
}

void CStage2::Occ_Trigger(void)
{
	Engine::CTransform* pPlayerInfo = (Engine::CTransform*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");

	CGhost_Knight* pBoss = ((CGhost_Knight*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"Ghost_Knight")->front()));

	bool bBossDead = pBoss->Get_bDieState();

	int iIndex = Engine::Test_InOccBox(pPlayerInfo->m_vPos);

	Engine::Set_LightRender(L"Start", false);
	Engine::Set_LightRender(L"StartStreet", false);
	Engine::Set_LightRender(L"NormalField", false);
	Engine::Set_LightRender(L"NormalStreet", false);
	Engine::Set_LightRender(L"MidBoss", false);
	Engine::Set_LightRender(L"BossStreet", false);
	Engine::Set_LightRender(L"Boss", false);

	switch(iIndex)
	{
	case 0:
		Engine::Set_LightRender(L"Start", true);
		Engine::Set_LightRender(L"StartStreet", false);
		break;
	case 1:
		Engine::Set_LightRender(L"Start", false);
		Engine::Set_LightRender(L"StartStreet", true);
		Engine::Set_LightRender(L"NormalField", false);
		break;
	case 2:
		Engine::Set_LightRender(L"StartStreet", false);
		Engine::Set_LightRender(L"NormalField", true);
		Engine::Set_LightRender(L"NormalStreet", false);
		break;
	case 3:
	case 4:
		Engine::Set_LightRender(L"NormalField", false);
		Engine::Set_LightRender(L"NormalStreet", true);
		Engine::Set_LightRender(L"MidBoss", false);
		break;
	case 5:
		Engine::Set_LightRender(L"NormalStreet", false);
		Engine::Set_LightRender(L"MidBoss", true);
		Engine::Set_LightRender(L"BossStreet", false);
		break;
	case 6:
	case 7:
		Engine::Set_LightRender(L"MidBoss", false);
		Engine::Set_LightRender(L"BossStreet", true);
		Engine::Set_LightRender(L"Boss", false);
		if(bBossDead == false && m_bBossBgm == true)
		{
			CSoundMgr::GetInstance()->StopBgm();
			CSoundMgr::GetInstance()->PlayBgm(L"Dungeon.ogg");
			m_bBossBgm = false;
		}
		else if(bBossDead == true && m_bClearBgm == true)
		{
			CSoundMgr::GetInstance()->StopBgm();
			CSoundMgr::GetInstance()->PlayBgm(L"Dungeon.ogg");
			m_bClearBgm = false;
		}
		break;
	case 8:
		Engine::Set_LightRender(L"BossStreet", false);
		Engine::Set_LightRender(L"Boss", true);
		if(bBossDead == false && m_bBossBgm == false)
		{
			CSoundMgr::GetInstance()->StopBgm();
			CSoundMgr::GetInstance()->PlayBgm(L"Boss.mp3");
			m_bBossBgm = true;
		}
		else if(bBossDead == true && m_bClearBgm == false)
		{
			CSoundMgr::GetInstance()->StopBgm();
			CSoundMgr::GetInstance()->PlayBgm(L"BossClear.mp3");
			m_bClearBgm = true;
		}
		if(m_bBossCamReady)
		{
			CDynamicCamera* pCam = (CDynamicCamera*)Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
			pCam->Set_ActionCam(CDynamicCamera::ACT_BOSSROOM);
			m_bBossCamReady = false;
		}
		break;
	default:
		Engine::Set_LightRender(L"Start", false);
		Engine::Set_LightRender(L"StartStreet", false);
		Engine::Set_LightRender(L"NormalField", false);
		Engine::Set_LightRender(L"NormalStreet", false);
		Engine::Set_LightRender(L"MidBoss", false);
		Engine::Set_LightRender(L"BossStreet", false);
		Engine::Set_LightRender(L"Boss", false);
		break;
	}

}
