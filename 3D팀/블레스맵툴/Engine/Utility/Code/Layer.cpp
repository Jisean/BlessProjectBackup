#include "Layer.h"
#include "GameObject.h"

USING(Engine)

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

Engine::CLayer::~CLayer(void)
{
	
}

HRESULT Engine::CLayer::AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		MAPOBJLIST::iterator	iter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjectKey));
		if(iter == m_mapObjlist.end())
		{
			m_mapObjlist[pObjectKey] = OBJECTLIST();
			m_mapObjlist[pObjectKey].push_back(pGameObject);
		}
		else
			(*iter).second.push_back(pGameObject);
		//m_mapObjlist[pObjectKey].push_back(pGameObject);
	}

	return S_OK;
}

_int Engine::CLayer::Update(const _float& fTimeDelta)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();
	
	_int		iResult = 0;

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
		{
			iResult = (*iterList)->Update(fTimeDelta);
			if(iResult & 0x80000000)
				return iResult;
		}		
	}	

	return iResult;
}

void Engine::CLayer::Render(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
		{
			(*iterList)->Render();
		}		
	}	
}



const Engine::CComponent* Engine::CLayer::GetComponent(const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	MAPOBJLIST::iterator	mapiter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjKey));
	if(mapiter == m_mapObjlist.end())
		return NULL;

	OBJECTLIST::iterator	iterlist = mapiter->second.begin();
	OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

	for( ; iterlist != iterlist_end; ++iterlist)
	{
		const CComponent* pComponent = (*iterlist)->GetComponent(pComponentKey);
		if(pComponent != NULL)
			return pComponent;
	}
	return NULL;
}

const Engine::VTXTEX* Engine::CLayer::GetTerrainVertex(const TCHAR* wstrObjKey)
{
	MAPOBJLIST::iterator		iter = m_mapObjlist.find(wstrObjKey);

	if(iter == m_mapObjlist.end())
		return NULL;

	return iter->second.front()->GetTerrainVertex();
}

void Engine::CLayer::DeleteByKey(const TCHAR* wstrObjKey)
{
	//MAPOBJLIST::iterator		mapiter = m_mapObjlist.find(wstrObjKey);
	MAPOBJLIST::iterator	mapiter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(wstrObjKey));

	if(mapiter == m_mapObjlist.end())
		return;

	OBJECTLIST::iterator Objiter = mapiter->second.begin();
	OBJECTLIST::iterator Objiter_end = mapiter->second.end();

	for(Objiter; Objiter != Objiter_end; ++Objiter)
	{
		Safe_Release(*Objiter);
	}
	mapiter->second.clear();
	m_mapObjlist.erase(mapiter);
}

void Engine::CLayer::DeleteByCompare(const CGameObject* pGameObject)
{
	MAPOBJLIST::iterator		mapiter = m_mapObjlist.begin();
	MAPOBJLIST::iterator		mapiter_end = m_mapObjlist.end();

	for(mapiter; mapiter != mapiter_end; ++mapiter)
	{
		OBJECTLIST::iterator	iterlist		= mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end	= mapiter->second.end();

		for(iterlist; iterlist != iterlist_end; ++iterlist)
		{
			if((*iterlist) == pGameObject)
				break;
		}
		if(iterlist != iterlist_end)
		{
			mapiter->second.erase(iterlist);
			break;
		}
	}
	if(mapiter == mapiter_end)
		return;

	if(mapiter->second.empty())
		m_mapObjlist.erase(mapiter);
}


Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);
}

void Engine::CLayer::Free(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
			Engine::Safe_Release((*iterList));

		iter->second.clear();
	}
	m_mapObjlist.clear();

	Engine::Safe_Release(m_pDevice);
}
