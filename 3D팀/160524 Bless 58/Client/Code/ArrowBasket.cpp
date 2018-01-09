#include "stdafx.h"
#include "ArrowBasket.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "DynamicMesh.h"
#include "Paladin.h"
#include "Ursa_01.h"
#include "ComSphere.h"
#include "Material.h"

CArrowBasket::CArrowBasket(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pmatPlayerInfo(NULL)
, m_pmatWeaponRef(NULL)
, m_pComSphere(NULL)
{
}

CArrowBasket::~CArrowBasket(void)
{

}

HRESULT CArrowBasket::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_vPoint[0] = D3DXVECTOR3(0.f,0.f,0.f);
	m_vPoint[1] = D3DXVECTOR3(0.f,0.f,0.f);



	return S_OK;
}

_int CArrowBasket::Update(const _float& fTimeDelta)
{
	if(m_pmatPlayerInfo == NULL || m_pmatWeaponRef == NULL)
	{
		m_pPaladin = ((CPaladin*)Engine::Get_ObjList(CStage::LAYER_GAMELOGIC,L"Paladin")->front());

		m_pmatPlayerInfo = &(static_cast<const Engine::CTransform*>(((Engine::CTransform*)m_pPaladin->GetComponent(L"Transform"))))->m_matWorld;
		m_pmatWeaponRef = ((Engine::CDynamicMesh*)m_pPaladin->GetComponent(L"Mesh"))->FindFrame("BN_Weapon_R");

		if(m_pComSphere == NULL)
		{
			m_matSphere = const_cast<D3DXMATRIX*>(m_pmatWeaponRef);

			_matrix matSphere;
			memcpy(matSphere,(*m_matSphere),sizeof(D3DXMATRIX));

			Engine::CComponent*		pComponent = NULL;
			pComponent = m_pComSphere = Engine::CComSphere::Create(matSphere,10.f,m_pInfo->m_vPos);
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			m_htComponent.Insert_Table(L"Com_Sphere", pComponent);
		}
	}
	_int iResult = Engine::CGameObject::Update(fTimeDelta);

	CPaladin* pPaladin = ((CPaladin*)Engine::Get_ObjList(CStage::LAYER_GAMELOGIC,L"Paladin")->front());

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_STATIC, this);

	return iResult;
}

void CArrowBasket::Render(void)
{
	if(m_pPaladin->Get_TrackID() == CPlayers::SK_4 ||
		m_pPaladin->Get_TrackID() == CPlayers::SK_3)
		return;

	if(m_pmatPlayerInfo != NULL && m_pmatWeaponRef != NULL)		
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatPlayerInfo);

	CreateSpherePos();

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_mymat			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);	

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&pMtrlInfo->Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pMtrlInfo->Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pMtrlInfo->Specular);

	matView.InverseThis(&matView);

	_vec4		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));
	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", pMtrlInfo->Power);

	Engine::Safe_Release(pEffect);

	m_pMesh->RenderMesh_ForShader(pEffect, &m_pInfo->m_matWorld);

	_mymat matIden;
	matIden.Identity();
	m_pComSphere->Render(&matIden);

	//m_pMesh->RenderMesh_ForSDK(&m_pInfo->m_matWorld);

}

HRESULT CArrowBasket::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);


	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Mace");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);


	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);


	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Mesh");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	return S_OK;
}

CArrowBasket* CArrowBasket::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CArrowBasket*		pGameObject = new CArrowBasket(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CArrowBasket::Free(void)
{
	Engine::CGameObject::Free();
}

void CArrowBasket::CreateSpherePos()
{
	m_pMesh->GetMinMax(&m_vPoint[0],&m_vPoint[1]);

	_vec3 vMin,vMax;

	vMax = m_vPoint[1];
	vMin = _vec3(m_vPoint[0].x,m_vPoint[1].y,m_vPoint[1].z + 2.f);

	_vec3 vPos = (vMin + vMax) / 2.f; 

	D3DXVec3TransformCoord(&vPos,&vPos,&m_pInfo->m_matWorld);

	_mymat	matTrans;
	matTrans.Identity();
	matTrans.Trans(vPos);

	m_pComSphere->Set_Pos(vPos);


}
