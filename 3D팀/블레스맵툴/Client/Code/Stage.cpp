#include "stdafx.h"
#include "Stage.h"

#include "Include.h"
#include "Export_Function.h"
#include "Layer.h"
#include "Terrain.h"
#include "Loading.h"
#include "DynamicCamera.h"
#include "Stone.h"
#include "Player.h"
#include "Bow.h"
#include "SkyBox.h"
#include "UI.h"
#include "Effect.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::InitScene(void)
{
	HRESULT		hr = NULL;
		
	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Light Add false");

	return S_OK;
}

_int CStage::Update(const _float& fTimeDelta)
{
	return Engine::CScene::Update(fTimeDelta);
}

void CStage::Render(void)
{
	Render_FPS();	
}

HRESULT CStage::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CSkyBox::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"SkyBox 积己 角菩");	
	pLayer->AddObject(L"SkyBox" , pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIRONMENT, pLayer));
	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Terrain 积己 角菩");
	pLayer->AddObject(L"Terrain" , pGameObject);

	pGameObject = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Player 积己 角菩");	
	pLayer->AddObject(L"Player" , pGameObject);

	for(int i = 0; i < 20; ++i)
	{
		pGameObject = CStone::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Stone 积己 角菩");	
		pLayer->AddObject(L"Stone" , pGameObject);

		pGameObject = CEffect::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Effect 积己 角菩");	
		pLayer->AddObject(L"Effect" , pGameObject);
	}

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CStage::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CDynamicCamera::Create(m_pDevice, &D3DXVECTOR3(0.f, 5.f, -10.f), &D3DXVECTOR3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"DynamicCamera 积己 角菩");
	pLayer->AddObject(L"DynamicCamera" , pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

HRESULT CStage::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(1.f, -1.f, 1.f);

	HRESULT		hr = NULL;
	hr = Engine::Add_Light(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Position = _vec3(15.f, 3.f, 15.f);
	LightInfo.Range = 7.f;	
	
	hr = Engine::Add_Light(m_pDevice, &LightInfo, 1);
	FAILED_CHECK(hr);

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Position = _vec3(5.f, 3.f, 15.f);
	LightInfo.Range = 7.f;	

	hr = Engine::Add_Light(m_pDevice, &LightInfo, 2);
	FAILED_CHECK(hr);

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pStage = new CStage(pDevice);

	if(FAILED(pStage->InitScene()))
	{
		Engine::Safe_Release(pStage);
	}

	return pStage;
}

void CStage::Free(void)
{
	Engine::CScene::Free();
}