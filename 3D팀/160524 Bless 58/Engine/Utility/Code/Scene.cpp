#include "Scene.h"
#include "Layer.h"
#include "FontMgr.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_fTimeAcc(0.f)
, m_dwFPSCnt(0)
{
	ZeroMemory(m_szFPS, sizeof(_tchar) * 128);
	ZeroMemory(m_szMemoryLoad, sizeof(_tchar) * 128);
	ZeroMemory(m_szTotal, sizeof(_tchar) * 128);

	m_pDevice->AddRef();
}

Engine::CScene::~CScene(void)
{
	
}

const Engine::CComponent* Engine::CScene::GetComponent(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);
	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetComponent(pObjKey, pComponentKey);
}

_int Engine::CScene::Update(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	_int		iResult = 0;

	for (; iter != iter_end; ++iter)
	{
		iResult = iter->second->Update(fTimeDelta);
		if(iResult & 0x80000000)
			return iResult;
	}
	return iResult;
}

void Engine::CScene::Render(void)
{

}

void Engine::CScene::Render_FPS(void)
{
	++m_dwFPSCnt;

	if(m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwFPSCnt);
		Render_Memory();
		m_dwFPSCnt = 0;
		m_fTimeAcc = 0.f;
	}

	CFontMgr::GetInstance()->Render(L"¸¼Àº°íµñ", _vec3(WINCX - 150.f, 0.f, 0.f), m_szFPS, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	CFontMgr::GetInstance()->Render(L"¸¼Àº°íµñ", _vec3(0.f, WINCY - 50.f, 0.f), m_szMemoryLoad, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	CFontMgr::GetInstance()->Render(L"¸¼Àº°íµñ", _vec3(0.f, WINCY - 30.f, 0.f), m_szTotal, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
}

void Engine::CScene::Render_Memory(void)
{
	MEMORYSTATUS memoryStatus;
	GlobalMemoryStatus(&memoryStatus);

	wsprintf(m_szMemoryLoad, L"»ç¿ë ¸Þ¸ð¸® ºñÀ²: %d%%", memoryStatus.dwMemoryLoad);

	SIZE_T dwAvailVirtual = memoryStatus.dwAvailVirtual>>20;
	SIZE_T dwTotalVirtual = memoryStatus.dwTotalVirtual>>20;
	wsprintf(m_szTotal, L"ÀüÃ¼¸Þ¸ð¸® »ç¿ë: %dMB/%dMB",  dwTotalVirtual - dwAvailVirtual, dwTotalVirtual);
}

void Engine::CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CRelease_Pair());
	m_mapLayer.clear();

	Engine::Safe_Release(m_pDevice);
}

vector<CGameObject*>* Engine::CScene::GetObjList(const WORD LayerID, const TCHAR* pObjKey)
{
	MAPLAYER::iterator iter = m_mapLayer.find(LayerID);
	return (*iter).second->GetObjList(pObjKey);
}

HRESULT Engine::CScene::AddObject(const WORD LayerID, const TCHAR* pObjectKey, CGameObject* pGameObject)
{
	return m_mapLayer[LayerID]->AddObject(pObjectKey, pGameObject);
}