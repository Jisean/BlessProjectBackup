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
, m_bDOF(true)
, m_bRadial(false)
, m_bHDR(true)
, m_bBloom(true)
, m_bShadow(false)
, m_bVolumetric(true)
, m_pRenderTagetMgr(CRenderTarget_Manager::GetInstance())
, m_pShaderMgr(CShader_Manager::GetInstance())
, m_fLightPower(2.f)
, m_fBloomRange(0.8f)
, m_fBloomPower(1.f)
, m_bStart(true)
, m_iStartCnt(0)
, m_bOnOffDebugBuffer(true)
, m_fRadialPower(1.5f)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
	m_pDevice->AddRef();
	m_RenderGroup[TYPE_PRIORITY].reserve(1);
	m_RenderGroup[TYPE_NONEALPHA].reserve(50);
	m_RenderGroup[TYPE_STATIC].reserve(100);
	m_RenderGroup[TYPE_ALPHA].reserve(50);
	m_RenderGroup[TYPE_UI].reserve(50);
}

Engine::CRenderer::~CRenderer(void)
{
	
}

void Engine::CRenderer::SetCurrentScene(CScene* pScene) {m_pScene = pScene;}

HRESULT Engine::CRenderer::Ready_Renderer(void)
{
	Load_Shader();

	Create_Buffer();

	Create_RenderTarget();

#ifdef _DEBUG
	Create_DebugBuffer();
#endif

	Create_MRT();

	return S_OK;
}

HRESULT Engine::CRenderer::Load_Shader(void)
{
	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Default", L"../../Reference/Headers/Shader_Default.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Terrain", L"../../Reference/Headers/Shader_Terrain.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Mesh", L"../../Reference/Headers/Shader_Mesh.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Cube", L"../../Reference/Headers/Shader_Cube.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Light", L"../../Reference/Headers/Shader_Light.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Blend", L"../../Reference/Headers/Shader_Blend.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Effect", L"../../Reference/Headers/Shader_Effect.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_DOF", L"../../Reference/Headers/Shader_DOF.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Radial", L"../../Reference/Headers/Shader_Radial.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Bloom", L"../../Reference/Headers/Shader_Bloom.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_UI", L"../../Reference/Headers/Shader_UI.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Particle", L"../../Reference/Headers/Shader_Particle.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_MeshEffect", L"../../Reference/Headers/Shader_MeshEffect.hpp")))
		return E_FAIL;

	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Water", L"../../Reference/Headers/Shader_Water.hpp")))
		return E_FAIL;
													  
	if(FAILED(m_pShaderMgr->Ready_Shader(m_pDevice, L"Shader_Volumetric", L"../../Reference/Headers/Shader_Volumetric.hpp")))
		return E_FAIL;
	// ------------- 추가 --------------//
	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_Instancing", L"../../Reference/Headers/Shader_Instancing.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_HardwareSkinning", L"../../Reference/Headers/Shader_HardwareSkinning.hpp")))
		return E_FAIL;

	if(FAILED(Engine::CShader_Manager::GetInstance()->Ready_Shader(m_pDevice, L"Shader_VTF", L"../../Reference/Headers/Shader_VTF.hpp")))
		return E_FAIL;	

	return S_OK;
}

HRESULT Engine::CRenderer::Create_Buffer(void)
{
	D3DVIEWPORT9			ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, VTXFVF_SCREEN, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXSCREEN*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPos = D3DXVECTOR4((FLOAT)ViewPort.Width, 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPos = D3DXVECTOR4((FLOAT)ViewPort.Width, (FLOAT)ViewPort.Height, 0.f, 1.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPos = D3DXVECTOR4(0.f, (FLOAT)ViewPort.Height, 0.f, 1.f);
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

	return S_OK;
}

HRESULT Engine::CRenderer::Create_RenderTarget()
{
	D3DVIEWPORT9			ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	// For.RenderTarget
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Target_Albedo", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Target_Light", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Target_Shaodw", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;


	// DefferdBlend
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Target_DefferdBlend", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	// Radial
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Radial_Blur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// DOF
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"DOF_FirstBlur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"DOF_SecondBlur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"DOF_Result", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	// Bloom
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Bloom_Bright", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Bloom_Blur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Bloom_2Blur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	// Shadow
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Shadow_Static" /*,13000, 9750,*/,8100, 6075, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Shadow_Dynamic" /*,13000, 9750,*/,2024, 1536, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;


	// Volumetric
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Volumetric_Sky", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Volumetric_Occlusion", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_RenderTarget(m_pDevice, L"Volumetric_Result", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	return S_OK;
}


HRESULT Engine::CRenderer::Create_DebugBuffer(void)
{
	D3DVIEWPORT9			ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	// Deferrd
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 200.f, 150.f)))
		return E_FAIL;																		   
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Target_Normal", 0.f, 150.f, 200.f, 150.f)))
		return E_FAIL;																		   
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Target_Depth", 0.f, 300.f, 200.f, 150.f)))
		return E_FAIL;																		   
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Target_Specular", 0.f, 450.f, 200.f, 150.f)))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Target_Light", 200.f, 0.f, 200.f, 150.f)))
		return E_FAIL;


	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Target_Shaodw", 200.f, 150.f, 200.f, 150.f)))
		return E_FAIL;

	// PostEffect
	// Bloom
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Bloom_2Blur", ViewPort.Width - 200.f, 0.f, 200.f, 150.f)))
		return E_FAIL;

	// Radial
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Radial_Blur", ViewPort.Width - 200.f, 150.f, 200.f, 150.f)))
		return E_FAIL;

	// Volumetric
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Volumetric_Result", ViewPort.Width - 200.f, 300.f, 200.f, 150.f)))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Volumetric_Occlusion", ViewPort.Width - 400.f, 150.f, 200.f, 150.f)))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Volumetric_Sky", ViewPort.Width - 400.f, 0.f, 200.f, 150.f)))
		return E_FAIL;

	// DOF
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"DOF_Result", ViewPort.Width - 200.f, 450.f, 200.f, 150.f)))
		return E_FAIL;




	// Deferrd Blend
	if(FAILED(m_pRenderTagetMgr->Ready_DebugBuffer(L"Target_DefferdBlend", ViewPort.Width - 200.f, 600.f, 200.f, 150.f)))
		return E_FAIL;
	return S_OK;
}


HRESULT Engine::CRenderer::Create_MRT(void)
{
	// For.MRT : Deferred
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"MRT_Deferred", L"Target_Albedo")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"MRT_Deferred", L"Target_Specular")))
		return E_FAIL;

	// For.MRT : LightAcc
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"MRT_LightAcc", L"Target_Light")))
		return E_FAIL;

	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"MRT_LightAcc", L"Target_Shaodw")))
		return E_FAIL;

	// For.MRT : DefferdBlend
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"MRT_DeferredBlend", L"Target_DefferdBlend")))
		return E_FAIL;

	// For.MRT : DOF
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_DOF_FirstBlur", L"DOF_FirstBlur")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_DOF_SecondBlur", L"DOF_SecondBlur")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_DOF_Result", L"DOF_Result")))
		return E_FAIL;

	// For.MRT : Radial
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Radial_Blur", L"Radial_Blur")))
		return E_FAIL;

	// For.MRT : Bloom
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Bloom_bright", L"Bloom_Bright")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Bloom_Blur", L"Bloom_Blur")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Bloom_Blur2", L"Bloom_2Blur")))
		return E_FAIL;

	// For.MRT : Shadow
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Shadow_Static", L"Shadow_Static")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Shadow_Dynamic", L"Shadow_Dynamic")))
		return E_FAIL;

	// For.MRT : Volumetric
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Volumetric_Sky", L"Volumetric_Sky")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Volumetric_Occ", L"Volumetric_Occlusion")))
		return E_FAIL;
	if(FAILED(m_pRenderTagetMgr->Ready_MRT(L"M_Volumetric_Result", L"Volumetric_Result")))
		return E_FAIL;

	return S_OK;

}

void Engine::CRenderer::Render_DebugBuffer(void)
{
	m_pRenderTagetMgr->Render_DebugBuffer(L"MRT_Deferred");
	m_pRenderTagetMgr->Render_DebugTagetBuffer(L"Target_Light");

	///////////////////////////////////////////////////////////////////////////////
	//if(m_bShadow)
		m_pRenderTagetMgr->Render_DebugTagetBuffer(L"Target_Shaodw");

	if(m_bBloom && m_bHDR)
		m_pRenderTagetMgr->Render_DebugBuffer(L"M_Bloom_Blur2");

	if(m_bRadial)
		m_pRenderTagetMgr->Render_DebugBuffer(L"M_Radial_Blur");

	if(m_bDOF)
		m_pRenderTagetMgr->Render_DebugBuffer(L"M_DOF_Result");

	if(m_bVolumetric)
		m_pRenderTagetMgr->Render_DebugBuffer(L"M_Volumetric_Result");
	///////////////////////////////////////////////////////////////////////////////
	//m_pRenderTagetMgr->Render_DebugBuffer(L"M_Volumetric_Sky");
	//m_pRenderTagetMgr->Render_DebugBuffer(L"M_Volumetric_Occ");
	//m_pRenderTagetMgr->Render_DebugBuffer(L"MRT_DeferredBlend");
}

void Engine::CRenderer::Render(void)
{
	m_pDevice->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
	m_pDevice->BeginScene();

	Render_Priority();

	if(m_bStart)
	{
		if(m_iStartCnt > 1)
		{
			m_bStart = false;
			m_iStartCnt = 0;
		}
		else
		{
			++m_iStartCnt;
			Render_Shadow_Static();
		}
	}
	Render_Shadow_Dynamic();

	Render_Static();
	Render_NoneAlpha();
	Render_LightAcc();
	Render_DeferredBlend();

	Render_Bloom();	
	Render_DOF();
	Render_Radial();
	Render_Volumetric();
	
	Render_Blend();

	Render_Alpha();	
	
	Render_UI();
	

	if(m_pScene != NULL)
		m_pScene->Render();

#ifdef _DEBUG
	if(m_bOnOffDebugBuffer)
		Render_DebugBuffer();
#endif

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

	RENDERVEC::iterator	iter = m_RenderGroup[TYPE_PRIORITY].begin();
	RENDERVEC::iterator	iter_end = m_RenderGroup[TYPE_PRIORITY].end();

	for( iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::Render_Shadow_Static(void)
{
	// Static
	m_pRenderTagetMgr->Begin_MRT(L"M_Shadow_Static");

	RENDERVEC::iterator	iter = m_RenderGroup[TYPE_STATIC].begin();
	RENDERVEC::iterator	iter_end = m_RenderGroup[TYPE_STATIC].end();

	for( iter; iter != iter_end; ++iter)
	{
		(*iter)->Set_ShadowRender(CGameObject::SHADOW_SHADOW);
		(*iter)->Render();
	}

	m_pRenderTagetMgr->End_MRT(L"M_Shadow_Static");
}

void Engine::CRenderer::Render_Shadow_Dynamic(void)
{
	if(!m_bShadow)
		return;

	// Static
	m_pRenderTagetMgr->Begin_MRT(L"M_Shadow_Dynamic");

	RENDERVEC::iterator	iter = m_RenderGroup[TYPE_NONEALPHA].begin();
	RENDERVEC::iterator	iter_end = m_RenderGroup[TYPE_NONEALPHA].end();

	for( iter; iter != iter_end; ++iter)
	{
		(*iter)->Set_ShadowRender(CGameObject::SHADOW_SHADOW);
		(*iter)->Render();
	}

	m_pRenderTagetMgr->End_MRT(L"M_Shadow_Dynamic");
}

void Engine::CRenderer::Render_Static(void)
{
	m_pRenderTagetMgr->Begin_MRT(L"MRT_Deferred");

	RENDERVEC::iterator	iter = m_RenderGroup[TYPE_STATIC].begin();
	RENDERVEC::iterator	iter_end = m_RenderGroup[TYPE_STATIC].end();

	for( iter; iter != iter_end; ++iter)
	{
		if(m_bShadow)
			(*iter)->Set_ShadowRender(CGameObject::SHADOW_OBJECT);
		else
			(*iter)->Set_ShadowRender(CGameObject::SHADOW_NONE_S);

		(*iter)->Render();
	}

	//m_pRenderTagetMgr->End_MRT(L"MRT_Deferred");
}

void Engine::CRenderer::Render_NoneAlpha(void)
{
	//m_pRenderTagetMgr->Begin_MRT(L"MRT_Deferred");

	RENDERVEC::iterator	iter = m_RenderGroup[TYPE_NONEALPHA].begin();
	RENDERVEC::iterator	iter_end = m_RenderGroup[TYPE_NONEALPHA].end();

	for( iter; iter != iter_end; ++iter)
	{
		if(m_bShadow)
			(*iter)->Set_ShadowRender(CGameObject::SHADOW_OBJECT);
		else
			(*iter)->Set_ShadowRender(CGameObject::SHADOW_NONE_D);

		(*iter)->Render();
	}

	m_pRenderTagetMgr->End_MRT(L"MRT_Deferred");
}
void Engine::CRenderer::Render_LightAcc(void)
{
	LPD3DXEFFECT pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_Light");
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_Normal", "g_NormalTexture");	
	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_Depth", "g_DepthTexture");	

	// HDR 밝기 조절
	if(m_bHDR)
		pEffect->SetFloat("g_fLightPower", m_fLightPower);
	else
		pEffect->SetFloat("g_fLightPower", 1.f);
	//////////////////////////////////////////////////
	m_pRenderTagetMgr->Begin_MRT(L"MRT_LightAcc");
	pEffect->Begin(NULL, 0);

	CLightMgr::GetInstance()->Render_Light(pEffect);

	pEffect->End();
	m_pRenderTagetMgr->End_MRT(L"MRT_LightAcc");

	Engine::Safe_Release(pEffect);
}


void Engine::CRenderer::Render_DeferredBlend(void)
{
	// 원본 만들기
	LPD3DXEFFECT pFirstEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_Blend");
	if(NULL == pFirstEffect)
		return;

	pFirstEffect->SetTechnique("Blend_BlendBuffer");

	m_pRenderTagetMgr->SetUp_TargetOnShader(pFirstEffect, L"Target_Albedo", "g_AlbedoTexture");	
	m_pRenderTagetMgr->SetUp_TargetOnShader(pFirstEffect, L"Target_Light", "g_LightTexture");	
	m_pRenderTagetMgr->SetUp_TargetOnShader(pFirstEffect, L"Target_Depth", "g_DepthTexture");
	m_pRenderTagetMgr->SetUp_TargetOnShader(pFirstEffect, L"Target_Specular", "g_SpecularTexture");

	pFirstEffect->AddRef();

	m_pRenderTagetMgr->Begin_MRT(L"MRT_DeferredBlend");
	
	DrawBuffer(pFirstEffect, 1);
		
	m_pRenderTagetMgr->End_MRT(L"MRT_DeferredBlend");

	Engine::Safe_Release(pFirstEffect);
}


void Engine::CRenderer::Render_Bloom(void)
{
	if(!m_bHDR || !m_bBloom)
		return;

	// 밝기 추출
	LPD3DXEFFECT pEffect = CShader_Manager::GetInstance()->Get_EffectHandle(L"Shader_Bloom");
	if(NULL == pEffect)
		return;

	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Target_DefferdBlend", "g_BaseTexture");	

	pEffect->SetFloat("g_fBloomPower", m_fBloomPower);
	pEffect->SetFloat("g_fBloomRange", m_fBloomRange);
	pEffect->SetFloat("g_fLightPower", m_fLightPower);

	pEffect->AddRef();

	m_pRenderTagetMgr->Begin_MRT(L"M_Bloom_bright");

	DrawBuffer(pEffect, 0);

	m_pRenderTagetMgr->End_MRT(L"M_Bloom_bright");

	Engine::Safe_Release(pEffect);


	// 블러 1회
	pEffect = CShader_Manager::GetInstance()->Get_EffectHandle(L"Shader_DOF");
	if(NULL == pEffect)
		return;

	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Bloom_Bright", "g_BlurTexture");	

	pEffect->AddRef();

	m_pRenderTagetMgr->Begin_MRT(L"M_Bloom_Blur");

	DrawBuffer(pEffect, 0);

	m_pRenderTagetMgr->End_MRT(L"M_Bloom_Blur");

	Engine::Safe_Release(pEffect);



	// 블러 2회
	pEffect = CShader_Manager::GetInstance()->Get_EffectHandle(L"Shader_DOF");
	if(NULL == pEffect)
		return;

	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Bloom_Blur", "g_BlurTexture");	

	pEffect->AddRef();

	m_pRenderTagetMgr->Begin_MRT(L"M_Bloom_Blur2");

	DrawBuffer(pEffect, 0);

	m_pRenderTagetMgr->End_MRT(L"M_Bloom_Blur2");

	Engine::Safe_Release(pEffect);
}

void Engine::CRenderer::Render_DOF(void)
{
	if(!m_bDOF)
		return;


	// 블러 1회
	LPD3DXEFFECT pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_DOF");
	if(NULL == pEffect)
		return;

	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_DefferdBlend", "g_BlurTexture");	

	D3DVIEWPORT9			ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	pEffect->AddRef();
	m_pRenderTagetMgr->Begin_MRT(L"M_DOF_FirstBlur");
	DrawBuffer(pEffect, 0);
	m_pRenderTagetMgr->End_MRT(L"M_DOF_FirstBlur");
	Engine::Safe_Release(pEffect);



	// 블러 2회
	pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_DOF");
	if(NULL == pEffect)
		return;

	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"DOF_FirstBlur", "g_BlurTexture");	

	pEffect->AddRef();
	m_pRenderTagetMgr->Begin_MRT(L"M_DOF_SecondBlur");
	DrawBuffer(pEffect, 0);
	m_pRenderTagetMgr->End_MRT(L"M_DOF_SecondBlur");
	Engine::Safe_Release(pEffect);


	// Combine
	pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_DOF");
	if(NULL == pEffect)
		return;

	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_DefferdBlend", "g_OriginTexture");
	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"DOF_FirstBlur", "g_FirstBlurTexture");
	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"DOF_SecondBlur", "g_SecondBlurXTexture");
	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_Depth", "g_DepthTexture");	

	// 200보다 크면 원거리
	pEffect->SetFloat("g_fFar", 999.5f);

	// 1000보다 작으면 근거리
	pEffect->SetFloat("g_fNear", 999.5f);
	pEffect->AddRef();

	if(m_bRadial || m_bVolumetric)
		m_pRenderTagetMgr->Begin_MRT(L"M_DOF_Result");

	DrawBuffer(pEffect, 1);
	
	if(m_bRadial || m_bVolumetric)
		m_pRenderTagetMgr->End_MRT(L"M_DOF_Result");

	Engine::Safe_Release(pEffect);
}


void Engine::CRenderer::Render_Radial(void)
{
	if(!m_bRadial)
		return;

	LPD3DXEFFECT pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_Radial");
	if(NULL == pEffect)
		return;

	if(m_bDOF)
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"DOF_Result", "g_BaseTexture");	
	else
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_DefferdBlend", "g_BaseTexture");	

	// 중점
	pEffect->SetFloat("g_CenterX", 0.5f);
	pEffect->SetFloat("g_CenterY", 0.5f);

	// 블러의 강도
	pEffect->SetFloat("g_BlurStart", 1.f);

	// 중점에 블러 안먹는 정도?
	pEffect->SetFloat("g_BlurWidth", m_fRadialPower);

	pEffect->AddRef();

	if(m_bVolumetric)
		m_pRenderTagetMgr->Begin_MRT(L"M_Radial_Blur");
	
	DrawBuffer(pEffect, 0);
	
	if(m_bVolumetric)
		m_pRenderTagetMgr->End_MRT(L"M_Radial_Blur");

	Engine::Safe_Release(pEffect);

}

void Engine::CRenderer::Render_Volumetric(void)
{
	if(!m_bVolumetric)
		return;

	// 1. 하늘을 흰색으로 걷어내기
	RENDERVEC::iterator	Sky_iter = m_RenderGroup[TYPE_VOLUMETRIC].begin();
	RENDERVEC::iterator	Sky_iter_end = m_RenderGroup[TYPE_VOLUMETRIC].end();

	m_pRenderTagetMgr->Begin_MRT(L"M_Volumetric_Sky");
	//m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_DefferdBlend", "g_BlurTexture");

	for(Sky_iter; Sky_iter != Sky_iter_end; ++Sky_iter)
	{
		(*Sky_iter)->Render();
	}

	m_pRenderTagetMgr->End_MRT(L"M_Volumetric_Sky");



	// 2. 검은색만 볼류메틱 랜더타겟에 그려주기
	// 2.1 태양 그리기 
	RENDERVEC::iterator iter = m_RenderGroup[TYPE_PRIORITY].begin();
	RENDERVEC::iterator iter_end = m_RenderGroup[TYPE_PRIORITY].end();

	m_pRenderTagetMgr->Begin_MRT(L"M_Volumetric_Occ");

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	// 2.2 차폐물 그리기
	LPD3DXEFFECT pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_Volumetric");
	if(NULL == pEffect)
		return;

	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Volumetric_Sky", "g_BaseTexture");	

	pEffect->SetTechnique("Sky");
	pEffect->AddRef();
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pDevice->SetFVF(VTXFVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	m_pRenderTagetMgr->End_MRT(L"M_Volumetric_Occ");



	// 3. 광선 만들기
	pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_Volumetric");
	if(NULL == pEffect)
		return;

	

	if(m_bRadial)
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Radial_Blur", "g_BaseTexture");	
	else if(m_bDOF)
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"DOF_Result", "g_BaseTexture");	
	else
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_DefferdBlend", "g_BaseTexture");	

	m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Volumetric_Occlusion", "g_OccTexture");	


	_mymat matProj, matView;
	_myvec vSunScreenPos;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXVec3Project(&vSunScreenPos, &_vec3(-76.f, 384.f, 3.f), NULL, &matProj, &matView, NULL);

	vSunScreenPos.x = (vSunScreenPos.x + 1.f) * 0.5f;
	vSunScreenPos.y = (1.f - vSunScreenPos.y) * 0.5f;

	pEffect->SetValue("g_ScreenSunPos", vSunScreenPos, sizeof(float) * 2);

	D3DXVECTOR2 vSunDir = D3DXVECTOR2(0.5f, 0.5f);
	vSunDir.x = vSunScreenPos.x - vSunDir.x;
	vSunDir.y = vSunScreenPos.y - vSunDir.y;
	D3DXVec2Normalize(&vSunDir, &vSunDir);
	pEffect->SetValue("g_SunDir", vSunDir, sizeof(float) * 2);

	pEffect->SetFloat("g_Decay", 0.95f);

	pEffect->SetTechnique("Light");
	pEffect->AddRef();

	//m_pRenderTagetMgr->Begin_MRT(L"M_Volumetric_Result");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pDevice->SetFVF(VTXFVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	//m_pRenderTagetMgr->End_MRT(L"M_Volumetric_Result");
}

void Engine::CRenderer::Render_Blend(void)
{
	LPD3DXEFFECT pEffect = m_pShaderMgr->Get_EffectHandle(L"Shader_Blend");
	if(NULL == pEffect)
		return;

	//// DOF
	//if(m_bDOF || m_bRadial || m_bVolumetric)
	//{
	//	pEffect->SetTechnique("Blend_DrawBuffer");

	//	if(m_bVolumetric)
	//		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Volumetric_Result", "g_DOFTexture");
	//	else if(m_bRadial)
	//		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Radial_Blur", "g_DOFTexture");
	//	else if(m_bDOF)
	//		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"DOF_Result", "g_DOFTexture");
	//}
	//else 
		if(!m_bDOF && !m_bRadial && !m_bVolumetric)
	{
		pEffect->SetTechnique("Blend_BlendBuffer");
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_Albedo", "g_AlbedoTexture");	
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_Light", "g_LightTexture");	
		m_pRenderTagetMgr->SetUp_TargetOnShader(pEffect, L"Target_Specular", "g_SpecularTexture");

		pEffect->AddRef();

		DrawBuffer(pEffect, 0);

		Engine::Safe_Release(pEffect);

	}

	// Bloom
	if(!m_bHDR || !m_bBloom)
		return;

	// Bloom 더하기..
	pEffect = CShader_Manager::GetInstance()->Get_EffectHandle(L"Shader_Bloom");
	if(NULL == pEffect)
		return;

	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, L"Bloom_Blur", "g_BaseTexture");

	pEffect->AddRef();
	DrawBuffer(pEffect, 1);
	Engine::Safe_Release(pEffect);
}



void Engine::CRenderer::Render_Alpha(void)
{
	RENDERVEC::iterator	iter = m_RenderGroup[TYPE_ALPHA].begin();
	RENDERVEC::iterator	iter_end = m_RenderGroup[TYPE_ALPHA].end();
	sort(iter, iter_end, Compare_ViewZ);

	for( iter; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void Engine::CRenderer::Render_UI(void)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	RENDERVEC::iterator	iter = m_RenderGroup[TYPE_UI].begin();
	RENDERVEC::iterator	iter_end = m_RenderGroup[TYPE_UI].end();

	for( iter; iter != iter_end; ++iter)
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

void Engine::CRenderer::DrawBuffer(LPD3DXEFFECT pEffect, _uint iPassNum /*= 0*/)
{
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassNum);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pDevice->SetFVF(VTXFVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
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

