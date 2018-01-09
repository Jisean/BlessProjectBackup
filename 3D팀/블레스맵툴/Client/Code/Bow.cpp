#include "stdafx.h"
#include "Bow.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "DynamicMesh.h"

CBow::CBow(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pmatPlayerInfo(NULL)
, m_pmatWeaponRef(NULL)
{

}

CBow::~CBow(void)
{

}

HRESULT CBow::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	return S_OK;
}

_int CBow::Update(const _float& fTimeDelta)
{
	if(m_pmatPlayerInfo == NULL || m_pmatWeaponRef == NULL)
	{
		const Engine::CComponent*	pPlayerInfo = Engine::Get_Component(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
		const Engine::CComponent*	pPlayerMesh = Engine::Get_Component(CStage::LAYER_GAMELOGIC, L"Player", L"Mesh");

		m_pmatPlayerInfo = &(static_cast<const Engine::CTransform*>(pPlayerInfo)->m_matWorld);
		m_pmatWeaponRef = ((Engine::CDynamicMesh*)pPlayerMesh)->FindFrame("Ref_Weapon_Left");
	}
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(90.f);

	_int iResult = Engine::CGameObject::Update(Engine::Get_Time());
	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatPlayerInfo);

	return iResult;
}

void CBow::Render(void)
{
	m_pMesh->RenderMesh_ForSDK(&m_pInfo->m_matWorld);
}

HRESULT CBow::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Bow");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));
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
	Engine::CGameObject::Free();
}
