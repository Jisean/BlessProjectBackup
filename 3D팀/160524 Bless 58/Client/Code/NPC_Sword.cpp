#include "stdafx.h"
#include "NPC_Sword.h"

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
#include "CityGuard2.h"
#include "MainPlayer.h"

CNPC_Sword::CNPC_Sword(LPDIRECT3DDEVICE9 pDevice)
: CEquipment(pDevice)
{
	m_pNpc = NULL;
	m_pNPCCuard2 = NULL;
	m_pmatWeaponRef = NULL;
	m_pmatPlayerInfo = NULL;
	m_pObjName = NULL;

}

CNPC_Sword::~CNPC_Sword(void)
{

}

HRESULT CNPC_Sword::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_vPoint[0] = D3DXVECTOR3(0.f,0.f,0.f);
	m_vPoint[1] = D3DXVECTOR3(0.f,0.f,0.f);

	return S_OK;
}

_int CNPC_Sword::Update(const _float& fTimeDelta)
{
	if(m_pmatPlayerInfo == NULL || m_pmatWeaponRef == NULL)
	{
		TCHAR	szObjName[30] = L"";
		wsprintf(szObjName, m_pObjName, m_iIndex);

		const Engine::CComponent*	pPlayerInfo = Engine::Get_Component(LAYER_GAMELOGIC, szObjName, L"Transform");
		const Engine::CComponent*	pPlayerMesh = Engine::Get_Component(LAYER_GAMELOGIC, szObjName, L"Mesh");
		m_pmatPlayerInfo = &(static_cast<const Engine::CTransform*>(pPlayerInfo)->m_matWorld);
	}

	CMainPlayer* pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front());
	if(pMainPlayer->Get_MainPlayerScene() == CMainPlayer::SCENE_STAGE1)
	{
		if(*pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_MAIN)
		{
			if(*pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_BEFORE)
				return 0;

			if(*pMainPlayer->Get_QuestStep() == CMainPlayer::QUEST_COMPLETE)
				return 0;
		}

		if(*pMainPlayer->Get_QuestType() == CMainPlayer::QUESTTYPE_SUB)
			return 0;
	}

	_int iResult = Engine::CGameObject::Update(fTimeDelta);
	
	if(m_pNpc->Get_bCulled())
		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	return iResult;
}

void CNPC_Sword::Render(void)
{
	////////////////////////////////////////////////////////////////////////// ÇýÀº Ãß°¡

	if(m_pmatPlayerInfo != NULL && m_pmatWeaponRef != NULL  
		&& (m_eShadowRender == CGameObject::SHADOW_SHADOW || m_eShadowRender == CGameObject::SHADOW_NONE_D))	
	{
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatPlayerInfo);

	}
	else
		return;


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


HRESULT CNPC_Sword::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_Sword4");
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

CNPC_Sword* CNPC_Sword::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CNPC_Sword*		pGameObject = new CNPC_Sword(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CNPC_Sword::Free(void)
{
	Engine::CGameObject::Free();
}

