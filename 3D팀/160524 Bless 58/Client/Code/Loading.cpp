#include "stdafx.h"
#include "Loading.h"

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include <process.h>
#include <fstream>

CLoading::CLoading(LOADINGID eLoadID)
: m_eLoadID(eLoadID)
, m_pDevice(NULL)
, m_hThread(NULL)
, m_bComplete(false)
{
	ZeroMemory(m_szLoadMessage, sizeof(TCHAR) * MAX_PATH);
	ZeroMemory(m_szLoadPath, sizeof(TCHAR) * MAX_PATH);
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
}

CLoading::~CLoading(void)
{

}

bool CLoading::GetComplete(void)
{
	return m_bComplete;
}

const TCHAR* CLoading::GetLoadMessage(void)
{
	return m_szLoadMessage;
}

const TCHAR* CLoading::GetLoadPath(void)
{
	return m_szLoadPath;
}

HRESULT CLoading::InitLoading(void)
{
	m_pDevice = Engine::Get_GraphicDev();

	InitializeCriticalSection(&m_CSKey);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);

	if(m_hThread == NULL)
		return E_FAIL;

	return S_OK;
}

void CLoading::StageLoading(LOADINGID eLoadID)
{
	HRESULT hr = NULL;

	switch(eLoadID)
	{
	case LOADING_STAGE1:
		// UI 로드
		lstrcpy(m_szLoadMessage, L"UI 로딩중...");
		UI_Setting();
		lstrcpy(m_szLoadMessage, L"이펙트 로딩중...");
		hr = EffectResourceLoad();

		// 터레인 타일 로드
		lstrcpy(m_szLoadMessage, L"텍스쳐 로딩중...");
		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/CityTile/Tile_%d.tga");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_Terrain"
			, L"../bin/Resources/Texture/CityTile/Tile_%d.tga", 5);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/CityTile/TileN_%d.tga");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_TerrainNormal"
			, L"../bin/Resources/Texture/CityTile/TileN_%d.tga", 5);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/CityTile/CityFilter%d.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_Filter"
			, L"../bin/Resources/Texture/CityTile/CityFilter%d.png", 4);
		FAILED_CHECK_RETURN(hr, );


		//물 텍스쳐
		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/Water.tga");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_Water"
			, L"../bin/Resources/Texture/Water.tga", 1);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/Water_NRM.tga");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_Water_NRM"
			, L"../bin/Resources/Texture/Water_NRM.tga", 1);
		FAILED_CHECK_RETURN(hr, );

		// 물 버퍼
		hr = Engine::Add_Buffer(m_pDevice
			, RESOURCE_STAGE
			, Engine::CVIBuffer::BUFFER_TERRAIN
			, L"Buffer_Water"
			, 129, 129, 1);
		FAILED_CHECK_RETURN_MSG(hr, , L"Buffer_Water 생성 실패");

		lstrcpy(m_szLoadMessage, L"스카이돔 메쉬 로딩중...");
		lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Sky/SkyDome.X");
		hr = Engine::Add_Mesh(m_pDevice
			, RESOURCE_STAGE
			, Engine::MESH_STATIC
			, L"Mesh_SkyDome"
			, L"../bin/Resources/Mesh/StaticMesh/Sky/"
			, L"SkyDome.X");
		FAILED_CHECK_RETURN(hr, );

		// 해
		lstrcpy(m_szLoadMessage, L"햇님 메쉬 로딩중...");
		lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Sun/Sun.X");
		hr = Engine::Add_Mesh(m_pDevice
			, RESOURCE_STAGE
			, Engine::MESH_STATIC
			, L"Mesh_Sun"
			, L"../bin/Resources/Mesh/StaticMesh/Sun/"
			, L"Sun.X");
		FAILED_CHECK_RETURN(hr, );

		//////////////////////////////////////////////////////////////////////////// 스피어
		lstrcpy(m_szLoadMessage, L"공 메쉬 로딩중...");
		lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Sphere/Sphere.X");
		hr = Engine::Add_Mesh(m_pDevice
			, RESOURCE_REMAIN
			, Engine::MESH_STATIC
			, L"Mesh_Sphere"
			, L"../bin/Resources/Mesh/StaticMesh/Sphere/"
			, L"Sphere.X");
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadMessage, L"플레이어 로딩중...");
		hr = PlayerLoad();

		break;

	case LOADING_STAGE2:
		// 터레인 타일 로드
		lstrcpy(m_szLoadMessage, L"텍스쳐 로딩중...");
		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/DungeonTile/Tile_%d.tga");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_Terrain"
			, L"../bin/Resources/Texture/DungeonTile/Tile_%d.tga", 5);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/DungeonTile/TileN_%d.tga");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_TerrainNormal"
			, L"../bin/Resources/Texture/DungeonTile/TileN_%d.tga", 5);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/DungeonTile/ColorHeight%d.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_Filter"
			, L"../bin/Resources/Texture/DungeonTile/ColorHeight%d.png", 4);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/SkillRange/Skill_Rande_%d.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STAGE
			, Engine::TEXTURE_NORMAL
			, L"Texture_Skill_Range"
			, L"../bin/Resources/Texture/SkillRange/Skill_Rande_%d.png", 2);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadMessage, L"UI 로딩중...");
		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/BossQuest/BossQuest.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, L"Texture_Quest_BossWidow_MainTxt"
			, L"../bin/Resources/Texture/NewUI/Set/Quest/BossQuest/BossQuest.png", 1);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/BossQuest/BossQuestTxt.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, L"Texture_Quest_BossWidow_Quest1"
			, L"../bin/Resources/Texture/NewUI/Set/Quest/BossQuest/BossQuestTxt1.png", 1);
		FAILED_CHECK_RETURN(hr, );

		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, L"Texture_Quest_BossWidow_Quest2"
			, L"../bin/Resources/Texture/NewUI/Set/Quest/BossQuest/BossQuestTxt2.png", 1);
		FAILED_CHECK_RETURN(hr, );

		// 세이브 퀘스트
		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/Num%d.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, L"Texture_SaveQuest_Num"
			, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/Num%d.png", 6);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/SaveQuest0.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, L"Texture_SaveQuest_OutLine"
			, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/SaveQuest0.png", 1);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/SaveQuestComplete.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, L"Texture_SaveQuest_Complete"
			, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/SaveQuestComplete.png", 1);
		FAILED_CHECK_RETURN(hr, );

		lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/SaveQT.png");
		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, L"Texture_SaveQuest_CompleteTxt"
			, L"../bin/Resources/Texture/NewUI/Set/Quest/SaveQuest/SaveQT.png", 1);
		FAILED_CHECK_RETURN(hr, );

		break;
	}
	


	lstrcpy(m_szLoadMessage, L"버퍼 로딩중...");
	lstrcpy(m_szLoadPath, L"Making Buffers.....");
	//Buffer
	hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_TERRAIN
		, L"Buffer_TerrainTex"
		, VTXCNTX, VTXCNTZ, VTXITV);
	FAILED_CHECK_RETURN_MSG(hr, , L"Buffer_TerrainTex 생성 실패");

	//hr = Engine::Add_Buffer(m_pDevice
	//	, RESOURCE_STAGE
	//	, Engine::CVIBuffer::BUFFER_TERRAIN
	//	, L"Buffer_BaseTerrainTex"
	//	, 2, 2, 512);
	//FAILED_CHECK_RETURN_MSG(hr, , L"Buffer_BaseTerrainTex 생성 실패");

	/*hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_CUBETEX
		, L"Buffer_CubeTex");
	FAILED_CHECK_RETURN_MSG(hr, , L"Buffer_CubeTex 생성 실패");*/

	

	//////////////////////////////////////////////////////////////////////////
	lstrcpy(m_szLoadMessage, L"맵 오브젝트 텍스쳐 로딩중...");
	hr = StageMeshTexLoad(eLoadID);
	lstrcpy(m_szLoadMessage, L"맵 오브젝트 메쉬 로딩중...");
	hr = StageMeshLoad(eLoadID);
	lstrcpy(m_szLoadMessage, L"몬스터 로딩중...");
	hr = MonsterLoad(eLoadID);
	lstrcpy(m_szLoadMessage, L"NPC 로딩중...");
	hr = NPCLoad();

	lstrcpy(m_szLoadMessage, L"로딩 완료...");
	lstrcpy(m_szLoadPath, L"Loading Complete...");

	m_bComplete = true;
}

CLoading* CLoading::Create(LOADINGID eLoadID)
{
	CLoading*	pLoading = new CLoading(eLoadID);
	if(FAILED(pLoading->InitLoading()))
		Engine::Safe_Release(pLoading);

	return pLoading;
}

void CLoading::Free(void)
{
	DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	Engine::Safe_Release(m_pDevice);
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	switch(pLoading->m_eLoadID)
	{
	case CLoading::LOADING_STAGE1:
		pLoading->StageLoading(LOADING_STAGE1);
		break;

	case CLoading::LOADING_STAGE2:
		pLoading->StageLoading(LOADING_STAGE2);
		break;
	}
	return 0;
}

HRESULT CLoading::PlayerLoad(void)
{
	HRESULT hr = NULL;
	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Paladin/PaladinNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_Paladin"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Paladin/"
		, L"PaladinNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	
	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Guardian/GuardianNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_Guardian"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Guardian/"
		, L"GuardianNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Ranger/RangerNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_Ranger"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Ranger/"
		, L"RangerNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Magician/MagicianNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_Magician"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Magician/"
		, L"MagicianNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	//////////////////////////////////////////////////////////////////////////무기
	lstrcpy(m_szLoadMessage, L"아이템 로딩중...");
	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Mace/Mace.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Mace"
		, L"../bin/Resources/Mesh/StaticMesh/Mace/"
		, L"Mace2.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Shield/Shield.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Shield_Paladin"
		, L"../bin/Resources/Mesh/StaticMesh/Shield/"
		, L"Shield.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Shield/CircleShield.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Shield_Guardian"
		, L"../bin/Resources/Mesh/StaticMesh/Shield/"
		, L"CircleShield.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/STAFF/STAFF.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Staff"
		, L"../bin/Resources/Mesh/StaticMesh/STAFF/"
		, L"STAFF.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Sword/Sword3.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Sword"
		, L"../bin/Resources/Mesh/StaticMesh/Sword/"
		, L"Sword3.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Bow/Bow.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Bow"
		, L"../bin/Resources/Mesh/StaticMesh/Bow/"
		, L"Bow.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Arrow/Arrow.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Arrow"
		, L"../bin/Resources/Mesh/StaticMesh/Arrow/"
		, L"Arrow.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/FireArrow/FireArrow.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_FireArrow"
		, L"../bin/Resources/Mesh/StaticMesh/FireArrow/"
		, L"FireArrow.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//================= NPC 전용 장비 생성 =================//
	lstrcpy(m_szLoadMessage, L"NPC장비 로딩중...");
	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Sword/Sword4.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Sword4"
		, L"../bin/Resources/Mesh/StaticMesh/Sword/"
		, L"Sword4.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Shield/Shield5.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_STATIC
		, L"Mesh_Shield5"
		, L"../bin/Resources/Mesh/StaticMesh/Shield/"
		, L"Shield5.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//////////////////////////////////////////////////////////////////////////// 마운트

	lstrcpy(m_szLoadMessage, L"와이번 로딩중...");
	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/BrownWyvern/BrownWyvernNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_WyvernBrown"
		, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/BrownWyvern/"
		, L"BrownWyvernNew.X");
	FAILED_CHECK_RETURN(hr,E_FAIL );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/LandWyvern/LandWyvernNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_WyvernLand"
		, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/LandWyvern/"
		, L"LandWyvernNew.X");
	FAILED_CHECK_RETURN(hr,E_FAIL );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/LesserWyvern/LesserWyvernNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_WyvernLesser"
		, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/LesserWyvern/"
		, L"LesserWyvernNew.X");
	FAILED_CHECK_RETURN(hr,E_FAIL );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/TwinsWyvern/TwinsWyvernNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_REMAIN
		, Engine::MESH_DYNAMIC
		, L"Mesh_WyvernTwins"
		, L"../bin/Resources/Mesh/DynamicMesh/Wyvern/TwinsWyvern/"
		, L"TwinsWyvernNew.X");
	FAILED_CHECK_RETURN(hr,E_FAIL );


	return S_OK;
}

HRESULT CLoading::MonsterLoad(LOADINGID eLoadID)
{
	HRESULT hr = NULL;

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Skel_Spear/Skel_Spear.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Skel_Spear"
		, L"../bin/Resources/Mesh/DynamicMesh/Skel_Spear/"
		, L"Skel_Spear.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Mummy_1/Mummy_1.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Mummy_1"
		, L"../bin/Resources/Mesh/DynamicMesh/Mummy_1/"
		, L"Mummy_1.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Mummy_2/Mummy_2.X");
	//hr = Engine::Add_Mesh(m_pDevice
	//	, RESOURCE_STAGE
	//	, Engine::MESH_DYNAMIC
	//	, L"Mesh_Mummy_2"
	//	, L"../bin/Resources/Mesh/DynamicMesh/Mummy_2/"
	//	, L"Mummy_2.X");
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Skel_Archer/Skel_Archer.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Skel_Archer"
		, L"../bin/Resources/Mesh/DynamicMesh/Skel_Archer/"
		, L"Skel_Archer.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Ghost_Knight/Ghost_Knight.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Ghost_Knight"
		, L"../bin/Resources/Mesh/DynamicMesh/Ghost_Knight/"
		, L"Ghost_Knight.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/SoulEater/SoulEater.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_SoulEater"
		, L"../bin/Resources/Mesh/DynamicMesh/SoulEater/"
		, L"SoulEater.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Slime/Slime.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Slime"
		, L"../bin/Resources/Mesh/DynamicMesh/Slime/"
		, L"Slime.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/SkeletonWarrior/SkeletonWarrior.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_SkeletonWarrior"
		, L"../bin/Resources/Mesh/DynamicMesh/SkeletonWarrior/"
		, L"SkeletonWarrior.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/SkeletonWolf/SkeletonWolf.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_SkeletonWolf"
		, L"../bin/Resources/Mesh/DynamicMesh/SkeletonWolf/"
		, L"SkeletonWolf.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}

HRESULT CLoading::NPCLoad(void)
{
	HRESULT hr = NULL;

	//lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/NPC_Gurad/NPC_Guard.X");
	//hr = Engine::Add_Mesh(m_pDevice
	//	, RESOURCE_STAGE
	//	, Engine::MESH_DYNAMIC
	//	, L"CityGuard"
	//	, L"../bin/Resources/Mesh/DynamicMesh/NPC_Gurad/"
	//	, L"NPC_Guard.X");
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/NPC_Gurad2/NPC_Guard2.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"CityGuard2"
		, L"../bin/Resources/Mesh/DynamicMesh/NPC_Gurad2/"
		, L"NPC_Guard2.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/BalianBollwerk/BalianBollwerk.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_BalianBollwerk"
		, L"../bin/Resources/Mesh/DynamicMesh/BalianBollwerk/"
		, L"BalianBollwerk.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	

	return S_OK;
}

HRESULT CLoading::StageMeshTexLoad(LOADINGID eLoadID)
{
	wifstream LoadFile;
	HRESULT hr = NULL;

	switch(eLoadID)
	{
	case LOADING_STAGE1:
		LoadFile.open(L"..//bin/Resources/Data/City/CityMeshTexPath.txt",ios::in);
		break;

	case LOADING_STAGE2:
		LoadFile.open(L"..//bin/Resources/Data/Dungeon/DungeonMeshTexPath.txt",ios::in);
		break;
	}

	while(!LoadFile.eof())
	{
		TCHAR* szTexName = new TCHAR[MAX_PATH];
		TCHAR* szTexPath = new TCHAR[MAX_PATH];

		LoadFile.getline(szTexName,MAX_PATH,'|');
		LoadFile.getline(szTexPath,MAX_PATH);

		lstrcpy(m_szLoadPath, szTexPath);

		if(szTexName == L"\n")
			break;

		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_STATIC
			, Engine::TEXTURE_NORMAL
			, szTexName
			, szTexPath, 1);
		FAILED_CHECK_RETURN(hr,E_FAIL);

		Engine::Safe_Delete_Array(szTexName);
		Engine::Safe_Delete_Array(szTexPath);
	}

	LoadFile.close();

	return S_OK;
}

HRESULT CLoading::StageMeshLoad(LOADINGID eLoadID)
{
	wifstream LoadFile;
	HRESULT hr = NULL;

	switch(eLoadID)
	{
	case LOADING_STAGE1:
		LoadFile.open(L"..//bin/Resources/Data/City/CityMeshPath.txt",ios::in);
		break;

	case LOADING_STAGE2:
		LoadFile.open(L"..//bin/Resources/Data/Dungeon/DungeonMeshPath.txt",ios::in);
		break;
	}

	while(!LoadFile.eof())
	{
		TCHAR* szMeshKey = new TCHAR[MAX_PATH];
		TCHAR* szMeshPath = new TCHAR[MAX_PATH];
		TCHAR* szMeshName = new TCHAR[MAX_PATH];

		LoadFile.getline(szMeshKey,MAX_PATH,'|');
		LoadFile.getline(szMeshPath,MAX_PATH,'|');
		LoadFile.getline(szMeshName,MAX_PATH);

		lstrcpy(m_szLoadPath, szMeshPath);
		lstrcat(m_szLoadPath, szMeshName);

		if(szMeshKey == L"\n")
			break;

		hr = Engine::Add_Mesh(m_pDevice
			, RESOURCE_STATIC
			, Engine::MESH_STATIC
			, szMeshKey
			, szMeshPath
			, szMeshName
			, true);
		FAILED_CHECK_RETURN(hr, E_FAIL);
		

		Engine::Safe_Delete_Array(szMeshKey);
		Engine::Safe_Delete_Array(szMeshPath);
		Engine::Safe_Delete_Array(szMeshName);
	}

	LoadFile.close();

	return S_OK;
}

void CLoading::UI_Setting()
{
	HRESULT hr = NULL;

	//lstrcpy(m_szLoadMessage, L"UI_Border 로딩중...");

	for(int i = 0 ; i < 10; ++i)	////////////////////////////////////////////////////////////////////////// 바
	{
		TCHAR* szName;
		TCHAR* szPath;

		szName = new TCHAR[128];
		szPath = new TCHAR[MAX_PATH];

		if(i == 7 || i == 8 || i == 9)
		{
			wsprintf(szName,L"Bar0%d",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/Bar/Bar0%d.tga",i);

			lstrcpy(m_szLoadPath, szPath);
			lstrcat(m_szLoadPath, szName);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_UI
				, Engine::TEXTURE_NORMAL
				, szName
				, szPath, 1);
			FAILED_CHECK_RETURN(hr, );

			
		}

		Engine::Safe_Delete_Array(szName);
		Engine::Safe_Delete_Array(szPath);
	}


	for(int i = 0 ; i < 6; ++i)	////////////////////////////////////////////////////////////////////////// eff
	{
		TCHAR* szName;
		TCHAR* szPath;

		szName = new TCHAR[128];
		szPath = new TCHAR[MAX_PATH];

		if(i == 2 || i == 4)
		{
			wsprintf(szName,L"eff0%d",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/eff/eff0%d.tga",i);

			lstrcpy(m_szLoadPath, szPath);
			lstrcat(m_szLoadPath, szName);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_UI
				, Engine::TEXTURE_NORMAL
				, szName
				, szPath, 1);
			FAILED_CHECK_RETURN(hr, );

			
		}

		Engine::Safe_Delete_Array(szName);
		Engine::Safe_Delete_Array(szPath);
	}


	for(int i = 0 ; i < 5; ++i)	////////////////////////////////////////////////////////////////////////// 아이콘
	{
		TCHAR* szName;
		TCHAR* szPath;

		szName = new TCHAR[128];
		szPath = new TCHAR[MAX_PATH];

		if(i < 4)
		{
			wsprintf(szName,L"Icon0%d",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/Icon/Icon0%d.png",i);
		}
		if(i == 4)
		{
			wsprintf(szName,L"back",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/Icon/back.tga",i);
		}

		lstrcpy(m_szLoadPath, szPath);
		lstrcat(m_szLoadPath, szName);

		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, szName
			, szPath, 1);
		FAILED_CHECK_RETURN(hr, );		

		Engine::Safe_Delete_Array(szName);
		Engine::Safe_Delete_Array(szPath);
	}

	for(int i = 41 ; i < 47; ++i)	////////////////////////////////////////////////////////////////////////// 아이콘
	{
		TCHAR* szName;
		TCHAR* szPath;

		szName = new TCHAR[128];
		szPath = new TCHAR[MAX_PATH];

		wsprintf(szName,L"PlayerInfo%d",i);
		wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/Font/PlayerInfo/PlayerInfo%d.png",i);

		lstrcpy(m_szLoadPath, szPath);
		lstrcat(m_szLoadPath, szName);

		hr = Engine::Add_Texture(m_pDevice
			, RESOURCE_UI
			, Engine::TEXTURE_NORMAL
			, szName
			, szPath, 1);
		FAILED_CHECK_RETURN(hr, );		

		Engine::Safe_Delete_Array(szName);
		Engine::Safe_Delete_Array(szPath);
	}

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/slot/slotsprite%d.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"slotsprite"
		, L"../bin/Resources/Texture/NewUI/Set/slot/slotsprite%d.tga", 35);
	FAILED_CHECK_RETURN(hr, );
	

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/PlayerInfo/Inventory29.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Inventory29"
		, L"../bin/Resources/Texture/NewUI/Set/PlayerInfo/Inventory29.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/PlayerInfo/skin24.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"skin24"
		, L"../bin/Resources/Texture/NewUI/Set/PlayerInfo/skin24.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Crest/Crest01.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Crest01"
		, L"../bin/Resources/Texture/NewUI/Set/Crest/Crest01.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/MonsterBar00.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"MonsterBar00"
		, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/MonsterBar00.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/Aim/AIm.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Aim_UI"
		, L"../bin/Resources/Texture/Aim/AIm.png", 1);
	FAILED_CHECK_RETURN(hr, );

	// 캐스팅 바
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/CastingBar/Bar.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Cast_Bar"
		, L"../bin/Resources/Texture/NewUI/Set/CastingBar/Bar.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/CastingBar/OutLine.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Cast_Outline"
		, L"../bin/Resources/Texture/NewUI/Set/CastingBar/OutLine.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/CastingBar/Slider.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Cast_Slider"
		, L"../bin/Resources/Texture/NewUI/Set/CastingBar/Slider.tga", 1);
	FAILED_CHECK_RETURN(hr, );


	for(int i = 0 ; i < 8; ++i)	
	{
		////////////////////////////////////////////////////////////////////////// 가디언
		{
			TCHAR* szName;
			TCHAR* szPath;

			szName = new TCHAR[128];
			szPath = new TCHAR[MAX_PATH];

			wsprintf(szName,L"Guardian_Skill_%d",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/SKill_Image/Guardian/G_%d.jpg",i);

			lstrcpy(m_szLoadPath, szPath);
			lstrcat(m_szLoadPath, szName);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_UI
				, Engine::TEXTURE_NORMAL
				, szName
				, szPath, 1);
			FAILED_CHECK_RETURN(hr, );		

			Engine::Safe_Delete_Array(szName);
			Engine::Safe_Delete_Array(szPath);
		}
		////////////////////////////////////////////////////////////////////////// 팔라딘
		{
			TCHAR* szName;
			TCHAR* szPath;

			szName = new TCHAR[128];
			szPath = new TCHAR[MAX_PATH];

			wsprintf(szName,L"Paladin_Skill_%d",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/SKill_Image/Paladin/P_%d.jpg",i);

			lstrcpy(m_szLoadPath, szPath);
			lstrcat(m_szLoadPath, szName);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_UI
				, Engine::TEXTURE_NORMAL
				, szName
				, szPath, 1);
			FAILED_CHECK_RETURN(hr, );		

			Engine::Safe_Delete_Array(szName);
			Engine::Safe_Delete_Array(szPath);
		}
		////////////////////////////////////////////////////////////////////////// 레인저
		{
			TCHAR* szName;
			TCHAR* szPath;

			szName = new TCHAR[128];
			szPath = new TCHAR[MAX_PATH];

			wsprintf(szName,L"Ranger_Skill_%d",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/SKill_Image/Ranger/R_%d.jpg",i);

			lstrcpy(m_szLoadPath, szPath);
			lstrcat(m_szLoadPath, szName);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_UI
				, Engine::TEXTURE_NORMAL
				, szName
				, szPath, 1);
			FAILED_CHECK_RETURN(hr, );		

			Engine::Safe_Delete_Array(szName);
			Engine::Safe_Delete_Array(szPath);
		}
		////////////////////////////////////////////////////////////////////////// 매지션
		{
			TCHAR* szName;
			TCHAR* szPath;

			szName = new TCHAR[128];
			szPath = new TCHAR[MAX_PATH];

			wsprintf(szName,L"Magician_Skill_%d",i);
			wsprintf(szPath,L"../bin/Resources/Texture/NewUI/Set/SKill_Image/Magician/M_%d.jpg",i);

			lstrcpy(m_szLoadPath, szPath);
			lstrcat(m_szLoadPath, szName);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_UI
				, Engine::TEXTURE_NORMAL
				, szName
				, szPath, 1);
			FAILED_CHECK_RETURN(hr, );		

			Engine::Safe_Delete_Array(szName);
			Engine::Safe_Delete_Array(szPath);
		}
		
	}

	// SKill Filter
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/SkillImage/Etc/Gra_ring.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_SkillFilter"
		, L"../bin/Resources/Texture/NewUI/Set/Skill_Image/Etc/Gra_ring.tga", 1);
	FAILED_CHECK_RETURN(hr, );
	lstrcpy(m_szLoadPath, L"스킬이미지 완료");


	// 옵션 판떼기
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/border08.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Border"
		, L"../bin/Resources/Texture/NewUI/Set/Option/border08.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	// 옵션 체크
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/BLUIGuildAuthority_I%d.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Check"
		, L"../bin/Resources/Texture/NewUI/Set/Option/BLUIGuildAuthority_I%d.tga", 2);
	FAILED_CHECK_RETURN(hr, );

	// 옵션 바
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/BLUIPC_I6.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Bar"
		, L"../bin/Resources/Texture/NewUI/Set/Option/BLUIPC_I6.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	// 옵션 슬라이더
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Crest00.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Slider"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Crest00.png", 1);
	FAILED_CHECK_RETURN(hr, );

	// 옵션 버튼
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/OptionButton.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Button"
		, L"../bin/Resources/Texture/NewUI/Set/Option/OptionButton.png", 1);
	FAILED_CHECK_RETURN(hr, );

	// 옵션 텍스트
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option0.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Option"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option0.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_Cancel0.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Cancel"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Cancel0.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_Accept0.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Accept"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Accept0.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_Volumetric%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Volumetric"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Volumetric%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_Shadow%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Shadow"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Shadow%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_Range%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Range"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Range%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_Power%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Power"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Power%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_Light%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Light"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Light%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_hdr%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_hdr"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_hdr%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Option_DOF%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_DOF"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_DOF%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Brightness%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Brightness"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Brightness%d.png", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Bloom%d.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Option_Bloom"
		, L"../bin/Resources/Texture/NewUI/Set/Option/Text/Option_Bloom%d.png", 2);
	FAILED_CHECK_RETURN(hr, );


	// 몬스터 HP바
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/MonsterBar00.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_HP_Outline"
		, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/MonsterBar00.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/bar12.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_HP_Bar"
		, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/bar12.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/Bloold1.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_HP_Blood"
		, L"../bin/Resources/Texture/NewUI/Set/MonsterBar/Bloold1.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	// 파티창
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/Outline.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Outline"
		, L"../bin/Resources/Texture/NewUI/Set/Party/Outline.tga", 1);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/characterwindow42.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Ring"
		, L"../bin/Resources/Texture/NewUI/Set/Party/characterwindow42.tga", 1);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/HpBar.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_HpBar"
		, L"../bin/Resources/Texture/NewUI/Set/Party/HpBar.tga", 1);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/MpBar.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_MpBar"
		, L"../bin/Resources/Texture/NewUI/Set/Party/MpBar.tga", 1);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Guardian.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Crest_Guardian"
		, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Guardian.tga", 1);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Magician.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Crest_Magician"
		, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Magician.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Paladin.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Crest_Paladin"
		, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Paladin.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Ranger.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Crest_Ranger"
		, L"../bin/Resources/Texture/NewUI/Set/Party/PartyCrest/Ranger.tga", 1);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Paladin.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Txt_Paladin"
		, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Paladin.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Guardian.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Txt_Guardian"
		, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Guardian.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Magician.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Txt_Magician"
		, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Magician.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Ranger.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Party_Txt_Ranger"
		, L"../bin/Resources/Texture/NewUI/Set/Party/Txt/Ranger.png", 1);
	FAILED_CHECK_RETURN(hr, );

	// 퀘스트
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Accept.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_Main_Accept"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Accept.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Button.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_Main_Button"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Button.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestBack.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_Main_QuestBack"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestBack.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestTxt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_Main_QuestTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestTxt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	// 데미지 텍스트
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Num5/Num_%d.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Dmg"
		, L"../bin/Resources/Texture/NewUI/Set/Num5/Num_%d.tga", 10);
	FAILED_CHECK_RETURN(hr, );

	// 퀘스트 그라데이션
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/TxtGradation.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_Main_TxtGradation"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/TxtGradation.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	// 퀘스트 창
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/Complete.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_CompleteTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/Complete.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/MainGuardian.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_MainGuardian"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/MainGuardian.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/MainMagician.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_MainMagician"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/MainMagician.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/MainPaladin.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_MainPaladin"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/MainPaladin.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/MainRanger.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_MainRanger"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/MainRanger.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/MainTxt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_MainTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/MainTxt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/SubHunt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_SubHunt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/SubHunt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/SubTxt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_SubTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/SubTxt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/QuestMark.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_QuestMark"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/QuestMark%d.tga", 2);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/QuestBar.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_QuestBar"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/QuestBar.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/QuestOutlineBar.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_QuestOutlineBar"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/QuestOutlineBar.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/QuestWindowTxt.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_QuestWindowTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/QuestWindowTxt.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	// 퀘스트 완료시
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/Complete/Back.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_Complete_Back"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/Complete/Back.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/Complete/Mark.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_Complete_Mark"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/Complete/Mark.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/SubTxt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_SubWidow_SubTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/SubTxt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/QuestWindow/SubQuest.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_SubWidow_SubQuest"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/SubQuest.png", 1);
	FAILED_CHECK_RETURN(hr, );

	//////////////////////////////////////////////////////////////////////////


	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/Complete/Guardian.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_CompleteGuardian"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Complete/Guardian.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/Complete/Magician.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_CompleteMagician"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Complete/Magician.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/Complete/Paladin.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_CompletePaladin"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Complete/Paladin.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/Complete/Ranger.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_CompleteRanger"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Complete/Ranger.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/MainQuest/Complete/Hunt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainWidow_CompleteHunt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/Complete/Hunt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/EndMark.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_EndMark"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/EndMark.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/CompleteMainQ.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_MainQuest_CompleteMainQ"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/CompleteMainQ.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/BossQuest/BossCompleteTxt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_BossWidow_BossCompleteTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/BossQuest/BossCompleteTxt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/SubCompleteTxt.png");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest_BossWidow_MoveCompleteTxt"
		, L"../bin/Resources/Texture/NewUI/Set/Quest/MainQuest/QuestWindow/SubCompleteTxt.png", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Guardian.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_PlayerBar_CenterGuardian"
		, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Guardian.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Magician.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_PlayerBar_CenterMagician"
		, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Magician.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Paladin.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_PlayerBar_CenterPaladin"
		, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Paladin.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Ranger.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_UI
		, Engine::TEXTURE_NORMAL
		, L"Texture_PlayerBar_CenterRanger"
		, L"../bin/Resources/Texture/NewUI/Set/Party/CenterCrest/Ranger.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/NumRed/Num_%d.tga");
   hr = Engine::Add_Texture(m_pDevice
      , RESOURCE_UI
      , Engine::TEXTURE_NORMAL
      , L"Texture_Font_MonsterDmg"
      , L"../bin/Resources/Texture/NewUI/Set/NumRed/Num_%d.tga", 10);
   FAILED_CHECK_RETURN(hr, );

   lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/NewUI/Set/Option/BLUIGuildAuthority_I1.tga");
   hr = Engine::Add_Texture(m_pDevice
	   , RESOURCE_UI
	   , Engine::TEXTURE_NORMAL
	   , L"Texture_Option_CheckX"
	   , L"../bin/Resources/Texture/NewUI/Set/Option/BLUIGuildAuthority_I1.tga", 1);
   FAILED_CHECK_RETURN(hr, );
}

HRESULT CLoading::EffectResourceLoad(void)
{
	HRESULT hr;
	// 트레일 텍스쳐
	lstrcpy(m_szLoadPath, L"../bin/Resources/Texture/EffectTexture/Trail/Trail.tga");
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_REMAIN
		, Engine::TEXTURE_NORMAL
		, L"Texture_Trail"
		, L"../bin/Resources/Texture/EffectTexture/Trail/Trail.tga", 1);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Trail_Texture 생성 실패");

	// 트레일 버퍼
	hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_REMAIN
		, Engine::CVIBuffer::BUFFER_TRAIL
		, L"Trail_Sword");
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Trail_Sword 생성 실패");

	hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_REMAIN
		, Engine::CVIBuffer::BUFFER_TRAIL
		, L"Trail_Mace");
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Trail_Mace 생성 실패");

	// Player
	//Guardian
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_DASH.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_INNATE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL1.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL1_TARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL2_EXPLO.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL2_ROOT.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL4.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL4_SUB.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL3.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Guardian/G_SKILL5.dat");


	//Magician
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_SKILL1.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_SKILL2.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_SKILL3.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_SKILL4.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_SKILL5.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_DEFAULT_2_FIRST.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_DEFAULT_2_SECOND.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_DEFAULT1.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_SKILL4_FLOOR.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Magician/M_INNATE.dat");

	//Paladin
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_DASH.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_DASH_TARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_INNATE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1_SUB.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1_TARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL1_WEAPON.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL3_TARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL4.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL4_TARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Paladin/P_SKILL2_BODY.dat");

	//Ranger
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL1.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL3.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL5.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL2_ARROW.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL2_BODY.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL3_ARROW.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL3_TARGETHIT.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILL5_TARGETHIT.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_BLUEARROW.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Ranger/R_SKILLSHOT.dat");

	// Monster
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_EXPLO.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_ORA.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_SLASH.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_SUMMON.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_SUMMON_TARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_THORN.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SPIDER_SMOKE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SPIDER_SMOKE_FLOOR.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SPIDER_EXPLO.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_FIREEXPLO.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_FIREEXPLO2.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_FIREEXPLOSINGLE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_THORN_FLOOR.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_METEO.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/Monster/SKULL_SHIELD.dat");

	// Common
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_HIT.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_HIT_2.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_HIT_BLOOD.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_HIT_CIRCLE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_HITSLASH.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_PLAYTARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_SMOKE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_TARGET.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_TARGETSELECT.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_Quest.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_TORCH.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_PORTAL.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_QUESTPOINT.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_NAVIARROW_BLUE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_NAVIARROW_ORANGE.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_NAVIARROW_RED.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_NAVIBODY.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_STARPOINT.dat");
	EffectReadFile(L"../bin/Resources/Data/Effect/EX/E_CASTLEMETEO.dat");
	

	return S_OK;
}

HRESULT CLoading::EffectReadFile(const wstring& wstrPath)
{
	HRESULT hr = NULL;


	vector<Engine::SAVEDATA*> vecSaveData;

	bool	bCheck = false;

	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, NULL, 

		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte = 0;

	while(true)
	{
		Engine::SAVEDATA pSaveData;	

		ZeroMemory(&pSaveData, sizeof(Engine::SAVEDATA));

		ReadFile(hFile, &pSaveData.szMeshKey						,sizeof(pSaveData.szMeshKey),		&dwByte, NULL);	
		ReadFile(hFile, &pSaveData.szMeshPath						,sizeof(pSaveData.szMeshPath),		&dwByte, NULL);
		ReadFile(hFile, &pSaveData.szTextureKey						,sizeof(pSaveData.szTextureKey),	&dwByte, NULL);
		ReadFile(hFile, &pSaveData.szTexturePath					,sizeof(pSaveData.szTexturePath),	&dwByte, NULL);
		ReadFile(hFile, &pSaveData.szMaskKey						,sizeof(pSaveData.szMaskKey),		&dwByte, NULL);
		ReadFile(hFile, &pSaveData.szMaskPath						,sizeof(pSaveData.szMaskPath),		&dwByte, NULL);	
		ReadFile(hFile, &pSaveData.szNormalKey						,sizeof(pSaveData.szNormalKey),		&dwByte, NULL);
		ReadFile(hFile, &pSaveData.szNormalPath						,sizeof(pSaveData.szNormalPath),	&dwByte, NULL);		

		for(int iIndexCount = 0; iIndexCount < Engine::ACT_END; ++iIndexCount)
		{	
			ReadFile(hFile, &pSaveData.iActionMaxIndex[iIndexCount]	,sizeof(int),					&dwByte, NULL);			
			ReadFile(hFile, &pSaveData.bUnLimited[iIndexCount]		,sizeof(bool),					&dwByte, NULL);		
		}

		for(int iActCount = 0; iActCount <  pSaveData.iActionMaxIndex[Engine::ACT_POS]; ++iActCount)
		{			
			Engine::POSACTION	tPosAction;
			ZeroMemory(&tPosAction, sizeof(Engine::POSACTION));	

			ReadFile(hFile, &tPosAction.vPosValue					,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tPosAction.fStart_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tPosAction.fEnd_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tPosAction.fAcc						,sizeof(float),					&dwByte, NULL);
		}

		for(int iActCount = 0; iActCount < pSaveData.iActionMaxIndex[Engine::ACT_ROT]; ++iActCount)
		{		
			Engine::ROTACTION	tRotAction;
			ZeroMemory(&tRotAction, sizeof(Engine::ROTACTION));

			ReadFile(hFile, &tRotAction.vRotValue					,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tRotAction.fStart_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tRotAction.fEnd_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tRotAction.fAcc						,sizeof(float),					&dwByte, NULL);			
		}		

		for(int iActCount = 0; iActCount < pSaveData.iActionMaxIndex[Engine::ACT_SCALE]; ++iActCount)
		{
			Engine::SCALEACTION	tScaleAction;
			ZeroMemory(&tScaleAction, sizeof(Engine::SCALEACTION));

			ReadFile(hFile, &tScaleAction.vScaleValue				,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tScaleAction.fStart_Time				,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tScaleAction.fEnd_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tScaleAction.fAcc						,sizeof(float),					&dwByte, NULL);			
		}

		for(int iActCount = 0; iActCount < pSaveData.iActionMaxIndex[Engine::ACT_FADE]; ++iActCount)
		{
			Engine::FADEACTION	tFadeAction;
			ZeroMemory(&tFadeAction, sizeof(Engine::FADEACTION));

			ReadFile(hFile, &tFadeAction.fFadeValue						,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tFadeAction.fStart_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tFadeAction.fEnd_Time						,sizeof(float),					&dwByte, NULL);		
		}

		for(int iActCount = 0; iActCount < pSaveData.iActionMaxIndex[Engine::ACT_UV]; ++iActCount)
		{
			Engine::UVACTION	tUVAction;
			ZeroMemory(&tUVAction, sizeof(Engine::UVACTION));

			ReadFile(hFile, &tUVAction.fU								,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fV								,sizeof(float),					&dwByte, NULL);			
			ReadFile(hFile, &tUVAction.fStart_Time						,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fEnd_Time						,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fAccU							,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVAction.fAccV							,sizeof(float),					&dwByte, NULL);		
		}

		for(int iActCount = 0; iActCount < pSaveData.iActionMaxIndex[Engine::ACT_UVSIZE]; ++iActCount)
		{
			Engine::UVSIZEACTION	tUVSizeAction;
			ZeroMemory(&tUVSizeAction, sizeof(Engine::UVSIZEACTION));

			ReadFile(hFile, &tUVSizeAction.fUSize								,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVSizeAction.fVSize								,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVSizeAction.fStart_Time							,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tUVSizeAction.fEnd_Time							,sizeof(float),					&dwByte, NULL);				
		}

		for(int iActCount = 0; iActCount < pSaveData.iActionMaxIndex[Engine::ACT_COLOR]; ++iActCount)
		{
			Engine::COLORACTION	tColorAction;
			ZeroMemory(&tColorAction, sizeof(Engine::COLORACTION));

			ReadFile(hFile, &tColorAction.vRGB							,sizeof(D3DXVECTOR3),			&dwByte, NULL);
			ReadFile(hFile, &tColorAction.fNext_Time					,sizeof(float),					&dwByte, NULL);
			ReadFile(hFile, &tColorAction.fChange_Time					,sizeof(float),					&dwByte, NULL);		
		}

		ReadFile(hFile, &pSaveData.vOriPos							,sizeof(D3DXVECTOR3),			&dwByte, NULL);
		ReadFile(hFile, &pSaveData.vOriRot							,sizeof(D3DXVECTOR3),			&dwByte, NULL);
		ReadFile(hFile, &pSaveData.vOriScale						,sizeof(D3DXVECTOR3),			&dwByte, NULL);

		ReadFile(hFile, &pSaveData.iOriCycle						,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.iPass							,sizeof(int),					&dwByte, NULL);

		ReadFile(hFile, &pSaveData.iSpriteMaxCount					,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.iSpriteX							,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.iSpriteY							,sizeof(int),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.fSpriteTotalTime					,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &pSaveData.fFade							,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.fOriFade							,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &pSaveData.fAliveStart_Time					,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.fAliveEnd_Time					,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &pSaveData.fOriUPos							,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.fOriVPos							,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &pSaveData.fOriUSize						,sizeof(float),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.fOriVSize						,sizeof(float),					&dwByte, NULL);

		ReadFile(hFile, &pSaveData.bColorOn							,sizeof(bool),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.bBillOn							,sizeof(bool),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.bHoldOn							,sizeof(bool),					&dwByte, NULL);
		ReadFile(hFile, &pSaveData.bSingleBillOn					,sizeof(bool),					&dwByte, NULL);
		if(dwByte == 0)
		{			
			break;
		}	

		vector<wstring>::iterator Meshiter = vecMeshCheck.begin();
		vector<wstring>::iterator Meshiter_end = vecMeshCheck.end();
		for(; Meshiter != Meshiter_end; ++Meshiter)
		{
			if(!lstrcmp((*Meshiter).c_str(), pSaveData.szMeshKey))
				bCheck = true;			
		}

		if(bCheck == false)
		{
			lstrcpy(m_szLoadPath, pSaveData.szMeshPath);
			lstrcat(m_szLoadPath, pSaveData.szMeshKey);

			hr = Engine::Add_Mesh(m_pDevice
				, RESOURCE_REMAIN
				, Engine::MESH_EFFECT
				, pSaveData.szMeshKey
				, pSaveData.szMeshPath
				, pSaveData.szMeshKey);
			FAILED_CHECK_RETURN(hr, E_FAIL);

			vecMeshCheck.push_back(pSaveData.szMeshKey);
		}

		bCheck = false;

		vector<wstring>::iterator Texiter = vecTextureCheck.begin();
		vector<wstring>::iterator Texiter_end = vecTextureCheck.end();
		for(; Texiter != Texiter_end; ++Texiter)
		{
			if(!lstrcmp((*Texiter).c_str(), pSaveData.szTextureKey))
				bCheck = true;			
		}

		if(bCheck == false)
		{
			lstrcpy(m_szLoadPath, pSaveData.szTexturePath);
			lstrcat(m_szLoadPath, pSaveData.szTextureKey);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_REMAIN
				, Engine::TEXTURE_NORMAL
				, pSaveData.szTextureKey
				, pSaveData.szTexturePath, 1);
			FAILED_CHECK_RETURN(hr, E_FAIL);

			vecTextureCheck.push_back(pSaveData.szTextureKey);
		}

		bCheck = false;

		vector<wstring>::iterator MaskTexiter = vecTextureCheck.begin();
		vector<wstring>::iterator MaskTexiter_end = vecTextureCheck.end();
		for(; MaskTexiter != MaskTexiter_end; ++MaskTexiter)
		{
			if(!lstrcmp((*MaskTexiter).c_str(), pSaveData.szMaskKey))
				bCheck = true;			
		}

		if(bCheck == false)
		{
			lstrcpy(m_szLoadPath, pSaveData.szMaskPath);
			lstrcat(m_szLoadPath, pSaveData.szMaskKey);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_REMAIN
				, Engine::TEXTURE_NORMAL
				, pSaveData.szMaskKey
				, pSaveData.szMaskPath, 1);
			FAILED_CHECK_RETURN(hr, E_FAIL);

			vecTextureCheck.push_back(pSaveData.szMaskKey);
		}

		bCheck = false;


		vector<wstring>::iterator NormalTexiter = vecTextureCheck.begin();
		vector<wstring>::iterator NormalTexiter_end = vecTextureCheck.end();
		for(; NormalTexiter != NormalTexiter_end; ++NormalTexiter)
		{
			if(!lstrcmp((*NormalTexiter).c_str(), pSaveData.szNormalKey))
				bCheck = true;			
		}

		if(bCheck == false)
		{
			lstrcpy(m_szLoadPath, pSaveData.szNormalPath);
			lstrcat(m_szLoadPath, pSaveData.szNormalKey);

			hr = Engine::Add_Texture(m_pDevice
				, RESOURCE_REMAIN
				, Engine::TEXTURE_NORMAL
				, pSaveData.szNormalKey
				, pSaveData.szNormalPath, 1);
			FAILED_CHECK_RETURN(hr, E_FAIL);

			vecTextureCheck.push_back(pSaveData.szNormalKey);
		}	

		bCheck = false;
	}

	CloseHandle(hFile);

	return S_OK;
}
