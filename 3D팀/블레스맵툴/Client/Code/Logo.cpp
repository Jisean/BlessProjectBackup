#include "stdafx.h"
#include "Logo.h"

#include "Layer.h"
#include "LogoBack.h"
#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"
#include "Loading.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pLoading(NULL)
{

}

CLogo::~CLogo(void)
{
}

HRESULT CLogo::InitScene(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_LOGO
		, Engine::TEXTURE_NORMAL
		, L"Texture_Logo"
		, L"../bin/Resources/Texture/LogoBack.jpg", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pLoading = CLoading::Create(CLoading::LOADING_STAGE);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Light Add false");

	return S_OK;
}

_int CLogo::Update(const _float& fTimeDelta)
{
	if(GetAsyncKeyState(VK_RETURN))
	{
		if(m_pLoading->GetComplete() == true)
		{
			Engine::SceneChange(CSceneSelector(CSceneSelector::SCENE_STAGE));
			return 0;
		}
	}

	return Engine::CScene::Update(fTimeDelta);
}

void CLogo::Render(void)
{
	Engine::CScene::Render();

	Engine::Render_Font(L"����", D3DXVECTOR3(50.f, 500.f, 0.f), m_pLoading->GetLoadMessage(), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	/*Engine::Get_FontMgr()->Render(L"����", D3DXVECTOR3(10, 20, 0.f), L"JusinGameAcademy", D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Get_FontMgr()->Render(L"����", D3DXVECTOR3(30, 50, 0.f), L"JusinGameAcademy", D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	Engine::Get_FontMgr()->Render(L"�ü�", D3DXVECTOR3(50, 80, 0.f), L"JusinGameAcademy", D3DXCOLOR(0.f, 1.f, 1.f, 1.f));*/
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
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CLogoBack ���� ����");
	pLayer->AddObject(L"LogoBack" , pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

void CLogo::Free(void)
{
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
	hr = Engine::Add_Light(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);

	return S_OK;
}
