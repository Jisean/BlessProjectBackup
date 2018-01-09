#include "stdafx.h"
#include "Loading.h"

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include <process.h>

CLoading::CLoading(LOADINGID eLoadID)
: m_eLoadID(eLoadID)
, m_pDevice(NULL)
, m_hThread(NULL)
, m_bComplete(false)
{
	ZeroMemory(m_szLoadMessage, sizeof(TCHAR) * 128);
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

HRESULT CLoading::InitLoading(void)
{
	m_pDevice = Engine::Get_GraphicDev();

	InitializeCriticalSection(&m_CSKey);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
	if(m_hThread == NULL)
		return E_FAIL;

	return S_OK;
}

void CLoading::StageLoading(void)
{
	HRESULT hr = NULL;

	lstrcpy(m_szLoadMessage, L"텍스쳐 로딩중...");
	//Texture
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Terrain"
		, L"../bin/Resources/Texture/Terrain/Grass_%d.tga", 2);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Filter"
		, L"../bin/Resources/Texture/Terrain/ColorHeight.png", 1);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_UI"
		, L"../bin/Resources/Texture/UI.jpg", 1);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_SkyBox"
		, L"../bin/Resources/Texture/SkyBox/burger%d.dds", 4);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Brush"
		, L"../bin/Resources/Texture/Terrain/Aura%d.tga", 1);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Effect"
		, L"../bin/Resources/Texture/Explosion/Explosion%d.png", 90);
	FAILED_CHECK_RETURN(hr, );


	

	lstrcpy(m_szLoadMessage, L"버퍼 로딩중...");
	//Buffer
	hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_TERRAIN
		, L"Buffer_TerrainTex"
		, VTXCNTX, VTXCNTZ, VTXITV);
	FAILED_CHECK_RETURN_MSG(hr, , L"Buffer_TerrainTex 생성 실패");

	hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_CUBETEX
		, L"Buffer_CubeTex");
	FAILED_CHECK_RETURN_MSG(hr, , L"Buffer_CubeTex 생성 실패");

	lstrcpy(m_szLoadMessage, L"메쉬 로딩중...");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Stone"
		, L"../bin/Resources/Mesh/StaticMesh/TombStone/"
		, L"TombStone.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Sylva"
		, L"../bin/Resources/Mesh/DynamicMesh/PlayerXFile/"
		, L"Player.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Bow"
		, L"../bin/Resources/Mesh/StaticMesh/Bow/"
		, L"Bow.X");
	FAILED_CHECK_RETURN(hr, );

	lstrcpy(m_szLoadMessage, L"로딩 완료...");
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
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	switch(pLoading->m_eLoadID)
	{
	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
		break;

	case CLoading::LOADING_STAGE1:
		break;
	}
	return 0;
}