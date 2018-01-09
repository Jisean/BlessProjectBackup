#include "RenderTarget.h"

USING(Engine)

Engine::CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pVB(NULL)
, m_pIB(NULL)
{
	m_pGraphicDev->AddRef();
}

Engine::CRenderTarget::~CRenderTarget(void)
{

}

void Engine::CRenderTarget::SetUp_TargetOnShader(LPD3DXEFFECT pEffect, const char* pConstantName)
{
	pEffect->SetTexture(pConstantName, m_pTargetTexture);

}

HRESULT Engine::CRenderTarget::Ready_RenderTarget(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color)
{	
	if(FAILED(m_pGraphicDev->CreateTexture(iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &m_pTargetTexture, NULL)))
		return E_FAIL;

	m_ClearColor = Color;

	m_pTargetTexture->GetSurfaceLevel(0, &m_pSurface);
	Engine::Safe_Release(m_pTargetTexture);	

	return S_OK;
}

void Engine::CRenderTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	m_iIndex = iIndex;

	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldSurface);

	m_pGraphicDev->SetRenderTarget(iIndex, m_pSurface);
}

void Engine::CRenderTarget::Release_OnGraphicDev(void)
{
	m_pGraphicDev->SetRenderTarget(m_iIndex, m_pOldSurface);

	Engine::Safe_Release(m_pOldSurface);
}

void Engine::CRenderTarget::Clear_RenderTarget(void)
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldSurface);

	m_pGraphicDev->SetRenderTarget(0, m_pSurface);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphicDev->SetRenderTarget(0, m_pOldSurface);

	Engine::Safe_Release(m_pOldSurface);
}

HRESULT Engine::CRenderTarget::Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, VTXFVF_SCREEN, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXSCREEN*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR4(fX, fY, 0.f, 1.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPos = D3DXVECTOR4(fX + fSizeX, fY, 0.f, 1.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPos = D3DXVECTOR4(fX + fSizeX, fY + fSizeY, 0.f, 1.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPos = D3DXVECTOR4(fX, fY + fSizeY, 0.f, 1.f);
	pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);	

	m_pVB->Unlock();


	if(FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CRenderTarget::Render_DebugBuffer(void)
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(VTXFVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

CRenderTarget* Engine::CRenderTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget*		pInstance = new CRenderTarget(pGraphicDev);
	if(FAILED(pInstance->Ready_RenderTarget(iSizeX, iSizeY, Format, Color)))
	{
		MSG_BOX(L"CRenderTarget Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CRenderTarget::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pTargetTexture);
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
}

