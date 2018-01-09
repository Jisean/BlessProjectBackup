#include "stdafx.h"
#include "MapObjects.h"
#include "Export_Function.h"
#include "KeyMgr.h"
#include "Stage.h"
#include "Transform.h"
#include "StaticMesh.h"

CMapObjects::CMapObjects(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_pMesh(NULL)
, m_pShader(NULL)
, m_pCalculatorCom(NULL)
, m_pMtrlCom(NULL)
, m_pPlayerInfo(NULL)
{
	
}

CMapObjects::~CMapObjects(void)
{

}

_int CMapObjects::Update(const _float& fTimeDelta)
{
	D3DXVECTOR3 vMin, vMax;
	m_pMesh->GetMinMax(&vMin, &vMax);
	D3DXVec3TransformCoord(&vMin, &vMin, &m_pInfo->m_matWorld);
	D3DXVec3TransformCoord(&vMax, &vMax, &m_pInfo->m_matWorld);

	Culling_Frustum_Object(vMin, vMax);

	m_pMesh->Set_BoxRender(g_bBoundingBoxRender);

	return 0;
}

void CMapObjects::Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix/* = NULL*/)
{
	if(m_eShadowRender == Engine::CGameObject::SHADOW_NONE_S)
		pEffect->SetTechnique("Default");
	else if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
		pEffect->SetTechnique("S_Draw_Shadow");
	else if(m_eShadowRender == Engine::CGameObject::SHADOW_OBJECT)
		pEffect->SetTechnique("S_Draw_Default");
	

	// Static
	_mymat	matLightView, matLightProj;
	D3DXMatrixLookAtLH(&matLightView, &SSHADOWEYE, &SSHADOWAT, &D3DXVECTOR3(0,1,0));
	D3DXMatrixOrthoLH(&matLightProj, 250.f, 250.f, 1.f, 1000.f);

	_mymat matLightWVP;
	matLightWVP = (*pWorldMatrix) * matLightView * matLightProj;

	pEffect->SetMatrix("g_matSLightWVP", &matLightWVP);
	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Static", "g_SShadowTexture");

	// Dynamic
	Engine::CTransform* pPlayerInfo = (Engine::CTransform*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");

	_myvec vDLightDir = SSHADOWEYE - SSHADOWAT;
	D3DXVec3Normalize(&vDLightDir, &vDLightDir);
	_myvec vDLightPos = pPlayerInfo->m_vPos + (vDLightDir * 10.f);

	_mymat	matDLightView, matDLightProj;
	D3DXMatrixLookAtLH(&matDLightView, &vDLightPos, &pPlayerInfo->m_vPos, &D3DXVECTOR3(0,1,0));
	D3DXMatrixOrthoLH(&matDLightProj, 3.f, 3.f, 1.f, 1000.f);

	_mymat matDLightWVP;
	matDLightWVP = (*pWorldMatrix) * matDLightView * matDLightProj;

	pEffect->SetMatrix("g_matDLightWVP", &matDLightWVP);
	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Dynamic", "g_DShadowTexture");
}
