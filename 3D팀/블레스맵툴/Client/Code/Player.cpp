#include "stdafx.h"
#include "Player.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_pMesh(NULL)
, m_dwIndex(0)
, m_bPush(false)
, m_iAniIdx(28)
, m_pShader(NULL)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.01f, 0.01f, 0.01f);

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Power = 20.f;

	m_iAniIdx = 55;
	return S_OK;
}

_int CPlayer::Update(const _float& fTimeDelta)
{
	KeyCheck();

	if(m_pMesh->GetPeriod() - 0.2 <= m_pMesh->GetTrackPosition())
	{
		m_iAniIdx = 55;
		m_pMesh->SetAnimationSet(m_iAniIdx);
		m_pMesh->SetTrackPos(0.0);
	}

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	_int iResult = Engine::CGameObject::Update(Engine::Get_Time());
	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return iResult;
}

void CPlayer::Render(void)
{
	m_pMesh->FrameMove(Engine::Get_Time());

	LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	Set_ContantTable(pEffect); 

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
	
	m_pMesh->RenderMesh_ForShader(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Sylva");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Mesh");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

void CPlayer::KeyCheck(void)
{
	float		fTime = Engine::Get_Time();

	if(Engine::GetDIKeyState(DIK_UP) & 0x80)
	{
		m_pInfo->m_vPos += m_pInfo->m_vDir * 5.f * fTime;
		//m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos, &(m_pInfo->m_vDir * 5.f * fTime), m_dwIndex);
	}

	if(Engine::GetDIKeyState(DIK_DOWN) & 0x80)
	{
		m_pInfo->m_vPos -= m_pInfo->m_vDir * 5.f * fTime;
		//m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos, &(m_pInfo->m_vDir * -5.f * fTime), m_dwIndex);
	}

	if(Engine::GetDIKeyState(DIK_LEFT) & 0x80)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f * fTime);
	}

	if(Engine::GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f * fTime);
	}

	if(Engine::GetDIKeyState(DIK_SPACE) & 0x80)
	{
		if(m_bPush == true)
			return;

		m_bPush = true;

		m_iAniIdx = 10;
		m_pMesh->SetAnimationSet(m_iAniIdx);
	}
	else
		m_bPush = false;
}

void CPlayer::Set_ContantTable(LPD3DXEFFECT	pEffect)
{
	pEffect->AddRef();
	
	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

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


	Engine::Safe_Release(pEffect);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer*		pGameObject = new CPlayer(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CPlayer::Free(void)
{
	Engine::CGameObject::Free();

}