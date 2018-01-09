#include "stdafx.h"
#include "BalianBollwerk.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Calculator.h"
#include "Material.h"
#include "Status.h"
#include "AniCtrl.h"
#include "KeyMgr.h"

#include "AIMgr.h"
#include "Monsters.h"
#include "MainPlayer.h"


CBalianBollwerk::CBalianBollwerk(LPDIRECT3DDEVICE9 pDevice)
: CNPCs(pDevice)
{

}

CBalianBollwerk::~CBalianBollwerk(void)
{

}

HRESULT CBalianBollwerk::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.01f, 0.01f, 0.01f);

	m_pInfo->m_vPos = _vec3(121.f, 0.f, 159.f);
	m_pInfo->m_vPos = _vec3(126.746f, 10.3773f, 232.315f);
	
	
	m_pInfo->m_fAngle[Engine::ANGLE_Y]= 1.57f;
	m_dwIndex = 580;
	
	m_fCautionRange = 1.f;
	m_fCollisionRange = 0.4f;
	m_fAttackDist = 0.4f;

	return S_OK;
}

_int CBalianBollwerk::Update(const _float& fTimeDelta)
{
	if(!m_bIninInfo)
	{
		m_vOriPos = m_pInfo->m_vPos;
		m_fOriAngle = m_pInfo->m_fAngle[Engine::ANGLE_Y];
		m_bIninInfo = true;
		m_dwIndex = Engine::CNaviMgr::GetInstance()->GetCellIndex(&m_pInfo->m_vPos);
	}

	//¿ä°ÅÇÏ¸é ÁöÇüÅ½(ÄÞÆ÷³ÍÆ®¿¡ Calculator Ãß°¡)
	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&m_pInfo->m_vPos, L"Terrain");
	m_pInfo->m_vPos.y = fHeight;

	if(m_pTargetInfo != NULL)
	{
		if(((CMonsters*)m_pTargetInfo)->Get_AniIdx() == CAIMgr::STATE_DIE)
			m_pTargetInfo = NULL;
	}

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &m_vLook, &m_pInfo->m_matWorld);
	AniFrameEnd();

	_int iResult = 0;
	if(m_bCulled)
	{
		if(20 == m_iCullingCnt)
		{
			iResult = CUnits::Update(fTimeDelta);
			m_iCullingCnt = 0;
		}
		else
			++m_iCullingCnt;
	}
	else
		iResult = CUnits::Update(fTimeDelta);

	iResult = Engine::CGameObject::Update(fTimeDelta);

	if(m_bCulled)
		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	m_fTimeDelta = fTimeDelta;
	return iResult;
}

void CBalianBollwerk::Render(void)
{
	m_pMesh->FrameMove(m_fTimeDelta);
	if(m_bCulled)
	{
		LPD3DXEFFECT	pEffect = m_pShader->Get_EffectHandle();
		if(NULL == pEffect)
			return;

		CUnits::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

		pEffect->AddRef();

		Set_ContantTable(pEffect);

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);

		m_pMesh->RenderMesh_ForShader(pEffect);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

void CBalianBollwerk::AniFrameEnd(/*Engine::ANI_INFO* pData*/)
{
	if(m_pMesh->GetPeriod() - 0.3f/*pData->fPeriod*/ < m_pMesh->GetTrackPosition())
	{		
		if(m_bAttackEnd)
		{
			m_bAttackEnd = false;
			m_bAttack = false;
		}

		if(m_iAniIdx == CAIMgr::STATENPC_IDLE_P)
		{
			m_iAniIdx = CAIMgr::STATENPC_IDLE_P;
			m_iPrevAniIdx = CAIMgr::STATENPC_IDLE_P;
		}
		else
		{	
			if(m_iAniIdx != CAIMgr::STATENPC_RUN && m_iAniIdx != CAIMgr::STATENPC_WALK)
			{
				m_iAniIdx = CAIMgr::STATENPC_IDLE_C;
				m_iPrevAniIdx = CAIMgr::STATENPC_IDLE_C;
				m_pMesh->SetAnimationSet(m_iAniIdx);
				m_pMesh->SetTrackPos(0.0);
			}	
		}
	}
}

HRESULT CBalianBollwerk::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_BalianBollwerk");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_VTF");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 10000.0f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);

	// For.Status Component
	Engine::_BaseStat* pBaseStat = new Engine::_BaseStat;
	pBaseStat->m_fStr = 25.f;
	pBaseStat->m_fDex = 15.f;
	pBaseStat->m_fHealth = 30.f;
	pBaseStat->m_fInt = 8.f;
	pBaseStat->m_fWis = 8.f;
	pBaseStat->m_fExStat = 10.f;
	pBaseStat->m_fLevel = 1.f;
	pComponent = m_pStatus = Engine::CStatus::Create(*pBaseStat);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Status", pComponent);
	Engine::Safe_Delete(pBaseStat);

	//Calculator
	pComponent = m_pCalculatorCom = CCalculator::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Calculator", pComponent);

	return S_OK;
}

void CBalianBollwerk::Set_ContantTable(LPD3DXEFFECT pEffect)
{
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CLandObject::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);
}

CBalianBollwerk* CBalianBollwerk::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBalianBollwerk*		pGameObject = new CBalianBollwerk(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CBalianBollwerk::Free(void)
{
	Engine::CGameObject::Free();
	CUnits::Free();
}

