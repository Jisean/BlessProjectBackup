#include "Management.h"

#include "Renderer.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
: m_pScene(NULL)
, m_pRenderer(NULL)
, m_pDevice(NULL)
{

}

Engine::CManagement::~CManagement(void)
{

}

const Engine::CComponent* Engine::CManagement::GetComponent(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	return m_pScene->GetComponent(LayerID, pObjKey, pComponentKey);
}

void Engine::CManagement::AddRenderGroup(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	m_pRenderer->AddRenderGroup(eRenderGroup, pGameObject);
}



HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pDevice->AddRef();

	m_pRenderer = CRenderer::Create(pDevice);
	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Create Failed");

	return S_OK;
}

_int Engine::CManagement::Update(const _float& fTimeDelta)
{
	_int		iResult = 0;

	if(m_pScene != NULL)
		iResult = m_pScene->Update(fTimeDelta);

	return iResult;
}

void Engine::CManagement::Render(void)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render();
}

Engine::CScene* Engine::CManagement::Get_Scene(void)
{
	return m_pScene;
}

void Engine::CManagement::Clear_RenderGroup(void)
{
	if(NULL != m_pRenderer)
		m_pRenderer->Clear_RenderGroup();
}

void Engine::CManagement::Free(void)
{
	Engine::Safe_Release(m_pRenderer);  
	Engine::Safe_Release(m_pScene);
	Engine::Safe_Release(m_pDevice);
}


