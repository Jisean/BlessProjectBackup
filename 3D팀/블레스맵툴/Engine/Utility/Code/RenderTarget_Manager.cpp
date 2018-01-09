#include "RenderTarget_Manager.h"
#include "RenderTarget.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTarget_Manager)

Engine::CRenderTarget_Manager::CRenderTarget_Manager(void)
{

}

Engine::CRenderTarget_Manager::~CRenderTarget_Manager(void)
{

}

void Engine::CRenderTarget_Manager::SetUp_TargetOnShader(LPD3DXEFFECT pEffect, const TCHAR* pTargetTag, const char* pConstantName)
{
	CRenderTarget* pTarget = Find_RenderTarget(pTargetTag);

	if(NULL == pTarget)
		return;

	pTarget->SetUp_TargetOnShader(pEffect, pConstantName);
}

HRESULT Engine::CRenderTarget_Manager::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if(NULL != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(pGraphicDev, iSizeX, iSizeY, Format, Color);
	if(NULL == pRenderTarget)
		return E_FAIL;

	m_mapTargets.insert(MAPTARGET::value_type(pTargetTag, pRenderTarget));

	return S_OK;
}

HRESULT Engine::CRenderTarget_Manager::Ready_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if(NULL == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);
		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, MRTList));
	}
	else
		pMRTList->push_back(pRenderTarget);

	return S_OK;
}

HRESULT Engine::CRenderTarget_Manager::Ready_DebugBuffer(const TCHAR* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if(NULL == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_DebugBuffer(fX, fY, fSizeX, fSizeY);
}
void Engine::CRenderTarget_Manager::Begin_MRT(const TCHAR* pMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
		return ;

	list<CRenderTarget*>::iterator	iter = pMRTList->begin();
	list<CRenderTarget*>::iterator	iter_end = pMRTList->end();

	

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Clear_RenderTarget();
	}		

	_uint		iIndex = 0;

	iter = pMRTList->begin();

	for (; iter != iter_end; ++iter, ++iIndex)
	{
		(*iter)->SetUp_OnGraphicDev(iIndex);
	}	
}

void Engine::CRenderTarget_Manager::End_MRT(const TCHAR* pMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
		return ;

	list<CRenderTarget*>::iterator	iter = pMRTList->begin();
	list<CRenderTarget*>::iterator	iter_end = pMRTList->end();

	_uint		iIndex = 0;

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Release_OnGraphicDev();
	}	

}

void Engine::CRenderTarget_Manager::Render_DebugBuffer(const TCHAR* pMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
		return ;

	list<CRenderTarget*>::iterator	iter = pMRTList->begin();
	list<CRenderTarget*>::iterator	iter_end = pMRTList->end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_DebugBuffer();
	}

	
}


CRenderTarget* Engine::CRenderTarget_Manager::Find_RenderTarget(const TCHAR* pTargetTag)
{
	MAPTARGET::iterator	iter = find_if(m_mapTargets.begin(), m_mapTargets.end(), CTagFinder(pTargetTag));

	if(iter == m_mapTargets.end())
		return NULL;

	return iter->second;
}

list<CRenderTarget*>* Engine::CRenderTarget_Manager::Find_MRT(const TCHAR* pMRTTag)
{
	MAPMRT::iterator	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTagFinder(pMRTTag));

	if(iter == m_mapMRT.end())
		return NULL;

	return &iter->second;
}

void Engine::CRenderTarget_Manager::Free(void)
{
	for_each(m_mapTargets.begin(), m_mapTargets.end(), CRelease_Pair());
	m_mapTargets.clear();
}

