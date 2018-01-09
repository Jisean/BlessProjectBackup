#include "stdafx.h"
#include "Bullet.h"
#include "GameObject.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Stage.h"
#include "MainPlayer.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_pMesh(NULL)
, m_pShader(NULL)
, m_pMtrlCom(NULL)
, m_vLook(1.f, 0.f, 0.f)
, m_pUnitInfo(NULL)
, m_pTarget_Matrix(NULL)
, m_wNumber(0)
, m_fTimeDelta(0.f)
, m_fSpeed(0.f)
{

}

CBullet::~CBullet(void)
{

}

void CBullet::Free(void)
{
	Engine::CGameObject::Free();
}

void CBullet::Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix /*= NULL*/)
{
	m_pMesh->Set_BoxRender(g_bBoundingBoxRender);

	if(m_eShadowRender == Engine::CGameObject::SHADOW_NONE_D)
		pEffect->SetTechnique("Default_Device");
	else if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
		pEffect->SetTechnique("D_Draw_Shadow");
	else if(m_eShadowRender == Engine::CGameObject::SHADOW_OBJECT)
		pEffect->SetTechnique("D_Draw_Default");

	pEffect->SetFloat("g_fSpecMapping", 1.f);
	pEffect->SetFloat("g_fWeaponCheck", 1.f);
	// Dynamic Shaodw Set
	Engine::CTransform* pPlayerInfo = (Engine::CTransform*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");

	_myvec vDLightDir = SSHADOWEYE - SSHADOWAT;
	D3DXVec3Normalize(&vDLightDir, &vDLightDir);
	_myvec vLightPos = pPlayerInfo->m_vPos + (vDLightDir * 10.f);

	_mymat	matLightView, matLightProj;
	D3DXMatrixLookAtLH(&matLightView, &vLightPos, &pPlayerInfo->m_vPos, &D3DXVECTOR3(0,1,0));
	D3DXMatrixOrthoLH(&matLightProj, 3.f, 3.f, 1.f, 1000.f);

	_mymat matLightWVP;
	matLightWVP = (*pWorldMatrix) * matLightView * matLightProj;

	pEffect->SetMatrix("g_matDLightWVP", &matLightWVP);
	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Dynamic", "g_DShadowTexture");

	// Static Shadow Set
	_mymat	matSLightView, matSLightProj;
	D3DXMatrixLookAtLH(&matSLightView, &D3DXVECTOR3(-148.066f, 432.107f, -148.066f), &D3DXVECTOR3(-142.399f, 424.358f, -142.399f), &D3DXVECTOR3(0,1,0));
	D3DXMatrixOrthoLH(&matSLightProj, 250.f, 250.f, 1.f, 1000.f);

	_mymat matSLightWVP;
	matSLightWVP = (*pWorldMatrix) * matSLightView * matSLightProj;

	pEffect->SetMatrix("g_matSLightWVP", &matSLightWVP);
	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Static", "g_SShadowTexture");
}

void CBullet::Set_ConstantTable(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix, const D3DLIGHT9* pLightInfo, const D3DMATERIAL9* pMtrlInfo)
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

	Engine::Safe_Release(pEffect);
}

WORD CBullet::Get_wNumber()
{
	return m_wNumber;
}

_bool CBullet::Clear_Render(void)
{
	CMainPlayer* pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front());

	if(pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
	{
		if(*pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_MAIN && *pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_BEFORE)
			return true;
	}
	return false;
}
