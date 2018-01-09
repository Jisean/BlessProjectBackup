#include "LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(Engine::CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	
}

const D3DLIGHT9* Engine::CLightMgr::Get_LightInfo(const _ulong& dwIndex) const
{
	LIGHTLIST::const_iterator	iter = m_LightList.begin();

	for (_ulong i = 0; i < dwIndex; ++i)
		++iter;
	
	return (*iter)->Get_LightInfo();	
}

HRESULT Engine::CLightMgr::AddLight(LPDIRECT3DDEVICE9 pDevice , const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx)
{
	if(pDevice == NULL || pLightInfo == NULL)
		return E_FAIL;

	CLight* pLight = CLight::Create(pDevice, pLightInfo, dwLightIdx);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_LightList.push_back(pLight);
	return S_OK;
}

void Engine::CLightMgr::Render_Light(LPD3DXEFFECT pEffect)
{
	LIGHTLIST::iterator	iter = m_LightList.begin();
	LIGHTLIST::iterator	iter_end = m_LightList.end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Light(pEffect);
	}

}

void Engine::CLightMgr::Free(void)
{
	for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
	m_LightList.clear();
}

