#include "stdafx.h"
#include "Sphere.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "CollisionMgr.h"
#include "Calculator.h"
#include "Material.h"

CSphere::CSphere(LPDIRECT3DDEVICE9 pDevice)
: CMapObjects(pDevice)
{

}

CSphere::~CSphere(void)
{

}

HRESULT CSphere::Initialize(const TCHAR* pMeshKey, _vec3 vPos/* = _vec3(0,0,0)*/)
{
	FAILED_CHECK(AddComponent(pMeshKey));
	m_pInfo->m_vScale = _vec3(0.005f,0.005f,0.005f);
	m_pInfo->m_vPos = vPos;
	return S_OK;
}

_int CSphere::Update(const _float& fTimeDelta)
{
	if(m_bDead)
		return 0;
	//¿ä°ÅÇÏ¸é ÁöÇüÅ½(ÄÞÆ÷³ÍÆ®¿¡ Calculator Ãß°¡)
	/*_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
	m_pInfo->m_vPos.y = fHeight;*/

	_int iResult = Engine::CGameObject::Update(fTimeDelta);
	//Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_STATIC, this);

	return iResult;
}

void CSphere::Render(void)
{
	if(m_bDead)
		return;

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	CMapObjects::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

	pEffect->AddRef();

	Set_ConstantTable(pEffect);

	m_pMesh->RenderMesh_ForShader(pEffect, &m_pInfo->m_matWorld);

	Engine::Safe_Release(pEffect);

	m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

}

HRESULT CSphere::AddComponent(const TCHAR* pMeshKey)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	if(pMeshKey)
		pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, pMeshKey);
	else
		pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_Sphere");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Mesh");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);	

	//Calculator
	pComponent = m_pCalculatorCom = CCalculator::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Calculator", pComponent);

	return S_OK;
}
void CSphere::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

void CSphere::SetPos(_vec3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

CSphere* CSphere::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey/* = NULL*/, _vec3 vPos /*= _vec3(0,0,0)*/)
{
	CSphere*		pGameObject = new CSphere(pDevice);
	if(FAILED(pGameObject->Initialize(pMeshKey,vPos)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSphere::Free(void)
{
	Engine::CGameObject::Free();
}