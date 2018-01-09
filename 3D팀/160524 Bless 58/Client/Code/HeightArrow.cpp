#include "stdafx.h"
#include "HeightArrow.h"

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

CHeightArrow::CHeightArrow(LPDIRECT3DDEVICE9 pDevice,WORD wNumber)
: CBullet(pDevice)
, m_fTimeCount(0.f)
, m_fTimeMax(0.f)
, m_vTargetPos(_vec3(0,0,0))
, m_fDistance(0.f)
, m_pTransform_Ranger(NULL)
, m_fShotAcc(0.f)
, m_matHandle(NULL)
, m_fRadian(0.f)
, m_bTemp(false)
, m_fTimeSecond(0.f)
, m_pRanger(NULL)
, m_vLastDir(_vec3(0,0,0))
{
	m_wNumber = wNumber;
}

CHeightArrow::~CHeightArrow(void)
{

}

HRESULT CHeightArrow::Initialize()
{
	m_fTimeDelta = 0.f;
	m_vLook = _vec3(1.f,0.f,0.f);
	m_fSpeed = 0.f;

	//D3DXVec3TransformNormal(&m_pInfo->m_vDir,&m_vLook,&m_pInfo->m_matWorld);


	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.01f, 0.01f, 0.01f);
	
	m_bDead = true;
	return S_OK;
}

_int CHeightArrow::Update(const _float& fTimeDelta)
{
	if(m_bDead || Clear_Render())
		return 0;
	
	m_fTimeDelta = fTimeDelta;

	
	m_fTimeSecond += m_fTimeDelta;

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	if(m_pRanger == NULL)
		m_pRanger = ((CRanger*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front());


	if(m_bDead == false)
	{
		D3DXMATRIX  matAxis;
		D3DXVECTOR3 vRight, vUp;
		vUp = D3DXVECTOR3(0,1,0);

		D3DXVec3Cross(&vRight,&vUp,&m_pInfo->m_vDir);
		D3DXMatrixRotationAxis(&matAxis, &vRight, -m_fRadian);	
		D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale, matParent;

		D3DXMatrixScaling(&matScale, m_pInfo->m_vScale.x, m_pInfo->m_vScale.y, m_pInfo->m_vScale.z);
		D3DXMatrixTranslation(&matTrans, 0,0,0);
		D3DXMatrixTranslation(&matParent, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

		D3DXMatrixRotationX(&matRotX, m_pInfo->m_fAngle[Engine::ANGLE_X]);
		D3DXMatrixRotationY(&matRotY, m_pInfo->m_fAngle[Engine::ANGLE_Y]);
		D3DXMatrixRotationZ(&matRotZ, m_pInfo->m_fAngle[Engine::ANGLE_Z]);

		if(m_fTimeSecond > 2.f)
		{
			m_bDead = true;
			m_fTimeSecond = 0.f;
		}
		else
		{
			m_pInfo->m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans * matAxis * matParent;
			m_pInfo->m_vPos += m_pInfo->m_vDir * (m_fSpeed * 0.1f) * m_fTimeDelta;
		}
	}
	else
	{
		m_bDead = true;
		m_fShotAcc = 0.f;
	}

	//Collision_Monster();

	//_int iResult = Engine::CGameObject::Update(fTimeDelta);
	return 0;
}

void CHeightArrow::Render(void)
{
	if(m_bDead)
		return;

	//if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
	//	return;

	////////////////////////////////////////////////////////////////////////// ÇýÀº Ãß°¡
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
HRESULT CHeightArrow::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(D3DXVECTOR3(1.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//StaticMesh
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Mesh_FireArrow");
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

CHeightArrow* CHeightArrow::Create(LPDIRECT3DDEVICE9 pDevice , WORD wNumber)
{
	CHeightArrow*		pGameObject = new CHeightArrow(pDevice,wNumber);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CHeightArrow::Free(void)
{
	CBullet::Free();
}

void CHeightArrow::ArrowTrans(_vec3 vDir,float fSpeed , float fTime , float fAngle /*= 0.f*/)
{
	m_fTimeMax = fTime;
	m_fSpeed = fSpeed;

	if(m_pTransform_Ranger == NULL || m_matHandle == NULL)
	{
		m_pTransform_Ranger = const_cast<Engine::CTransform*>(((CRanger*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front())->GetInfo());
		m_matHandle = const_cast<Engine::CDynamicMesh*>((((CUnits*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front())->GetMesh()))->FindFrame("BN_Weapon_R");
	}

	//cout << (*m_matHandle)._42 << endl;
	_vec3 vBowPos = ((Engine::CTransform*)((CBow*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Bow")->front())->GetComponent(L"Transform"))->m_vPos;

	m_pInfo->m_vPos		= m_pTransform_Ranger->m_vPos;

	m_pInfo->m_vPos.x	+= vBowPos.x * m_pTransform_Ranger->m_vScale.x;
	m_pInfo->m_vPos.y	+= ( m_pTransform_Ranger->m_vPos.y  + 20.f)* m_pTransform_Ranger->m_vScale.y;
	m_pInfo->m_vPos.z	+= vBowPos.z * m_pTransform_Ranger->m_vScale.z;

	m_pInfo->m_vScale = _vec3(0.01f,0.01f,0.01f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = m_pTransform_Ranger->m_fAngle[Engine::ANGLE_Y];

	_myvec vLook , vPos , vRangerDir;

	_vec3 vTemp(0,0,0);
	vPos = m_pTransform_Ranger->m_vPos + m_pTransform_Ranger->m_vDir * 20.f;
	vPos.y = vPos.y + 0.5f;
	vRangerDir = vPos - m_pInfo->m_vPos;
	vLook = _vec3(vRangerDir.x,0.f,vRangerDir.z);

	vRangerDir.Normalize();
	vLook.Normalize();
	_float fRadian = acosf(vRangerDir.Dot(&vLook));

	if(m_pInfo->m_vPos.y > vPos.y)
		fRadian = -fRadian;
	else
		fRadian = fRadian;

	m_fRadian = fRadian;

	m_pInfo->m_vDir = vRangerDir;


	m_fShotAcc = 0.f;
	m_bDead = false;
}
