#include "stdafx.h"
#include "Terrain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_pTerrainVtx(NULL)
, m_pIndex(NULL)
, m_dwTriCnt(0)
{
	ZeroMemory(&m_tMtrl, sizeof(D3DMATERIAL9));
}

CTerrain::~CTerrain(void)
{
	
}

HRESULT CTerrain::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Power = 20.f;

	m_pTerrainVtx = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];
	m_pBuffer->GetVtxInfo(m_pTerrainVtx);

	D3DXCreateTexture(m_pDevice, 129, 129, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pMyFilterTex);

	D3DLOCKED_RECT		RectInfo;

	m_pMyFilterTex->LockRect(0, &RectInfo, NULL, 0);

	for (_ulong i = 0; i < 129 * 129; ++i)
	{
		((_ulong*)RectInfo.pBits)[i] = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	}	

	m_pMyFilterTex->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Bin/Test.jpg", D3DXIFF_JPG, m_pMyFilterTex, NULL);

	
	return S_OK;
}

_int CTerrain::Update(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	
	return Engine::CGameObject::Update(Engine::Get_Time());
}

void CTerrain::Render(void)
{	
	
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture");
	m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture2",1);
	m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture3",2);
	m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture");
	m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture", 1);
	m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture2",2);
	m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture3",3);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&m_tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&m_tMtrl.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&m_tMtrl.Specular);

	_matrix			matViewInv;

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matViewInv.m[3][0]);	
	pEffect->SetFloat("g_fPower", m_tMtrl.Power);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pBuffer->Render(&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Terrain");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Filter");
	m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"FilterTexture", pComponent));


	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain*		pGameObject = new CTerrain(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Delete_Array(m_pTerrainVtx);
	Engine::Safe_Delete_Array(m_pIndex);	
}