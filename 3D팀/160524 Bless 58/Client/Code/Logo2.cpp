#include "stdafx.h"
#include "Logo2.h"

#include "Layer.h"
#include "LogoBack.h"
#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"
#include "Loading.h"
#include "SecondThread.h"
#include "SoundMgr.h"

CLogo2::CLogo2(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pLoading(NULL)
, m_pSecondThread(NULL)
{
	
}

CLogo2::~CLogo2(void)
{
}

HRESULT CLogo2::InitScene(void)
{
	VTXCNTX = 2;
	VTXCNTZ = 2;
	VTXITV = 257;

	CSoundMgr::GetInstance()->PlayBgm(L"Loading.ogg");
	m_pLoading = CLoading::Create(CLoading::LOADING_STAGE2);

	//FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Light Add false");

	Engine::Switch_DebugBuffer(false);
	Engine::Set_DOFRender(false);

	return S_OK;
}

_int CLogo2::Update(const _float& fTimeDelta)
{
	if(GetAsyncKeyState(VK_RETURN))
	{
		if(m_pLoading->GetComplete() == true)
		{
			HRESULT hr = Engine::SceneChange(CSceneSelector(CSceneSelector::SCENE_STAGE2));
			FAILED_CHECK_RETURN_MSG(hr, -1, L"¾À ÀüÈ¯ ½ÇÆÐ");
			return 0;
		}
	}

	return Engine::CScene::Update(fTimeDelta);
}

void CLogo2::Render(void)
{
#ifdef _DEBUG
	Render_FPS();
#endif
	
	Engine::Render_Font(L"¹ÙÅÁ", D3DXVECTOR3(10.f, WINCY - 150.f, 0.f), m_pLoading->GetLoadMessage(), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	Engine::Render_Font(L"¸¼Àº°íµñS", D3DXVECTOR3(10.f, WINCY - 100.f, 0.f), m_pLoading->GetLoadPath(), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
}

CLogo2* CLogo2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo2* pLogo = new CLogo2(pDevice);
	if(FAILED(pLogo->InitScene()))
		Engine::Safe_Release(pLogo);

	return pLogo;
}

HRESULT CLogo2::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CLogo2::Add_GameLogic_Layer(void)
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

void CLogo2::Free(void)
{
	CSoundMgr::GetInstance()->StopBgm();
	Engine::Clear_RenderGroup();
	Engine::Safe_Release(m_pLoading);
	Engine::CLightMgr::GetInstance()->DestroyInstance();
	Engine::CScene::Free();
}

HRESULT CLogo2::Add_Light(void)
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
