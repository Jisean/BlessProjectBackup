#include "Layer.h"
#include "GameObject.h"

USING(Engine)

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{
	m_pDevice->AddRef();
	m_htObjlist.Ready_Table(1499);
}

Engine::CLayer::~CLayer(void)
{
	
}

HRESULT Engine::CLayer::AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		OBJECTLIST* pObjList = (OBJECTLIST*)m_htObjlist.Search_TableData(pObjectKey);
		if(NULL == pObjList)
		{
			pObjList = new OBJECTLIST;
			m_htObjlist.Insert_Table(pObjectKey, pObjList);
		}
		pObjList->push_back(pGameObject);
	}

	return S_OK;
}

_int Engine::CLayer::Update(const _float& fTimeDelta)
{
	size_t iTableSize =	m_htObjlist.Get_TableSize();

	_int		iResult = 0;

	for(size_t i = 0; i < iTableSize; ++i)
	{
		OBJECTLIST* pObjList = (OBJECTLIST*)m_htObjlist.Search_TableData(i);
		if(NULL != pObjList)
		{
			OBJECTLIST::iterator	iterList = pObjList->begin();
			OBJECTLIST::iterator	iterList_end = pObjList->end();

			for (; iterList != iterList_end; ++iterList)		
			{
				if((*iterList)->GetDead())
					continue;

				iResult = (*iterList)->Update(fTimeDelta);
				if(iResult & 0x80000000)
					return iResult;
			}
		}
	}

	return iResult;
}

void Engine::CLayer::Render(void)
{
	size_t iTableSize =	m_htObjlist.Get_TableSize();

	_int		iResult = 0;

	for(size_t i = 0; i < iTableSize; ++i)
	{
		OBJECTLIST* pObjList = (OBJECTLIST*)m_htObjlist.Search_TableData(i);
		if(NULL != pObjList)
		{
			OBJECTLIST::iterator	iterList = pObjList->begin();
			OBJECTLIST::iterator	iterList_end = pObjList->end();

			for (; iterList != iterList_end; ++iterList)		
			{
				if((*iterList)->GetDead())
					continue;

				(*iterList)->Render();
			}
		}
	}
}



const Engine::CComponent* Engine::CLayer::GetComponent(const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	OBJECTLIST* pObjList = (OBJECTLIST*)m_htObjlist.Search_TableData(pObjKey);
	
	if(NULL == pObjList)
		return NULL;

	OBJECTLIST::iterator	iterList = pObjList->begin();
	OBJECTLIST::iterator	iterList_end = pObjList->end();

	for( ; iterList != iterList_end; ++iterList)
	{
		const CComponent* pComponent = (*iterList)->GetComponent(pComponentKey);
		if(pComponent != NULL)
			return pComponent;
	}

	return NULL;
}

Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);
}

void Engine::CLayer::Free(void)
{
	size_t iTableSize =	m_htObjlist.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		OBJECTLIST*		pObjList = (OBJECTLIST*)m_htObjlist.Erase_Slot(i);

		if(NULL != pObjList)
		{
			OBJECTLIST::iterator	iterList = pObjList->begin();
			OBJECTLIST::iterator	iterList_end = pObjList->end();

			for(iterList ; iterList != iterList_end; ++iterList)
			{
				Engine::Safe_Release((*iterList));
			}
			Engine::Safe_Delete(pObjList);
		}
	}
	m_htObjlist.Erase_Table();

	Engine::Safe_Release(m_pDevice);
}

Engine::CLayer::OBJECTLIST* Engine::CLayer::GetObjList(const TCHAR* pObjKey)
{
	OBJECTLIST*		pObjList = (OBJECTLIST*)m_htObjlist.Search_TableData(pObjKey);
	return pObjList;
}
