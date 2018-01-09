#include "stdafx.h"
#include "SecondThread.h"

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include <process.h>

CSecondThread::CSecondThread(void)
: m_pDevice(NULL)
, m_hThread(NULL)
, m_bComplete(false)
{
	ZeroMemory(m_szLoadMessage, sizeof(TCHAR) * MAX_PATH);
	ZeroMemory(m_szLoadPath, sizeof(TCHAR) * MAX_PATH);
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
}

CSecondThread::~CSecondThread(void)
{
	
}

bool CSecondThread::GetComplete(void)
{
	return m_bComplete;
}

const TCHAR* CSecondThread::GetLoadMessage(void)
{
	return m_szLoadMessage;
}

const TCHAR* CSecondThread::GetLoadPath(void)
{
	return m_szLoadPath;
}

HRESULT CSecondThread::InitLoading(void)
{
	m_pDevice = Engine::Get_GraphicDev();

	InitializeCriticalSection(&m_CSKey);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
	if(m_hThread == NULL)
		return E_FAIL;

	return S_OK;
}

HRESULT CSecondThread::PlayerLoad(void)
{
	HRESULT hr = NULL;

	lstrcpy(m_szLoadMessage, L"플레이어 로딩중...");
	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Paladin/PaladinNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Paladin"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Paladin/"
		, L"PaladinNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Guardian/GuardianNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Guardian"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Guardian/"
		, L"GuardianNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Ranger/RangerNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Ranger"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Ranger/"
		, L"RangerNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Magician/MagicianNew.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Magician"
		, L"../bin/Resources/Mesh/DynamicMesh/Job/Job_Magician/"
		, L"MagicianNew.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	//////////////////////////////////////////////////////////////////////////장비
	lstrcpy(m_szLoadMessage, L"아이템 로딩중...");
	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Mace/Mace.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Mace"
		, L"../bin/Resources/Mesh/StaticMesh/Mace/"
		, L"Mace.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Shield/Shield.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Shield"
		, L"../bin/Resources/Mesh/StaticMesh/Shield/"
		, L"Shield.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/STAFF/STAFF.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_STAFF"
		, L"../bin/Resources/Mesh/StaticMesh/STAFF/"
		, L"STAFF.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Sword/Sword.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Sword"
		, L"../bin/Resources/Mesh/StaticMesh/Sword/"
		, L"Sword.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadPath, L"../bin/Resources/Mesh/StaticMesh/Bow/Bow.X");
	hr = Engine::Add_Mesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Bow"
		, L"../bin/Resources/Mesh/StaticMesh/Bow/"
		, L"Bow.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadMessage, L"로딩 완료...");
	lstrcpy(m_szLoadPath, L"Loading Complete...");
	m_bComplete = true;

	return S_OK;
}

CSecondThread* CSecondThread::Create(void)
{
	CSecondThread*	pLoading = new CSecondThread;
	if(FAILED(pLoading->InitLoading()))
		Engine::Safe_Delete(pLoading);

	return pLoading;
}

void CSecondThread::Free(void)
{
	DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	Engine::Safe_Release(m_pDevice);
}

UINT WINAPI CSecondThread::LoadingFunction(void* pArg)
{
	CSecondThread*		pLoading = (CSecondThread*)pArg;	

	pLoading->PlayerLoad();

	return 0;
}