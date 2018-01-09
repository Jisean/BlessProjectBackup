#include "stdafx.h"
#include "Sword.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "DynamicMesh.h"
#include "Paladin.h"
#include "Guardian.h"
#include "Material.h"
#include "Shader.h"

CSword::CSword(LPDIRECT3DDEVICE9 pDevice)
: CEquipment(pDevice)
{

}

CSword::~CSword(void)
{
	m_pmatWeaponRef = NULL;
	m_pmatPlayerInfo = NULL;


}

HRESULT CSword::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_vPoint[0] = D3DXVECTOR3(0.f,0.f,0.f);
	m_vPoint[1] = D3DXVECTOR3(0.f,0.f,0.f);

	m_fTrailDistance = 20.f;

	return S_OK;
}

_int CSword::Update(const _float& fTimeDelta)
{
	if(m_pmatPlayerInfo == NULL || m_pmatWeaponRef == NULL)
	{
		m_pGuardian = ((CGuardian*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Guardian")->front());

		m_pmatPlayerInfo = &(static_cast<const Engine::CTransform*>(((Engine::CTransform*)m_pGuardian->GetComponent(L"Transform"))))->m_matWorld;
		m_pmatWeaponRef = ((Engine::CDynamicMesh*)m_pGuardian->GetComponent(L"Mesh"))->FindFrame("BN_Weapon_R");
	}


	//m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(90.f);

	_int iResult = Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return iResult;
}

void CSword::Render(void)
{
	if(m_pGuardian != NULL)	
	{
		if(m_pGuardian->Test_ActBitSet(CPlayers::ACT_MOUNT))
			return;
	}

	////////////////////////////////////////////////////////////////////////// 혜은 추가
	if(m_pmatPlayerInfo != NULL && m_pmatWeaponRef != NULL  
		&& (m_eShadowRender == CGameObject::SHADOW_SHADOW || m_eShadowRender == CGameObject::SHADOW_NONE_D))	
	{
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatPlayerInfo);

		//=============================트레일 Pos=====================


		memcpy(&m_vTrailPos[1], &m_pInfo->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

		m_vTrailPos[0] = m_vTrailPos[1];

		D3DXMATRIX matLocal;

		D3DXMatrixIdentity(&matLocal);

		D3DXMatrixInverse(&matLocal, 0 , &m_pInfo->m_matWorld);

		D3DXVec3TransformCoord(&m_vTrailPos[0], &m_vTrailPos[0], &matLocal);

		m_vTrailPos[0].y += m_fTrailDistance;

		D3DXVec3TransformCoord(&m_vTrailPos[0], &m_vTrailPos[0], & m_pInfo->m_matWorld);

		D3DXVec3TransformCoord(&m_vTrailPos[1], &m_vTrailPos[1], &matLocal);

		m_vTrailPos[1].y += 10.f;

		D3DXVec3TransformCoord(&m_vTrailPos[1], &m_vTrailPos[1], & m_pInfo->m_matWorld);

		//=============================================================

	}
	LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	CEquipment::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

	pEffect->AddRef();

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CEquipment::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);

	m_pMesh->RenderMesh_ForShader(pEffect,&m_pInfo->m_matWorld);

	Engine::Safe_Release(pEffect);

}

HRESULT CSword::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_Sword");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);

	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_VTF");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	return S_OK;
}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSword*		pGameObject = new CSword(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSword::Free(void)
{
	Engine::CGameObject::Free();
}

void CSword::CreateSpherePos()
{
	m_pMesh->GetMinMax(&m_vPoint[0],&m_vPoint[1]);

	m_vPoint[0];
	m_vPoint[1];

	_vec3 vMin,vMax;

	vMax = m_vPoint[1];
	vMin = _vec3(m_vPoint[0].x,m_vPoint[1].y,m_vPoint[1].z + 2.f);

	_vec3 vPos = (vMin + vMax) / 2.5f; 

	D3DXVec3TransformCoord(&vPos,&vPos,&m_pInfo->m_matWorld);

	//D3DXVec3TransformCoord(&vHandPos,&vHandPos,&m_pInfo->m_matWorld);
}
