#include "LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(Engine::CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{
	m_htLightMap.Ready_Table(11);
}

Engine::CLightMgr::~CLightMgr(void)
{
	
}

const D3DLIGHT9* Engine::CLightMgr::Get_LightInfo(const _ulong& dwIndex, TCHAR*	pKeyName)
{
	vector<CLight*>*	pvecLight = (vector<CLight*>*)m_htLightMap.Search_TableData(pKeyName);

	return ((*pvecLight)[dwIndex])->Get_LightInfo();
}

HRESULT Engine::CLightMgr::AddLight(LPDIRECT3DDEVICE9 pDevice , const D3DLIGHT9* pLightInfo, TCHAR*	pKeyName)
{
	if(pDevice == NULL || pLightInfo == NULL)
		return E_FAIL;

	vector<CLight*>*	pvecLight = (vector<CLight*>*)m_htLightMap.Search_TableData(pKeyName);

	if(pvecLight == NULL)
	{
		pvecLight = new vector<CLight*>;
		m_htLightMap.Insert_Table(pKeyName, pvecLight);
	}

	CLight* pLight = CLight::Create(pDevice, pLightInfo);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	pvecLight->push_back(pLight);
	return S_OK;
}

void Engine::CLightMgr::Render_Light(LPD3DXEFFECT pEffect)
{
	
	size_t iTableSize =	m_htLightMap.Get_TableSize();

	_int		iResult = 0;
	for(size_t i = 0; i < iTableSize; ++i)
	{
		LIGHTVEC* pvecLight = (LIGHTVEC*)m_htLightMap.Search_TableData(i);
		if(NULL != pvecLight)
		{
			for (_uint i = 0; i != pvecLight->size(); ++i)
			{
				if(*((*pvecLight)[i]->Get_LightRender()) == true)
					(*pvecLight)[i]->Render_Light(pEffect);
			}
		}
	}
}

void Engine::CLightMgr::Free(void)
{
	size_t iTableSize =	m_htLightMap.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		LIGHTVEC*		pvecLight = (LIGHTVEC*)m_htLightMap.Erase_Slot(i);
		if(NULL != pvecLight)
		{
			for (_uint i = 0; i != pvecLight->size(); ++i)	
				Engine::Safe_Release((*pvecLight)[i]);

			Engine::Safe_Delete(pvecLight);
		}
	}
	m_htLightMap.Erase_Table();
}

Engine::CLight* Engine::CLightMgr::Get_Light(TCHAR*	pKeyName, const DWORD& dwLightIdx)
{
	vector<CLight*>*	pvecLight = (vector<CLight*>*)m_htLightMap.Search_TableData(pKeyName);

	return (*pvecLight)[dwLightIdx];
}

int	Engine::CLightMgr::Get_LightCnt(TCHAR*	pKeyName)
{
	vector<CLight*>*	pvecLight = (vector<CLight*>*)m_htLightMap.Search_TableData(pKeyName);

	return pvecLight->size();
}

void Engine::CLightMgr::Set_LightRender(TCHAR* pKeyName, bool bRender)
{
	vector<CLight*>*	pvecLight = (vector<CLight*>*)m_htLightMap.Search_TableData(pKeyName);

	if(pvecLight == NULL)
		return;

	if(*((*pvecLight)[0]->Get_LightRender()) == bRender)
		return;

	for(_uint i = 0; i < pvecLight->size(); ++i)
	{
		*((*pvecLight)[i]->Get_LightRender()) = bRender;
	}
}

void Engine::CLightMgr::Set_LightRender(UINT iLightKey, bool bRender)
{
	vector<CLight*>*	pvecLight = (vector<CLight*>*)m_htLightMap.Search_TableData(iLightKey);

	if(pvecLight == NULL)
		return;

	// 방향성 광원 제외
	if((vector<CLight*>*)m_htLightMap.Search_TableData(L"Directional") == pvecLight)
		return;

	if((vector<CLight*>*)m_htLightMap.Search_TableData(L"Temp") == pvecLight)
		return;

	if(*((*pvecLight)[0]->Get_LightRender()) == bRender)
		return;

	for(_uint i = 0; i < pvecLight->size(); ++i)
	{
		*((*pvecLight)[i]->Get_LightRender()) = bRender;
	}
}

size_t Engine::CLightMgr::Get_LightKeyCnt(void)
{
	return m_htLightMap.Get_TableSize();
}
