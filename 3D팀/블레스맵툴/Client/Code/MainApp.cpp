#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "SceneSelector.h"
#include "VIBuffer.h"

CMainApp::CMainApp(void)
: m_pDevice(NULL)
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::InitApp(void)
{
	HRESULT		hr = NULL;

	if(FAILED(Engine::Ready_GraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY)))
		return E_FAIL;



	if(FAILED(Engine::Ready_InputDev(g_hInst, g_hWnd)))
		return E_FAIL;

	m_pDevice = Engine::Get_GraphicDev();
	m_pDevice->AddRef();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if(FAILED(Engine::Ready_Management(m_pDevice)))
		return E_FAIL;

	if(FAILED(Engine::Reserve_ContainerSize(RESOURCE_END)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Font(m_pDevice, L"¹ÙÅÁ", 40, 24, FW_HEAVY)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Font(m_pDevice, L"±¼¸²", 20, 12, FW_NORMAL)))	
		return E_FAIL;
	if(FAILED(Engine::Ready_Font(m_pDevice, L"±Ã¼­", 40, 24, FW_HEAVY)))
		return E_FAIL;

	hr = Engine::Add_Buffer(m_pDevice, RESOURCE_STATIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Buffer_RcTex »ý¼º ½ÇÆÐ");

	Engine::Init_Time();

	hr = Engine::SceneChange(CSceneSelector(CSceneSelector::SCENE_LOGO));
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"¾À ÀüÈ¯ ½ÇÆÐ");

	return S_OK;
}

_int CMainApp::Update(const _float& fTimeDelta)
{
	Engine::Set_Time();
	Engine::Update_InputDev();
	return Engine::Update_Management(fTimeDelta);
}

void CMainApp::Render(void)
{	
	Engine::CManagement::GetInstance()->Render();
//	Engine::Render_Font(L"¹ÙÅÁ", _vec3(0.f, 0.f, 0.f), L"Test", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*		pMainApp = new CMainApp;
	if(FAILED(pMainApp->InitApp()))
	{
		MSG_BOX(L"CMainApp Created Failed");
		Engine::Safe_Release(pMainApp);		
	}

	return pMainApp;
}

void CMainApp::Free(void)
{	
	Engine::Safe_Release(m_pDevice);

	
	Engine::Release_Utility();	
	Engine::Release_Resource();
	Engine::Release_System();
}

