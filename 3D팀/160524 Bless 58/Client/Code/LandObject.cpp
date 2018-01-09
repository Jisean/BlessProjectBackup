#include "stdafx.h"
#include "LandObject.h"
#include "Export_Function.h"
#include "Transform.h"
#include "MainPlayer.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInfo(NULL)
, m_bCollision(false)
, m_bCulled(false)
, m_pCollision(NULL)
, m_iCullingCnt(0)
{
	m_htComponent.Ready_Table(19);
}

CLandObject::~CLandObject(void)
{

}

_int CLandObject::Update(const _float& fTimeDelta)
{
	//m_bCulled = true;
	return 0;
}

void CLandObject::Set_ConstantTable(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix, const D3DLIGHT9* pLightInfo, const D3DMATERIAL9* pMtrlInfo)
{
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", pWorldMatrix);

	_mymat			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);	

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightPos", (_vec4*)&pLightInfo->Position);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&pMtrlInfo->Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pMtrlInfo->Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pMtrlInfo->Specular);

	matView.InverseThis(&matView);

	_vec4		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));
	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", pMtrlInfo->Power);
	pEffect->SetFloat("g_fFogRender", 0.f);

	if(((CMainPlayer*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front()))->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE2)
		pEffect->SetFloat("g_fFogRender", 1.f);

	Engine::Safe_Release(pEffect);
}

void CLandObject::Culling_Frustum_Object(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
	_myvec vRadius = vMax - vMin;
	_float fRad = vRadius.Length();
	m_bCulled = Engine::Decision_InFrustum_Obj(m_pDevice, &vMin, fRad) 
		|| Engine::Decision_InFrustum_Obj(m_pDevice, &vMax, fRad);
}
