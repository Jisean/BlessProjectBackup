#include "stdafx.h"
#include "Option.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Stage.h"
#include "Texture.h"
#include "DynamicCamera.h"
#include "KeyMgr.h"

COption::COption(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fMinBloomRange(2.f)
, m_fMinBloomPower(1.f)
, m_fMinLightPower(0.4f)

{

}

COption::~COption(void)
{

}

HRESULT COption::Initialize(void)
{
	for(int i = 0; i < OPTION_END; ++i)
		m_pTexture[i] = NULL;

	for(int i = 0; i < GROUP_END; ++i)
		m_bCheckGroup[i] = false;

	for(int i = 0; i < S_END; ++i)
		m_bCheckSlider[i] = false;

	m_fOldBloomRange = 0.45448104f;
	m_fOldBloomPower = 0.97306466f;
	m_fOldLightPower = 2.f;

	m_fMaxBloomRange = m_fOldBloomRange - (m_fMinBloomRange - m_fOldBloomRange);
	m_fMaxBloomPower = 5.f;
	m_fMaxLightPower = m_fOldLightPower + (m_fOldLightPower - m_fMinLightPower);
	
	Check_Group();
	Update_Option();
	
	m_eTexNum = SLIDE_RANGE;

	FAILED_CHECK(AddComponent());

	Set_TextureInit(OPTION_BORDER, WINCX / 2.f, WINCY / 2.f, WINCX / 4.f, WINCY / 2.8f);

	// txt
	Set_TextureInit(TXT_OPTION, WINCX / 3.08f, WINCY / 5.9f, WINCX / 16.4f, WINCY / 70.6001f);
	Set_TextureInit(TXT_LIGHT, WINCX / 2.94f, WINCY / 4.12f, WINCX / 30.0f, WINCY / 54.8002f);
	
	Set_TextureInit(TXT_HDR, WINCX / 2.98f, WINCY / 3.08f, WINCX / 35.2f, WINCY / 70.6001f);
	Set_TextureInit(TXT_BRIGHTNESS, WINCX / 2.84f, WINCY / 2.66f, WINCX / 14.7999f, WINCY / 61.2002f);
	Set_TextureInit(TXT_BLOOM, WINCX / 3.08f, WINCY / 2.36f, WINCX / 24.9999f, WINCY / 89.5998f);
	Set_TextureInit(TXT_RANGE, WINCX / 2.88f, WINCY / 2.14f, WINCX / 24.9999f, WINCY / 63.2002f);
	Set_TextureInit(TXT_POWER, WINCX / 2.88f, WINCY / 1.98f, WINCX / 24.9999f, WINCY / 75.2000f);
	
	Set_TextureInit(TXT_DOF, WINCX / 2.98f, WINCY / 1.68f, WINCX / 36.2000f, WINCY / 68.4001f);
	Set_TextureInit(TXT_SHADOW, WINCX / 2.84f, WINCY / 1.48f, WINCX / 21.9999f, WINCY / 72.6000f);
	Set_TextureInit(TXT_VOLUMETRIC, WINCX / 2.48f, WINCY / 1.32f, WINCX / 10.5999f, WINCY / 57.0001f);

	Set_TextureInit(TXT_CANCEL, WINCX / 2.72f, WINCY / 1.21f, WINCX / 27.5999f, WINCY / 89.1997f);
	Set_TextureInit(TXT_ACCEPT, WINCX / 1.64f, WINCY / 1.205f, WINCX / 29.3999f, WINCY / 68.8000f);

	// check
	Set_TextureInit(CHECK_LIGHT, WINCX / 3.56f, WINCY / 4.18f, WINCX / 39.8f, WINCY / 39.0001f);
	Set_TextureInit(CHECK_HDR, WINCX / 3.56f, WINCY / 3.1f, WINCX / 39.8f, WINCY / 39.0001f);
	Set_TextureInit(CHECK_DOF, WINCX / 3.56f, WINCY / 1.68f, WINCX / 39.8f, WINCY / 39.0001f);
	Set_TextureInit(CHECK_SHADOW, WINCX / 3.56f, WINCY / 1.48f, WINCX / 39.8f, WINCY / 39.0001f);
	Set_TextureInit(CHECK_VOLUMETRIC, WINCX / 3.56f, WINCY / 1.33f, WINCX / 39.8f, WINCY / 39.0001f);


	// Bar
	Set_TextureInit(BAR_BRIGHTNESS, WINCX / 1.70f, WINCY / 2.66f, WINCX / 6.79991f, WINCY / 52.80020f);
	Set_TextureInit(BAR_RANGE, WINCX / 1.70f, WINCY / 2.14f, WINCX / 6.79991f, WINCY / 52.80020f);
	Set_TextureInit(BAR_POWER, WINCX / 1.70f, WINCY / 1.98f, WINCX / 6.79991f, WINCY / 52.80020f);

	Set_TextureInit(OPTION_BOTTOM, WINCX / 2.00f, WINCY / 1.21f, WINCX / 4.19991f, WINCY / 52.80020f);

	// SLIDER
	Set_TextureInit(SLIDE_BRIGHTNESS, WINCX / 1.69f, WINCY / 2.66f, WINCX / 81.7998f, WINCY / 50.80020f);
	Set_TextureInit(SLIDE_RANGE, WINCX / 1.69f, WINCY / 2.14f, WINCX / 81.7998f, WINCY / 50.80020f);
	Set_TextureInit(SLIDE_POWER, WINCX / 2.14f, WINCY / 1.98f, WINCX / 81.7998f, WINCY / 50.80020f);


	// Rect
	MakeRect(m_rcCheckBox[GROUP_LIGHT], CHECK_LIGHT);
	MakeRect(m_rcCheckBox[GROUP_HDR], CHECK_HDR);
	MakeRect(m_rcCheckBox[GROUP_DOF], CHECK_DOF);
	MakeRect(m_rcCheckBox[GROUP_SHADOW], CHECK_SHADOW);
	MakeRect(m_rcCheckBox[GROUP_VOLUMETRIC], CHECK_VOLUMETRIC);

	MakeRect(m_rcSliderBox[S_BRIGHTNESS], SLIDE_BRIGHTNESS, 20);
	MakeRect(m_rcSliderBox[S_POWER], SLIDE_POWER, 20);
	MakeRect(m_rcSliderBox[S_RANGE], SLIDE_RANGE, 20);

	MakeRect(m_rcCancelBox, TXT_CANCEL);
	MakeRect(m_rcAcceptBox, TXT_ACCEPT);
	
	Init_MoveTex(m_eTexNum);

	return S_OK;
}

_int COption::Update(const _float& fTimeDelta)
{
	if(m_bStart)
	{
		Check_Group();
		Update_Option();
		m_bStart = false;
	}

	if(g_bMainQuest)
		return 0;

	if(!g_bOption)
		return 0;

	if(g_bMainQuest)
		return 0;
	//Move_Tex();
	//Set_TextureInit(m_eTexNum, WINCX / m_MoveX, WINCY / m_MoveY, WINCX / m_MoveSizeX, WINCY / m_MoveSizeY);

	Check_Key();
	Check_Slider();
	
	MakeRect(m_rcSliderBox[S_BRIGHTNESS], SLIDE_BRIGHTNESS, 20);
	MakeRect(m_rcSliderBox[S_POWER], SLIDE_POWER, 20);
	MakeRect(m_rcSliderBox[S_RANGE], SLIDE_RANGE, 20);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	Set_TexView();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void COption::Render(void)
{
	if(g_bOption == false)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	for(int i = 0; i < OPTION_END; ++i)
	{
		if(m_pTexture[i] == NULL)
			continue;

		pEffect->SetTechnique("Default_Device");
		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
		pEffect->SetMatrix("g_matView",&m_matView[i]);
		pEffect->SetMatrix("g_matProj",&m_matProj);

		if(i == OPTION_BORDER)
			pEffect->SetFloat("g_fAlpha", 0.6f);
		else
			pEffect->SetFloat("g_fAlpha", 1.f);

		pEffect->SetFloat("g_fRGB", 1.f);
		pEffect->SetFloat("g_fPointPower", 1.f);

		m_pTexture[i]->Set_ConstantTable(pEffect,"g_BaseTexture"/*, iNum*/);

		int iNum = 0;
		if(m_bCheck[i])
		{
			iNum = 0;

			if(i == BAR_BRIGHTNESS || i == BAR_POWER || i == BAR_RANGE)
				pEffect->SetFloat("g_fRGB", 1.f);
		}
		else
		{
			iNum = 1;

			if(i == BAR_BRIGHTNESS || i == BAR_POWER || i == BAR_RANGE)
			{
				pEffect->SetFloat("g_fRGB", 0.5f);
				iNum = 0;
			}
			else if(i == SLIDE_BRIGHTNESS || i == SLIDE_POWER || i == SLIDE_RANGE)
			{
				Engine::Safe_Release(pEffect);
				continue;
			}
			// 텍스쳐 어둡게 하기
			else if(i >= TXT_OPTION && i <= TXT_ACCEPT)
				pEffect->SetFloat("g_fRGB", 0.7f);
			else if(i >= CHECK_LIGHT && i <= CHECK_VOLUMETRIC)
				m_pCheckXTex->Set_ConstantTable(pEffect,"g_BaseTexture"/*, iNum*/);

		}
			

		//m_pTexture[i]->Set_ConstantTable(pEffect,"g_BaseTexture"/*, iNum*/);

		pEffect->Begin(NULL,0);
		pEffect->BeginPass(0);

		m_pBuffer->Render(&m_pInfo->m_matWorld);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);

	}

	pEffect->SetFloat("g_fRGB", 1.f);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void COption::Free(void)
{
	CUI::Free();
}

HRESULT COption::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader",pComponent);




	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Border");
	m_pTexture[OPTION_BORDER] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[OPTION_BORDER], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Border", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Option");
	m_pTexture[TXT_OPTION] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_OPTION], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Option", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Light");
	m_pTexture[TXT_LIGHT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_LIGHT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Light", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_hdr");
	m_pTexture[TXT_HDR] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_HDR], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_hdr", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Brightness");
	m_pTexture[TXT_BRIGHTNESS] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_BRIGHTNESS], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Brightness", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Bloom");
	m_pTexture[TXT_BLOOM] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_BLOOM], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Bloom", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Range");
	m_pTexture[TXT_RANGE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_RANGE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Range", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Power");
	m_pTexture[TXT_POWER] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_POWER], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Power", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_DOF");
	m_pTexture[TXT_DOF] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_DOF], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_DOF", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Shadow");
	m_pTexture[TXT_SHADOW] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_SHADOW], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Shadow", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Volumetric");
	m_pTexture[TXT_VOLUMETRIC] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[TXT_VOLUMETRIC], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_Volumetric", pComponent);

	//pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Cancel");
	//m_pTexture[TXT_CANCEL] = dynamic_cast<Engine::CTexture*>(pComponent);
	//NULL_CHECK_RETURN(m_pTexture[TXT_CANCEL], E_FAIL);
	//m_htComponent.Insert_Table(L"Texture_Option_Cancel", pComponent);

	//pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Accept");
	//m_pTexture[TXT_ACCEPT] = dynamic_cast<Engine::CTexture*>(pComponent);
	//NULL_CHECK_RETURN(m_pTexture[TXT_ACCEPT], E_FAIL);
	//m_htComponent.Insert_Table(L"Texture_Option_Accept", pComponent);




	// check
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Check");
	m_pTexture[CHECK_LIGHT] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CHECK_LIGHT], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_CheckLight", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Check");
	m_pTexture[CHECK_HDR] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CHECK_HDR], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_CheckHDR", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Check");
	m_pTexture[CHECK_DOF] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CHECK_DOF], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_CheckDOF", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Check");
	m_pTexture[CHECK_SHADOW] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CHECK_SHADOW], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_CheckShaodw", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Check");
	m_pTexture[CHECK_VOLUMETRIC] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[CHECK_VOLUMETRIC], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_CheckVolumetric", pComponent);


	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_CheckX");
	m_pCheckXTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pCheckXTex, E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_CheckX", pComponent);

	
	

	// Bar
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Bar");
	m_pTexture[BAR_BRIGHTNESS] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_BRIGHTNESS], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_BarBRIGHTNESS", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Bar");
	m_pTexture[BAR_RANGE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_RANGE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_BarRANGE", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Bar");
	m_pTexture[BAR_POWER] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[BAR_POWER], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_BarPower", pComponent);

	//// Button
	//pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Button");
	//m_pTexture[OPTION_BOTTOM] = dynamic_cast<Engine::CTexture*>(pComponent);
	//NULL_CHECK_RETURN(m_pTexture[OPTION_BOTTOM], E_FAIL);
	//m_htComponent.Insert_Table(L"Texture_Option_Button", pComponent);


	// Slider
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Slider");
	m_pTexture[SLIDE_BRIGHTNESS] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[SLIDE_BRIGHTNESS], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_SliderBRIGHTNESS", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Slider");
	m_pTexture[SLIDE_RANGE] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[SLIDE_RANGE], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_SliderRange", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_Option_Slider");
	m_pTexture[SLIDE_POWER] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture[SLIDE_POWER], E_FAIL);
	m_htComponent.Insert_Table(L"Texture_Option_SliderPower", pComponent);
	return S_OK;
}

COption* COption::Create(LPDIRECT3DDEVICE9 pDevice)
{
	COption*		pGameObject = new COption(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void COption::Set_Option(eGroup eGroup, bool bRender)
{
	m_bCheckGroup[eGroup] = bRender;

	Check_Group();
	Update_Option();
}

bool COption::Get_LightOption(void)
{
	return m_bCheck[CHECK_LIGHT];
}

void COption::Adjust_Light(void)
{
	float fMin = WINCX / 2.14f;
	float fMax = WINCX / 1.4f;

	float fPixRange = fMax - fMin;
	float fRange = m_fMaxLightPower - m_fMinLightPower;

	// 픽셀당 올릴 값
	float fValue = fRange / fPixRange;

	// 올려야할 픽셀 값
	float fUpPix = *(Engine::Set_RendererLightPower()) / fValue;

	// 재조정
	m_fX[SLIDE_BRIGHTNESS] = fMin + fUpPix;
}

void COption::Set_TextureInit(eTextureName eTexNum, float fX, float fY, float fSizeX, float fSizeY)
{
	m_fX[eTexNum] = fX;
	m_fY[eTexNum] = fY;
	m_fSizeX[eTexNum] = fSizeX;
	m_fSizeY[eTexNum] = fSizeY;
}

void COption::Set_TexView(void)
{
	for(int i = 0; i < OPTION_END; ++i)
	{
		D3DXMatrixIdentity(&m_matView[i]);	

		m_matView[i]._11 = m_fSizeX[i];
		m_matView[i]._22 = m_fSizeY[i];
		m_matView[i]._33 = 1.f;

		// -0.5	-> -400		,	0.5	-> 400
		m_matView[i]._41 = m_fX[i] - (WINCX >> 1);
		m_matView[i]._42 = -m_fY[i] + (WINCY >> 1);
	}
}

void COption::Check_Key(void)
{
	if(CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		for(int i = 0; i < GROUP_END; ++i)
		{
			if(PtInRect(&m_rcCheckBox[i], ptMouse))
			{
				m_bCheckGroup[i] = !m_bCheckGroup[i];
				Check_Group();
				Update_Option();
			}
		}

		for(int i = 0; i < S_END; ++i)
		{
			if(PtInRect(&m_rcSliderBox[i], ptMouse))
			{
				m_bCheckSlider[i] = true;
				Check_Group();
				//Update_Option();
			}
		}

		/*if(PtInRect(&m_rcCancelBox, ptMouse))
		{
			g_bOption = false;
			g_bPause = false;

			Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
			((CDynamicCamera*)pCamera)->Set_MouseFix(true);
		}

		if(PtInRect(&m_rcAcceptBox, ptMouse))
		{
			g_bOption = false;
			g_bPause = false;

			Engine::CGameObject* pCamera = Engine::Get_ObjList(LAYER_UI, L"DynamicCamera")->front();
			((CDynamicCamera*)pCamera)->Set_MouseFix(true);
		}*/
	}


	if(CKeyMgr::GetInstance()->GetKeyUp(VK_LBUTTON))
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		for(int i = 0; i < S_END; ++i)
		{
			//if(PtInRect(&m_rcSliderBox[i], ptMouse))
				m_bCheckSlider[i] = false;
		}
	}
}



void COption::Check_Group(void)
{
	// Light 그룹
	{
		m_bCheck[CHECK_LIGHT] = m_bCheckGroup[GROUP_LIGHT];
		m_bCheck[TXT_LIGHT] = m_bCheckGroup[GROUP_LIGHT];
	}

	// HDR 그룹
	{
		m_bCheck[CHECK_HDR] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[TXT_HDR] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[TXT_BRIGHTNESS] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[TXT_BLOOM] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[TXT_RANGE] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[TXT_POWER] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[BAR_BRIGHTNESS] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[BAR_POWER] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[BAR_RANGE] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[SLIDE_BRIGHTNESS] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[SLIDE_POWER] = m_bCheckGroup[GROUP_HDR];
		m_bCheck[SLIDE_RANGE] = m_bCheckGroup[GROUP_HDR];
	}

	// DOF 그룹
	{
		m_bCheck[CHECK_DOF] = m_bCheckGroup[GROUP_DOF];
		m_bCheck[TXT_DOF] = m_bCheckGroup[GROUP_DOF];
	}

	// Shadow 그룹
	{
		m_bCheck[CHECK_SHADOW] = m_bCheckGroup[GROUP_SHADOW];
		m_bCheck[TXT_SHADOW] = m_bCheckGroup[GROUP_SHADOW];
	}

	// Volumetric 그룹
	{
		m_bCheck[CHECK_VOLUMETRIC] = m_bCheckGroup[GROUP_VOLUMETRIC];
		m_bCheck[TXT_VOLUMETRIC] = m_bCheckGroup[GROUP_VOLUMETRIC];
	}

}

void COption::Check_Slider(void)
{
	if(!m_bCheckGroup[GROUP_HDR])
		return;

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	float fMin = WINCX / 2.14f;
	float fMax = WINCX / 1.4f;



	if(fMax <= ptMouse.x)
		ptMouse.x = (long)fMax;
	else if(fMin >= ptMouse.x)
		ptMouse.x = (long)fMin;


	float fPixRange = fMax - fMin;


	if(m_bCheckSlider[S_BRIGHTNESS] == true)
	{
		float fRange = m_fMaxLightPower - m_fMinLightPower;

		// 픽셀당 올릴 값
		float fValue = fRange / fPixRange;
		float fTemp = ((float)ptMouse.x - m_fX[SLIDE_BRIGHTNESS]) * fValue;

		if(*(Engine::Set_RendererLightPower()) + fTemp <= 0.f)
			*(Engine::Set_RendererLightPower()) = 0.f;
		else
			*(Engine::Set_RendererLightPower()) += fTemp;

		m_fX[SLIDE_BRIGHTNESS] = (float)ptMouse.x;

	}

	if(m_bCheckSlider[S_POWER] == true)
	{
		float fRange = m_fMaxBloomPower - m_fMinBloomPower;

		// 픽셀당 올릴 값
		float fValue = fRange / fPixRange;
		float fTemp = ((float)ptMouse.x - m_fX[SLIDE_POWER]) * fValue;
		*(Engine::SetRendererBloomPower()) += fTemp;

		m_fX[SLIDE_POWER] = (float)ptMouse.x;
	}

	if(m_bCheckSlider[S_RANGE] == true)
	{
		float fRange = m_fMaxBloomRange - m_fMinBloomRange;

		// 픽셀당 올릴 값
		float fValue = fRange / fPixRange;

		float fTemp = ((float)ptMouse.x - m_fX[SLIDE_RANGE]) * fValue;
		*(Engine::SetRendererBloomRange()) += fTemp;


		m_fX[SLIDE_RANGE] = (float)ptMouse.x;
	}


}

void COption::Update_Option(void)
{
	// Light
	{
		size_t iLightKeyCnt = Engine::Get_LightKeyCnt();
		
		for(UINT i = 0; i < iLightKeyCnt; ++i)
			Engine::Set_LightRender(i, m_bCheck[CHECK_LIGHT]);
	}

	// HDR
	{
		if(!m_bCheckGroup[GROUP_HDR])
		{
			m_fOldLightPower = *(Engine::Set_RendererLightPower());
			m_fOldBloomPower = *(Engine::SetRendererBloomPower());
			m_fOldBloomRange = *(Engine::SetRendererBloomRange());

			*(Engine::Set_RendererLightPower()) = 1.f;
			*(Engine::SetRendererBloomPower()) = 1.f;
			*(Engine::SetRendererBloomRange()) = 2.f;
		}
		else
		{
			*(Engine::Set_RendererLightPower()) = m_fOldLightPower;
			*(Engine::SetRendererBloomPower()) = m_fOldBloomPower;
			*(Engine::SetRendererBloomRange()) = m_fOldBloomRange;
		}
	}

	// DOF
	{
		Engine::Set_DOFRender(m_bCheckGroup[GROUP_DOF]);
	}

	// Shadow
	{
		Engine::Set_ShadowRender(m_bCheckGroup[GROUP_SHADOW]);
	}

	// Volumetric Light
	{
		Engine::Set_VolumetricRender(m_bCheckGroup[GROUP_VOLUMETRIC]);
	}
}

void COption::MakeRect(RECT& rc, eTextureName eTexNum, int iAdd)
{
	rc.left = long(m_fX[eTexNum] - (m_fSizeX[eTexNum] + iAdd) * 0.5f);
	rc.top =  long(m_fY[eTexNum] - (m_fSizeY[eTexNum] + iAdd) * 0.5f);
	rc.right = long(m_fX[eTexNum] + (m_fSizeX[eTexNum] + iAdd) * 0.5f);
	rc.bottom = long(m_fY[eTexNum] + (m_fSizeY[eTexNum] + iAdd) * 0.5f);
}

void COption::Init_MoveTex(eTextureName eTexNum)
{
	m_MoveX = WINCX / m_fX[eTexNum];
	m_MoveY = WINCY / m_fY[eTexNum];
	m_MoveSizeX = WINCX / m_fSizeX[eTexNum];
	m_MoveSizeY = WINCY / m_fSizeY[eTexNum];
}

void COption::Move_Tex(void)
{
	float fTemp2 = 0.02f;

	if(Engine::GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		m_MoveX -= fTemp2;
	}

	if(Engine::GetDIKeyState(DIK_LEFT) & 0x80)
	{
		m_MoveX += fTemp2;
	}

	fTemp2 = 0.01f;
	if(Engine::GetDIKeyState(DIK_UP) & 0x80)
	{
		m_MoveY += fTemp2;
	}

	if(Engine::GetDIKeyState(DIK_DOWN) & 0x80)
	{
		m_MoveY -= fTemp2;
	}


	float fTemp = 0.2f;
	if(Engine::GetDIKeyState(DIK_Z) & 0x80)
	{
		m_MoveSizeX += fTemp;
	}

	if(Engine::GetDIKeyState(DIK_X) & 0x80)
	{
		m_MoveSizeX -= fTemp;
	}

	if(Engine::GetDIKeyState(DIK_C) & 0x80)
	{
		m_MoveSizeY += fTemp;
	}

	if(Engine::GetDIKeyState(DIK_V) & 0x80)
	{
		m_MoveSizeY -= fTemp;
	}

}
