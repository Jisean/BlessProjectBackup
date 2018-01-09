#include "RenderTarget_Manager.h"
#include "RenderTarget.h"
#include "GraphicDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTarget_Manager)

Engine::CRenderTarget_Manager::CRenderTarget_Manager(void)
{
	m_htTargets.Ready_Table(29);
	m_htMRT.Ready_Table(29);
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
	CRenderTarget*	pRenderTarget = Find_RenderTarget(pTargetTag);

	if(NULL != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(pGraphicDev, iSizeX, iSizeY, Format, Color);
	if(NULL == pRenderTarget)
		return E_FAIL;

	if(!lstrcmp(L"Shadow_Static", pTargetTag))
		Engine::CGraphicDev::GetInstance()->GetDevice()->CreateDepthStencilSurface(iSizeX, iSizeX, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowStaticZ, NULL);
	else if(!lstrcmp(L"Shadow_Dynamic", pTargetTag))
		Engine::CGraphicDev::GetInstance()->GetDevice()->CreateDepthStencilSurface(iSizeX, iSizeX, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDynamicZ, NULL);

	m_htTargets.Insert_Table(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT Engine::CRenderTarget_Manager::Ready_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if(NULL == pRenderTarget)
		return E_FAIL;

	vector<CRenderTarget*>*	pvecMTR = Find_MRT(pMRTTag);

	if(NULL == pvecMTR)
	{
		vector<CRenderTarget*>*		pvecMTR = new vector<CRenderTarget*>;
		pvecMTR->reserve(10);
		pvecMTR->push_back(pRenderTarget);
		m_htMRT.Insert_Table(pMRTTag, pvecMTR);
	}
	else
		pvecMTR->push_back(pRenderTarget);

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
	vector<CRenderTarget*>* pvecMRT = Find_MRT(pMRTTag);

	if(NULL == pvecMRT)
		return;

	Engine::CGraphicDev::GetInstance()->GetDevice()->GetViewport(&m_pBackViewport);
	vector<CRenderTarget*>::iterator	iter		= pvecMRT->begin();
	vector<CRenderTarget*>::iterator	iter_end	= pvecMRT->end();

	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->Clear_RenderTarget();
	}

	_uint	iIndex = 0;

	iter = pvecMRT->begin();

	for(iter; iter != iter_end; ++iter, ++iIndex)
	{
		(*iter)->SetUp_OnGraphicDev(iIndex);
	}

	// 그림자
	if(!lstrcmp(pMRTTag, L"M_Shadow_Static") || !lstrcmp(pMRTTag, L"M_Shadow_Dynamic"))
	{
		Engine::CGraphicDev::GetInstance()->GetDevice()->GetDepthStencilSurface(&m_pBackZ);

		DWORD	dwViewPortWidth, dwViewPortHeight;

		if(!lstrcmp(pMRTTag, L"M_Shadow_Static"))
		{
			Engine::CGraphicDev::GetInstance()->GetDevice()->SetDepthStencilSurface(m_pShadowStaticZ);
			dwViewPortWidth = 8100;
			dwViewPortHeight = 6075;
		}
		else
		{
			Engine::CGraphicDev::GetInstance()->GetDevice()->SetDepthStencilSurface(m_pShadowDynamicZ);
			dwViewPortWidth = 2048;
			dwViewPortHeight = 1536;
		}
	

		D3DVIEWPORT9 viewport = {0,0      // 좌측상단좌표
			//,13000, 9750
			, dwViewPortWidth	// 폭
			, dwViewPortHeight	// 높이
			, 0.0f,1.0f};     // 전면 후면
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetViewport(&viewport);
		Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	}
	
}

void Engine::CRenderTarget_Manager::End_MRT(const TCHAR* pMRTTag)
{
	vector<CRenderTarget*>*	pvecMRT = Find_MRT(pMRTTag);

	if(NULL == pvecMRT)
		return ;

	vector<CRenderTarget*>::iterator	iter = pvecMRT->begin();
	vector<CRenderTarget*>::iterator	iter_end = pvecMRT->end();

	_uint		iIndex = 0;

	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->Release_OnGraphicDev();
	}

	// 그림자
	if(!lstrcmp(pMRTTag, L"M_Shadow_Static") || !lstrcmp(pMRTTag, L"M_Shadow_Dynamic"))
	{
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetDepthStencilSurface(m_pBackZ);
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetViewport(&m_pBackViewport);
		Engine::Safe_Release(m_pBackZ);
	}

}

void Engine::CRenderTarget_Manager::Render_DebugBuffer(const TCHAR* pMRTTag)
{
	vector<CRenderTarget*>*	pvecMRT = Find_MRT(pMRTTag);

	if(NULL == pvecMRT)
		return ;

	vector<CRenderTarget*>::iterator	iter = pvecMRT->begin();
	vector<CRenderTarget*>::iterator	iter_end = pvecMRT->end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_DebugBuffer();
	}
}

void Engine::CRenderTarget_Manager::Render_DebugTagetBuffer(const TCHAR* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if(NULL == pRenderTarget)
		return;

	pRenderTarget->Render_DebugBuffer();
}

CRenderTarget* Engine::CRenderTarget_Manager::Find_RenderTarget(const TCHAR* pTargetTag)
{
	CRenderTarget*	pRenderTarget = (CRenderTarget*)m_htTargets.Search_TableData(pTargetTag);

	return pRenderTarget;
}

vector<CRenderTarget*>* Engine::CRenderTarget_Manager::Find_MRT(const TCHAR* pMRTTag)
{
	vector<CRenderTarget*>* pvecMRT = (vector<CRenderTarget*>*)m_htMRT.Search_TableData(pMRTTag);

	return pvecMRT;
}

void Engine::CRenderTarget_Manager::Free(void)
{
	_uint iTableSize =	m_htTargets.Get_TableSize();
	for(_uint i = 0; i < iTableSize; ++i)
	{
		CRenderTarget*		pRenderTarget = (CRenderTarget*)m_htTargets.Erase_Slot(i);
		if(NULL != pRenderTarget)
			Safe_Release(pRenderTarget);
	}
	m_htTargets.Erase_Table();

	iTableSize =	m_htMRT.Get_TableSize();
	for(_uint i = 0; i < iTableSize; ++i)
	{
		vector<CRenderTarget*>*		pvecMRT = (vector<CRenderTarget*>*)m_htMRT.Erase_Slot(i);
		if(NULL != pvecMRT)
			Safe_Delete(pvecMRT);
	}
	m_htMRT.Erase_Table();
}