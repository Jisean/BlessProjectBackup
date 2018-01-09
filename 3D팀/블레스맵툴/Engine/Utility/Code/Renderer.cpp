#include "Renderer.h"
#include "Shader_Manager.h"
#include "RenderTarget_Manager.h"

#include "Scene.h"
#include "Export_Function.h"
#include "GameObject.h"

bool Compare_ViewZ(Engine::CGameObject* pSour, Engine::CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_fTime(0.f)
, m_dwCount(0)
, m_pVB(NULL)
, m_pIB(NULL)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
	m_pDevice->AddRef();
}

Engine::CRenderer::~CRenderer(void)
{
	
}

void Engine::CRenderer::SetCurrentScene(CScene* pScene) {m_pScene = pScene;}

HRESULT Engine::CRenderer::Ready_Renderer(void)
{
	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Default", L"../../Reference/Headers/Shader_Default.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Terrain", L"../../Reference/Headers/Shader_Terrain.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Mesh", L"../../Reference/Headers/Shader_Mesh.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Cube", L"../../Reference/Headers/Shader_Cube.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Light", L"../../Reference/Headers/Shader_Light.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Blend", L"../../Reference/Headers/Shader_Blend.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Effect", L"../../Reference/Headers/Shader_Effect.hpp")))
		return E_FAIL;

	D3DVIEWPORT9			ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, VTXFVF_SCREEN, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXSCREEN*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPos = D3DXVECTOR4((float)ViewPort.Width, 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPos = D3DXVECTOR4((float)ViewPort.Width, (float)ViewPort.Height, 0.f, 1.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPos = D3DXVECTOR4(0.f, (float)ViewPort.Height, 0.f, 1.f);
	pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);	

	m_pVB->Unlock();


	if(FAILED(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
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


	// For.RenderTarget
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_RenderTarget(m_pDevice, L"Target_Albedo", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_RenderTarget(m_pDevice, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_RenderTarget(m_pDevice, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_RenderTarget(m_pDevice, L"Target_Light", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_RenderTarget(m_pDevice, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;


#ifdef _DEBUG
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 0.f, 0.f)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Normal", 0.f, 0.f, 0.f, 0.f)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Depth", 0.f, 0.f, 0.f, 0.f)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Light", 0.f, 0.f, 0.f, 0.f)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Specular", 0.f, 0.f, 0.f, 0.f)))
		return E_FAIL;
#endif

	// For.MRT : Deferred
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred", L"Target_Albedo")))
		return E_FAIL;
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;

	// For.MRT : LightAcc
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_MRT(L"MRT_LightAcc", L"Target_Light")))
		return E_FAIL;
	if(FAILED(Engine::CRenderTarget_Manager::GetInstance()->Ready_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;

	return S_OK;
}

void Engine::CRenderer::Render(void)
{
	m_pDevice->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();

	Render_Priority();
	Render_NoneAlpha();

	Render_LightAcc();
	Render_Blend();

	Render_Alpha();
	Render_UI();

	if(m_pScene != NULL)
		m_pScene->Render();

	CRenderTarget_Manager::GetInstance()->Render_DebugBuffer(L"MRT_Deferred");
	CRenderTarget_Manager::GetInstance()->Render_DebugBuffer(L"MRT_LightAcc");

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);

	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < TYPE_END; ++i)
	{
		m_RenderGroup[i].clear();
	}
}

void Engine::CRenderer::Render_Priority(void)
{

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	RENDERLIST::iterator	iter = m_RenderGroup[TYPE_PRIORITY].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[TYPE_PRIORITY].end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}


	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::Render_NoneAlpha(void)
{
	Engine::CRenderTarget_Manager::GetInstance()->Begin_MRT(L"MRT_Deferred");

	RENDERLIST::iterator	iter = m_RenderGroup[TYPE_NONEALPHA].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[TYPE_NONEALPHA].end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	Engine::CRenderTarget_Manager::GetInstance()->End_MRT(L"MRT_Deferred");
}

void Engine::CRenderer::Render_LightAcc(void)
{
	LPD3DXEFFECT pEffect = CShader_Manager::GetInstance()->Get_EffectHandle(L"Shader_Light");
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Target_Normal", "g_NormalTexture");	
	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Target_Depth", "g_DepthTexture");	

	pEffect->Begin(NULL, 0);

	Engine::CRenderTarget_Manager::GetInstance()->Begin_MRT(L"MRT_LightAcc");

	CLightMgr::GetInstance()->Render_Light(pEffect);

	Engine::CRenderTarget_Manager::GetInstance()->End_MRT(L"MRT_LightAcc");

	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void Engine::CRenderer::Render_Blend(void)
{
	LPD3DXEFFECT pEffect = CShader_Manager::GetInstance()->Get_EffectHandle(L"Shader_Blend");
	if(NULL == pEffect)
		return;

	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Target_Albedo", "g_AlbedoTexture");	
	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Target_Light", "g_LightTexture");	
	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Target_Specular", "g_SpecularTexture");	

	pEffect->AddRef();

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pDevice->SetFVF(VTXFVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}



void Engine::CRenderer::Render_Alpha(void)
{
	m_RenderGroup[TYPE_ALPHA].sort(Compare_ViewZ);

	RENDERLIST::iterator	iter = m_RenderGroup[TYPE_ALPHA].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[TYPE_ALPHA].end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void Engine::CRenderer::Render_UI(void)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	RENDERLIST::iterator	iter = m_RenderGroup[TYPE_UI].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[TYPE_UI].end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}



void Engine::CRenderer::AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if(pGameObject == NULL)
		return;

	m_RenderGroup[eRenderGroup].push_back(pGameObject);
}



Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer* pRenderer = new CRenderer(pDevice);
	if(FAILED(pRenderer->Ready_Renderer()))
	{
		Engine::Safe_Release(pRenderer);
	}
	return pRenderer;
}

void Engine::CRenderer::Free(void)
{
	for(int i = 0; i < TYPE_END; ++i)
		m_RenderGroup[i].clear();

	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
	
	Engine::Safe_Release(m_pDevice);
}
