#include "stdafx.h"
#include "Water.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Material.h"
#include "KeyMgr.h"

CWater::CWater(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_fWaveTime(0.f)
{
	m_htComponent.Ready_Table(19);
}

CWater::~CWater(void)
{

}

HRESULT CWater::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = _vec3(80.f, 1.2f, 150.f);
	m_pInfo->m_vScale = _vec3(0.9f, 5.f, 0.9f);

	m_fSpecRange = 1.f;
	m_fSpecPower = 1.f;

	return S_OK;

}

_int CWater::Update(const _float& fTimeDelta)
{
	m_fWaveTime += fTimeDelta;

	if(CKeyMgr::GetInstance()->GetStateKey('B'))
		m_fSpecPower += 0.01f;
	if(CKeyMgr::GetInstance()->GetStateKey('N'))
		m_fSpecPower -= 0.01f;

	if(CKeyMgr::GetInstance()->GetStateKey('X'))
		m_fSpecRange += 0.1f;
	if(CKeyMgr::GetInstance()->GetStateKey('C'))
		m_fSpecRange -= 0.1f;

	/*if(CKeyMgr::GetInstance()->GetKeyDown('Z'))
		__debugbreak();*/

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CWater::Render(void)
{
	if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	Set_ConstantTable(pEffect);


	pEffect->SetFloat("g_fSpecMapping", 0.f);
	pEffect->SetFloat("g_fSpecRange", m_fSpecRange);	// 범위 설정 값이 높을 수록 범위가 축소된다.
	pEffect->SetFloat("g_fSpecPow", m_fSpecPower);		// 파워 설정 값이 높을 수록 빛의 세기가 세진다.



	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pBuffer->Render(&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CWater::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Water");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Water");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Water_NRM");
	m_pTextureNRM = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"TextureNRM", pComponent);


	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Water");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);	

	return S_OK;
}

void CWater::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture");
	m_pTextureNRM->Set_ConstantTable(pEffect, "g_NormalTexture");

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightPos", (_vec4*)&pLightInfo->Position);

	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();
	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&pMtrlInfo->Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pMtrlInfo->Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pMtrlInfo->Specular);

	_matrix			matViewInv;

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matViewInv.m[3][0]);	
	pEffect->SetVector("g_vCamPos", (_vec4*)&matViewInv.m[3][0]);	
	pEffect->SetFloat("g_fPower", pMtrlInfo->Power);

	pEffect->SetFloat("g_fTime", m_fWaveTime);
	pEffect->SetVector("wvK", &D3DXVECTOR4( 0.02F, 0.01F, 0.1F, 0.04F));
	pEffect->SetVector("wvSpeed", &D3DXVECTOR4( 4.0F, 3.5F,  1.4F, 2.8F));
	pEffect->SetVector("wvOmega", &D3DXVECTOR4( 0.35F, 0.2F,  0.4F, 0.3F));
}


CWater* CWater::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey /*= NULL*/)
{
	CWater*		pGameObject = new CWater(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CWater::Free(void)
{
	Engine::CGameObject::Free();
}

