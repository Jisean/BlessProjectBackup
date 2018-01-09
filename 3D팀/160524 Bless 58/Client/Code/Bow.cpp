#include "stdafx.h"
#include "Bow.h"

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
#include "Ranger.h"

CBow::CBow(LPDIRECT3DDEVICE9 pDevice)
: CEquipment(pDevice)
{
}

CBow::~CBow(void)
{

}

HRESULT CBow::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_vPoint[0] = D3DXVECTOR3(0.f,0.f,0.f);
	m_vPoint[1] = D3DXVECTOR3(0.f,0.f,0.f);

	return S_OK;
}

_int CBow::Update(const _float& fTimeDelta)
{
	if(m_bDead || Clear_Render())
		return 0;

	if(m_pmatPlayerInfo == NULL || m_pmatWeaponRef == NULL)
	{
		
		m_pRanger = ((CRanger*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front());

		m_pmatPlayerInfo = &(static_cast<const Engine::CTransform*>(((Engine::CTransform*)m_pRanger->GetComponent(L"Transform"))))->m_matWorld;
		m_pmatWeaponRef = ((Engine::CDynamicMesh*)m_pRanger->GetComponent(L"Mesh"))->FindFrame("BN_Weapon_L");
	}

	_int iResult = Engine::CGameObject::Update(fTimeDelta);
	
	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return iResult;
}

void CBow::Render(void)
{
	if(m_pRanger != NULL)
	{
		if(m_pRanger->Test_ActBitSet(CPlayers::ACT_MOUNT))
			return;
	}

	////////////////////////////////////////////////////////////////////////// ÇýÀº Ãß°¡
	if(m_pmatPlayerInfo != NULL && m_pmatWeaponRef != NULL  
		&& (m_eShadowRender == CGameObject::SHADOW_SHADOW || m_eShadowRender == CGameObject::SHADOW_NONE_D))	
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatPlayerInfo);

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

HRESULT CBow::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_Bow");
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

CBow* CBow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBow*		pGameObject = new CBow(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CBow::Free(void)
{
	CEquipment::Free();
}

void CBow::CreateSpherePos()
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
