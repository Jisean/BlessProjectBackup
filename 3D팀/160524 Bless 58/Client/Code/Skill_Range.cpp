#include "stdafx.h"
#include "Skill_Range.h"

#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "Export_Function.h"
#include "Sword.h"
#include "Scene.h"
#include "Stage.h"

#include "Skill_Range_1.h"

CSkill_Range::CSkill_Range(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pShader(NULL)
{
	m_htComponent.Ready_Table(119);
	m_bStart = false;
	m_fSpeed = 0.f;
	m_fAlpha_Ctr = 0.5f;

	m_pSkill_Range_1 = NULL;
}

CSkill_Range::~CSkill_Range(void)
{

}

HRESULT CSkill_Range::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_pInfo->m_vScale = _vec3(2.f, 2.f, 2.f);
	m_pInfo->m_vPos = _vec3(112 * 1.f, 3.f, 125.f);
	m_pInfo->m_fAngle[Engine::ANGLE_X] = D3DXToRadian(90);
	
	m_bDead = true;
	return S_OK;
}

_int CSkill_Range::Update(const _float& fTimeDelta)
{
	if(m_pSkill_Range_1 != NULL)
	{
		if(m_pSkill_Range_1->Get_bEnd())
		{
			if(m_fAlpha_Ctr <= 0.f)
			{
				m_pSkill_Range_1 = NULL;
				m_bDead = true;
			}
			else
				m_fAlpha_Ctr -= 0.03f;
		}
	}
	
	Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_ALPHA, this);
	_int iResult = Engine::CGameObject::Update(fTimeDelta);

	return iResult;
}

void CSkill_Range::Render(void)
{
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->SetTechnique("SkillRange");
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fAlpha",m_fAlpha_Ctr);

	pEffect->SetFloat("g_fRGB",1.f);
	pEffect->SetFloat("g_fPointPower",1.f);

	m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture", 0);

	pEffect->Begin(NULL,0);
	pEffect->BeginPass(0);

	m_pBuffer->Render(&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CSkill_Range::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader",pComponent);


	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Skill_Range");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);

	return S_OK;
}

CSkill_Range* CSkill_Range::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkill_Range* pGameObject = new CSkill_Range(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSkill_Range::Free(void)
{
	Engine::CGameObject::Free();
}

void CSkill_Range::Start_SkillRange(CSkill_Range_1* pSkillRange, _vec3 vPos, float fRange, float fSpeed)
{
	m_bDead = false;
	m_fAlpha_Ctr = 0.5f;
	m_pSkill_Range_1 = pSkillRange;
	vPos.y += 0.06f;
	m_pInfo->m_vPos = vPos;
	

	m_pInfo->m_vScale = _vec3(fRange, fRange, fRange);
	m_fSpeed = fSpeed;
	return;
}

