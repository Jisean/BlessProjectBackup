#include "stdafx.h"
#include "SphereBullet.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "DynamicMesh.h"
#include "Paladin.h"
#include "ComSphere.h"
#include "Material.h"
#include "Ranger.h"
#include "Bullet.h"
#include "Bow.h"
#include "DynamicCamera.h"
#include "KeyMgr.h"
#include "DmgView.h"
#include "Monsters.h"
#include "Status.h"
#include "Magician.h"
#include "Effect_Manager.h"

static int iCnt = 0;
CSphereBullet::CSphereBullet(LPDIRECT3DDEVICE9 pDevice,WORD wNumber)
: CBullet(pDevice)
, m_pTargetMonster(NULL)
, m_pvecMonster(NULL)
, m_fRadian(0.f)
, m_vTargetPos(_vec3(0,0,0))
, m_fShotAcc(0.f)
, m_pMagician(NULL)
, m_fRotY_Angle(45.f)
, m_fAxis_Angle(0.f)
, m_bTemp(false)
, m_fSkillAngle(0.f)
, m_vEffectPos(_vec3(0,0,0))
, m_bFirst(false)
, m_fTimeSkill(0.f)
{
	m_wNumber = wNumber;
}

CSphereBullet::~CSphereBullet(void)
{

}

HRESULT CSphereBullet::Initialize()
{
	m_fTimeDelta = 0.f;
	m_vLook = _vec3(1.f,0.f,0.f);
	m_fSpeed = 20.f;

	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.03f, 0.03f, 0.03f);
	


	m_bDead = true;
	return S_OK;
}

_int CSphereBullet::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if(m_bDead)
	{
		if(m_pMagician != NULL)
		{
			/*if(m_pMagician->Get_TrackID() != CPlayers::ATK_0)
			{
				if(true == Engine::CEffect_Manager::GetInstance()->GetMagicianCalling(Engine::M_DEFAULT1))
					Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT1);

				if(Engine::CEffect_Manager::GetInstance()->GetMagicianCalling(Engine::M_DEFAULT_2_FIRST))
					Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT_2_FIRST);
			}*/
		}
		

		m_pvecMonster->clear();
		return 0;
	}

	if(NULL == m_pMagician)
		m_pMagician = ((CMagician*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Magician")->front());

	D3DXVec3TransformNormal(&m_pInfo->m_vDir,&m_vLook,&m_pInfo->m_matWorld);

	m_fTimeDelta = fTimeDelta;

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	SetUp();

	if(m_bFirst == true)
	{
		if(m_fTimeSkill > 2.f)
		{
			Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT_2_FIRST);
			Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT1);
			m_bFirst = false;
			m_bDead = true;
			m_fShotAcc = 0.f;
			m_fTimeSkill = 0.f;
		}
		else
			m_fTimeSkill += m_fTimeDelta;
	}
	
	if(m_bDead == false)
	{
		m_vEffectPos = _vec3(m_vTargetPos.x,m_vTargetPos.y + 0.02f,m_vTargetPos.z);
		_vec3 vTargetPos = _vec3(m_vTargetPos.x,0.f,m_vTargetPos.z);
		_vec3 vMagicianPos = _vec3(const_cast<Engine::CTransform*>(m_pMagician->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pMagician->GetInfo())->m_vPos.z);
		_vec3 vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
		_myvec vMagician_Look = const_cast<Engine::CTransform*>(m_pMagician->GetInfo())->m_vDir;
		_myvec vMagicianDir = vTargetPos - vMagicianPos;
		_myvec vDir = vTargetPos - vMyPos;
		_float fDis = vDir.Length();
		_float fMagicianDis = vMagicianDir.Length();
		vMagician_Look.Normalize();
		vMagicianDir.Normalize();
		vDir.Normalize();
		_float fRadian = acosf(vDir.Dot(&m_vLook));

		if(vMagicianPos.z > vTargetPos.z)
			fRadian = -fRadian;
		else
			fRadian = fRadian;

		m_pInfo->m_fAngle[Engine::ANGLE_Y] = -fRadian;
		m_fSkillAngle = m_pInfo->m_fAngle[Engine::ANGLE_Y];

		if(m_pMagician->Get_First_Atk())
		{
			if(fDis >= 0.3f)
			{
				/*_vec3 vRight = _vec3(0,0,0);
				memcpy(&vRight,&m_pInfo->m_matWorld.m[2][0],sizeof(D3DXVECTOR3));
				
				_mymat matAxis,matRotY,matWorld;	///커브 ㅜㅜ
				matAxis.Identity();
				matRotY.Identity();
				matWorld.Identity();

				D3DXMatrixRotationY(&matRotY,D3DXToRadian(m_fRotY_Angle));
				D3DXMatrixRotationAxis(&matAxis,&vRight,D3DXToRadian(m_fAxis_Angle));
				matWorld = matRotY * matAxis;
				D3DXVec3TransformNormal(&vDir,&vDir,&matWorld);

				m_fSkillAngle = D3DXVec3Dot(&vRight, &vDir);
				m_fSkillAngle = acosf(m_fSkillAngle);

				if(m_fAxis_Angle <= -5.f)
					m_fAxis_Angle = m_fAxis_Angle;
				else
					m_fAxis_Angle -= 2.f;

				if(m_fRotY_Angle <= 0.f)
					m_fRotY_Angle = 0.f;
				else
					m_fRotY_Angle -= 2.f;*/

				m_fShotAcc += 2.f * m_fTimeDelta;
				m_pInfo->m_vPos += vDir * m_fTimeDelta * ((m_fSpeed * 0.3f) * m_fShotAcc + m_fShotAcc * 0.2f - 0.2f);
				Engine::Call_MagicianFX(Engine::M_DEFAULT1,&m_pInfo->m_vPos,true,NULL,&m_fSkillAngle);
			}
			else
			{
				m_fShotAcc = 0.f;
				m_fRotY_Angle = 45.f;
				m_fAxis_Angle = 0.f;
				m_fSkillAngle = 0.f;
				Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT1);

				if(m_pTargetMonster != NULL)
				{
					if(m_pTargetMonster->Get_bDieState() == false)
					{
						m_pInfo->m_vPos = const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos;

						if(m_bFirst == false)
						{
							Engine::Call_MagicianFX(Engine::M_DEFAULT_2_FIRST,&const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos,true);
							Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT1);
							m_pMagician->Set_First_Atk(false);
							m_bFirst = true;
						}
					}
				}
				
			}
		}
		else
		{
			Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT1);
		}
		if(m_pMagician->Get_Second_Atk())
		{
			
			if(m_pTargetMonster != NULL)
			{
				_vec3 vTargetPos, vMyPos;
				vTargetPos = _vec3(const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos.z);
				vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
				_myvec vDir = vTargetPos - vMyPos;
				_float fDis = vDir.Length();

				m_bFirst = false;
			
				if(fDis < 3.f)
				{
					m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(1),(CUnits*)m_pMagician,Engine::CStatus::DMG_MELEE);
					const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
					Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
					Engine::CEffect_Manager::GetInstance()->Off_MagicianFX(Engine::M_DEFAULT_2_FIRST);
					Engine::Call_MagicianFX(Engine::M_DEFAULT_2_SECOND,&const_cast<Engine::CTransform*>(m_pTargetMonster->GetInfo())->m_vPos,false);
				}
			}
			m_pMagician->Set_Second_Atk(false);
			m_bDead = true;

		}
	}
	else
	{
		m_fShotAcc = 0.f;

	}
	return 0;
}

void CSphereBullet::Render(void)
{
	if(m_bDead)
		return;
	
	return;
	//if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
	//	return;

	////////////////////////////////////////////////////////////////////////// 혜은 추가
	LPD3DXEFFECT			pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	CBullet::Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);
	//__debugbreak();

	pEffect->AddRef();

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();

	CBullet::Set_ConstantTable(pEffect, &m_pInfo->m_matWorld, pLightInfo, pMtrlInfo);

	m_pMesh->RenderMesh_ForShader(pEffect,&m_pInfo->m_matWorld);

	Engine::Safe_Release(pEffect);
}

void CSphereBullet::SetUp()
{
	D3DXMATRIX  matAxis;
	D3DXVECTOR3 vRight, vUp;
	vUp = D3DXVECTOR3(0,1,0);

	D3DXVec3Cross(&vRight,&vUp,&m_pInfo->m_vDir);

	D3DXMatrixRotationAxis(&matAxis, &vRight, m_fRadian);	

	D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale, matParent;

	D3DXMatrixScaling(&matScale, m_pInfo->m_vScale.x, m_pInfo->m_vScale.y, m_pInfo->m_vScale.z);
	D3DXMatrixTranslation(&matTrans, 0,0,0);
	D3DXMatrixTranslation(&matParent, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	D3DXMatrixRotationX(&matRotX, m_pInfo->m_fAngle[Engine::ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_pInfo->m_fAngle[Engine::ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_pInfo->m_fAngle[Engine::ANGLE_Z]);

	m_pInfo->m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans * matAxis * matParent;

}

void CSphereBullet::SphereTrans(_vec3 vPos , CMonsters* pTargetMon, float fAngle /*= 0.f*/)
{
	m_pTargetMonster = pTargetMon;
	m_vTargetPos = vPos;

	Engine::CTransform* pTransform_Magician = NULL;	
	pTransform_Magician = const_cast<Engine::CTransform*>(((CMagician*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Magician")->front())->GetInfo());

	m_pInfo->m_vPos = pTransform_Magician->m_vPos;
	m_pInfo->m_vPos.y += (pTransform_Magician->m_vPos.y + 20.f)* pTransform_Magician->m_vScale.y;

	m_pInfo->m_vDir = pTransform_Magician->m_vDir;

	m_pInfo->m_vScale = _vec3(0.01f,0.01f,0.01f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = pTransform_Magician->m_fAngle[Engine::ANGLE_Y];

	_myvec vDir = pTransform_Magician->m_vDir;
	_myvec vLook = _vec3(vDir.x,0.f,vDir.z);
	vDir.Normalize();
	vLook.Normalize();

	if(fAngle == 0.f)
	{
		m_fRadian = acosf(vDir.Dot(&vLook));

		if(m_pInfo->m_vPos.z > pTransform_Magician->m_vPos.z)
			m_fRadian = -m_fRadian;
		else
			m_fRadian = m_fRadian;	
	}
	else
	{
		m_fRadian = fAngle;
	}

	m_fShotAcc = 0.f;
	m_bDead = false;
}

void CSphereBullet::Collision_Monster()
{

}
void CSphereBullet::Set_MonContainer(vector<CMonsters*>* pvecMon)
{
	m_pvecMonster = pvecMon;
}

HRESULT CSphereBullet::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_Sphere");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_htComponent.Insert_Table(L"Mesh", pComponent);


	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 5.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);


	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_VTF");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	return S_OK;
}

CSphereBullet* CSphereBullet::Create(LPDIRECT3DDEVICE9 pDevice , WORD wNumber)
{
	CSphereBullet*		pGameObject = new CSphereBullet(pDevice,wNumber);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSphereBullet::Free(void)
{
	CBullet::Free();
}