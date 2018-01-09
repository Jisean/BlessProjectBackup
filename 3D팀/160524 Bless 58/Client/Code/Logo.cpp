#include "stdafx.h"
#include "Logo.h"

#include "Layer.h"
#include "LogoBack.h"
#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"
#include "Loading.h"
#include "SecondThread.h"
#include "SoundMgr.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pLoading(NULL)
, m_pSecondThread(NULL)
{
	
}

CLogo::~CLogo(void)
{
}

HRESULT CLogo::InitScene(void)
{
	HRESULT		hr = NULL;

	CSoundMgr::GetInstance()->PlayBgm(L"Loading.ogg");

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_REMAIN
		, Engine::TEXTURE_NORMAL
		, L"Texture_Logo"
		, L"../bin/Resources/Texture/LogoBack/LogoBack%d.jpg", 5);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_REMAIN
		, Engine::TEXTURE_NORMAL
		, L"Texture_LogoFilter"
		, L"../bin/Resources/Texture/LogoBack/LogoFilter.tga", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pLoading = CLoading::Create(CLoading::LOADING_STAGE1);

	//FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Light Add false");

	Engine::Switch_DebugBuffer(false);
	Engine::Set_DOFRender(false);

	return S_OK;
}

_int CLogo::Update(const _float& fTimeDelta)
{
	if(GetAsyncKeyState(VK_RETURN))
	{
		if(m_pLoading->GetComplete() == true)
		{
			HRESULT hr = Engine::SceneChange(CSceneSelector(CSceneSelector::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, -1, L"¾À ÀüÈ¯ ½ÇÆÐ");
			return 0;
		}
	}

	return Engine::CScene::Update(fTimeDelta);
}

void CLogo::Render(void)
{
#ifdef _DEBUG
	Render_FPS();
#endif
	Engine::Render_Font(L"¹ÙÅÁ", D3DXVECTOR3(10.f, WINCY - 150.f, 0.f), m_pLoading->GetLoadMessage(), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	Engine::Render_Font(L"¸¼Àº°íµñS", D3DXVECTOR3(10.f, WINCY - 100.f, 0.f), m_pLoading->GetLoadPath(), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pLogo = new CLogo(pDevice);
	if(FAILED(pLogo->InitScene()))
		Engine::Safe_Release(pLogo);

	return pLogo;
}

HRESULT CLogo::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CLogo::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CLogoBack::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CLogoBack »ý¼º ½ÇÆÐ");
	pLayer->AddObject(L"LogoBack" , pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

void CLogo::Free(void)
{
	CSoundMgr::GetInstance()->StopBgm();
	Engine::Clear_RenderGroup();
	Engine::Safe_Release(m_pLoading);
	Engine::CLightMgr::GetInstance()->DestroyInstance();
	Engine::CScene::Free();
}

HRESULT CLogo::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(0.f, 0.f, 1.f);

	HRESULT		hr = NULL;
	hr = Engine::Add_Light(m_pDevice, &LightInfo, L"Logo");
	FAILED_CHECK(hr);

	return S_OK;
}
