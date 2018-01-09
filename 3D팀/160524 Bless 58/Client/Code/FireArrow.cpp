#include "stdafx.h"
#include "FireArrow.h"

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
#include "SoundMgr.h"
#include "MainPlayer.h"

static int iCnt = 0;
CFireArrow::CFireArrow(LPDIRECT3DDEVICE9 pDevice,WORD wNumber)
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
, m_pTargetMonster(NULL)
, m_bWidthShot(false)
, m_pvecMonster(NULL)
, m_bEffect_First(false)
, m_wEffectNumber(0)
, m_wRanger_TrackID(0)
, m_vPrevPos(_vec3(0,0,0))
, m_vTemp(_vec3(0,0,0))
{
	m_wNumber = wNumber;
}

CFireArrow::~CFireArrow(void)
{

}

HRESULT CFireArrow::Initialize()
{
	m_fTimeDelta = 0.f;
	m_vLook = _vec3(2.f,0.f,0.f);
	m_fSpeed = 0.f;

	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = _vec3(0.06f, 0.06f, 0.06f);
	
	m_bDead = true;
	return S_OK;
}

_int CFireArrow::Update(const _float& fTimeDelta)
{
	if(m_bDead)
	{
		m_pvecMonster->clear();
		return 0;
	}

	if(Clear_Render())
		return 0;

	if(m_vPrevPos != _vec3(0,0,0))
	{
		_vec3 vPrevPos,vMyPos;
		vPrevPos = _vec3(m_vPrevPos.x,0.f,m_vPrevPos.z);
		vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);

		_myvec vDir = vPrevPos - vMyPos;

		if(vDir.Length() > 10.f)
		{
			m_vPrevPos = _vec3(0,0,0);
			m_bDead = true;
			m_fShotAcc = 0.f;
			return 0;
		}
	}

	D3DXVec3TransformNormal(&m_pInfo->m_vDir,&m_vLook,&m_pInfo->m_matWorld);

	m_fTimeDelta = fTimeDelta;

	if(m_fTimeSecond > 10.f)
	{
		if(m_bDead == false)
		{
			m_fShotAcc = 0.f;
			m_fTimeSecond = 0.f;
			m_bDead = true;
			return 0;
		}
	}
	else
		m_fTimeSecond += m_fTimeDelta;

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);

	if(m_pRanger == NULL)
		m_pRanger = ((CRanger*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front());


	SetUp();

	if(m_bDead == false)
	{
		switch(m_eActBullet)
		{
		case AB_NORMAL:
			NormalShot();
			Collision_Monster();
			break;
		case AB_WIDTH:
			WidthShot();
			Collision_Width_Monster();
			break;
		case AB_STUN:
			StunShot();
			Collision_Monster();
			break;
		case AB_DOWN:
			DownShot();
			Collision_Monster();
			break;
		case AB_DMG:
			DmgShot();
			break;
		case AB_DEFAULT:
			NormalShot();
			break;
		default:
			__debugbreak();
			break;
		}


		if(m_bEffect_First == false)
		{
			m_eRanger_TrackID = m_pRanger->Get_TrackID();

			switch(m_eRanger_TrackID)
			{
			case CPlayers::ATK_0:
				Engine::Call_RangerFX(Engine::R_DEFAULT,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_DEFAULT;
				break;
			case CPlayers::ATK_1:
				Engine::Call_RangerFX(Engine::R_DEFAULT,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_DEFAULT;
				break;
			case CPlayers::ATK_2:
				Engine::Call_RangerFX(Engine::R_DEFAULT,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_DEFAULT;
				break;
			case CPlayers::SK_0:
				Engine::Call_RangerFX(Engine::R_SKILL1,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_SKILL1;
				break;
			case CPlayers::SK_1:
				Engine::Call_RangerFX(Engine::R_SKILL2,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_SKILL2;
				break;
			case CPlayers::SK_2:/*
				Engine::Call_RangerFX(Engine::R_SKILL3,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_SKILL3;*/
				break;
			case CPlayers::SK_3:
				/*Engine::Call_RangerFX(Engine::R_SKILL4,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_SKILL4;*/
				break;
			case CPlayers::SK_4:
				/*Engine::Call_RangerFX(Engine::R_SKILL5,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_SKILL5;*/
				break;
			case CPlayers::DASH:
				Engine::Call_RangerFX(Engine::R_DASH,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_DASH;
				break;
			case CPlayers::HIDDEN_SKILL:
				Engine::Call_RangerFX(Engine::R_INNATE,&m_pInfo->m_vPos,true,NULL,&m_pInfo->m_fAngle[Engine::ANGLE_Y]);
				m_bEffect_First = true;
				m_wEffectNumber = Engine::R_INNATE;
				break;
			default:
				break;
			}

		}
	}
	else
	{
		m_fShotAcc = 0.f;
	}

	//Collision_Monster();

	//_int iResult = Engine::CGameObject::Update(fTimeDelta);
	return 0;
}

void CFireArrow::Render(void)
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

void CFireArrow::StunShot()
{
	_vec3 vPos = SharePos();

	if(vPos == _vec3(0,0,0))
		return;
}

void CFireArrow::DownShot()
{
	_vec3 vPos = SharePos();

	if(vPos == _vec3(0,0,0))
		return;

	
}

void CFireArrow::DmgShot()
{

}

void CFireArrow::NormalShot()
{
	_vec3 vPos = SharePos();

	if(vPos == _vec3(0,0,0))
		return;
	else
	{

	}
}

void CFireArrow::WidthShot()
{
	_vec3 vDir = m_pInfo->m_vDir;

	D3DXVec3Normalize(&vDir,&vDir);
	m_fShotAcc += 2.f * m_fTimeDelta;
	m_pInfo->m_vPos += vDir * m_fTimeDelta * ((20.f) * m_fShotAcc + m_fShotAcc * 0.2f - 0.2f);
}

_vec3 CFireArrow::SharePos()
{
	_myvec vDir = _vec3(0,0,0); 
	_myvec vTargetPos(m_vTargetPos.x,0.f,m_vTargetPos.z);
	_myvec vArrowPos(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
	_myvec vMyPos(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
	_myvec vMyDir = m_pTransform_Ranger->m_vDir;
	_vec3 vRangerPos(m_pTransform_Ranger->m_vPos.x,0.f,m_pTransform_Ranger->m_vPos.z);

	if((m_pRanger)->Get_TrackID() != CPlayers::SK_0 &&
		(m_pRanger)->Get_TrackID() != CPlayers::SK_1)
	{
		if(m_vTargetPos != _vec3(0,0,0))
		{
			vDir = vTargetPos - vRangerPos;
			_myvec vArrowDir = vTargetPos - vArrowPos;
			m_fDistance = vArrowDir.Length();
			_myvec vNormal(m_pInfo->m_vDir.z,0.f,-m_pInfo->m_vDir.x);
			vNormal.Normalize();
			vDir.Normalize();
			m_vLastDir = vDir; 
			vMyDir.Normalize();
			_float fScala = vDir.Dot(&vNormal);
			_float fDot = vDir.Dot(&vMyDir);
			_float fDotAngle = acosf(vDir.Dot(&vMyDir));
			_float fDegree = (acosf(vDir.Dot(&vMyDir)));
			_float fTemp = m_pInfo->m_fAngle[Engine::ANGLE_Y];

			if(vMyPos.x < vTargetPos.x)
				fDegree = -fDegree;
			else
				fDegree = fDegree;

			if(vArrowDir.Length() > 0.35f)
			{
				if(fDot > 0.99f)
					m_pInfo->m_fAngle[Engine::ANGLE_Y] = m_pTransform_Ranger->m_fAngle[Engine::ANGLE_Y];
				else
				{
					if(m_bTemp == false)
					{
						_float fPlusAngle = D3DXToRadian(60.f) - fDotAngle;

						if(fScala > 0)
						{
							m_pInfo->m_fAngle[Engine::ANGLE_Z] = 0.f;
							m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(120.f - fDotAngle) * m_fTimeDelta;

							if(m_pInfo->m_fAngle[Engine::ANGLE_Y] > D3DXToRadian(180.f))
							{
								m_pInfo->m_fAngle[Engine::ANGLE_Y] = -D3DXToRadian(180.f);
								if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
									m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
							}
							if(m_pInfo->m_fAngle[Engine::ANGLE_Y] < -D3DXToRadian(180.f))
							{
								m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(180.f);
								if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
									m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
							}
						}
						else
						{
							m_pInfo->m_fAngle[Engine::ANGLE_Z] = 0.f;
							m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(120.f - fDotAngle) * m_fTimeDelta;


							if(m_pInfo->m_fAngle[Engine::ANGLE_Y] > D3DXToRadian(180.f))
							{
								m_pInfo->m_fAngle[Engine::ANGLE_Y] = -D3DXToRadian(180.f);
								if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
									m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
							}
							if(m_pInfo->m_fAngle[Engine::ANGLE_Y] < -D3DXToRadian(180.f))
							{
								m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(180.f);
								if(!_finite(m_pInfo->m_fAngle[Engine::ANGLE_Y]))
									m_pInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
							}
						}
					}
				}

				m_fShotAcc += 2.f * m_fTimeDelta;
				m_pInfo->m_vPos += m_vLastDir * m_fTimeDelta * ((20.f) * m_fShotAcc + m_fShotAcc * 0.2f - 0.2f);
				return m_pInfo->m_vPos;
			}
		}
	}

	return _vec3(0,0,0);
}

void CFireArrow::SetUp()
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

void CFireArrow::Collision_Monster()
{
	/*if(iCnt == 1)
		g_Test = true;
	else
	++iCnt;*/

	CPlayers* pPlayer =	((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer();
	if(pPlayer == NULL)
		return;
	float fSoundDistance = abs(D3DXVec3Length(&pPlayer->GetInfo()->m_vPos) - D3DXVec3Length(&m_pInfo->m_vPos));
	float fSoundVolume = -((fSoundDistance/6.f) - 0.5f);

	if(m_fDistance < 0.35f)
	{
		if(m_pTargetMonster != NULL)
		{
			switch(m_eActBullet)
			{
			case AB_NORMAL:
				m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(1),(CUnits*)m_pRanger,Engine::CStatus::DMG_MELEE);
				const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
				Engine::Call_RangerFX(Engine::R_SKILL3_TARGETHIT,&m_pInfo->m_vPos,false);
				if(pPlayer->Get_PlayerJob() == CPlayers::JOB_RANGER)
					CSoundMgr::GetInstance()->PlayRangerSound3(L"RangerAttHit.ogg");
				else
					CSoundMgr::GetInstance()->PlayRangerSound3(L"RangerAttHit.ogg",fSoundVolume);
				break;
			case AB_STUN:
				m_pTargetMonster->Set_bStunState(true);
				m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(1),(CUnits*)m_pRanger,Engine::CStatus::DMG_MELEE);
				const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
				Engine::Call_RangerFX(Engine::R_SKILL3_TARGETHIT,&m_pInfo->m_vPos,false);
				if(pPlayer->Get_PlayerJob() == CPlayers::JOB_RANGER)
					CSoundMgr::GetInstance()->PlayRangerSound3(L"RangerEffectHit3.ogg");
				else
					CSoundMgr::GetInstance()->PlayRangerSound3(L"RangerEffectHit3.ogg",fSoundVolume);
				break;
			case AB_DOWN:
				m_pTargetMonster->Set_bDownState(true);
				m_pTargetMonster->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(1),(CUnits*)m_pRanger,Engine::CStatus::DMG_MELEE);
				const_cast<Engine::CStatus*>(m_pTargetMonster->GetStatus())->Damaged((_float)m_pTargetMonster->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
				Engine::Call_RangerFX(Engine::R_SKILL5_TARGETHIT,&m_pInfo->m_vPos,false);
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)m_pTargetMonster->Get_DMG_Check()));
				break;
			case AB_DMG:
				m_pTargetMonster->Set_bDMG_Timing(true);
				m_pTargetMonster->Set_DMG_Check(100.f,(CUnits*)m_pRanger,Engine::CStatus::DMG_MELEE);
				break;
			case AB_DEFAULT:
				m_pTargetMonster->Set_DMG_Check(100.f,(CUnits*)m_pRanger,Engine::CStatus::DMG_MELEE);
				break;
			default:
				__debugbreak();
				break;
			}
		}

		if(m_bEffect_First == true)
		{
			m_bEffect_First = false;
			//Engine::CEffect_Manager::GetInstance()->Off_RangerFX((Engine::RANGER_FX)m_wEffectNumber);
		}
		
		m_bDead = true;
		m_fShotAcc = 0.f;
		m_fDistance = 0.f;
	}
}

void CFireArrow::Collision_Width_Monster()
{
	CPlayers* pPlayer =	((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->Get_TargetPlayer();
	if(pPlayer == NULL)
		return;
	if(m_pvecMonster == NULL)
		return;

	float fSoundDistance = abs(D3DXVec3Length(&pPlayer->GetInfo()->m_vPos) - D3DXVec3Length(&m_pInfo->m_vPos));
	float fSoundVolume = -((fSoundDistance/6.f) - 0.5f);

	if(((*m_pvecMonster)).size() != 0)
	{
		for(_uint i = 0; i < ((*m_pvecMonster)).size(); ++i)
		{
			if(((CMonsters*)((*m_pvecMonster)[i]))->Get_bDieState())
				continue;

			_myvec vMyPos,vTargetPos,vTargetDir;
			vMyPos = _vec3(m_pInfo->m_vPos.x,0.f,m_pInfo->m_vPos.z);
			vTargetPos =_vec3(const_cast<Engine::CTransform*>(((CMonsters*)(*m_pvecMonster)[i])->GetInfo())->m_vPos.x,0.f,const_cast<Engine::CTransform*>(((CMonsters*)(*m_pvecMonster)[i])->GetInfo())->m_vPos.z);
			vTargetDir = vTargetPos - vMyPos;
			m_vTemp = _vec3(const_cast<Engine::CTransform*>(((CMonsters*)(*m_pvecMonster)[i])->GetInfo())->m_vPos.x,const_cast<Engine::CTransform*>(((CMonsters*)(*m_pvecMonster)[i])->GetInfo())->m_vPos.y + 0.1f,const_cast<Engine::CTransform*>(((CMonsters*)(*m_pvecMonster)[i])->GetInfo())->m_vPos.z);
			_float fDis = vTargetDir.Length();
			
			if(fDis < 0.35f)
			{
				((CMonsters*)(*m_pvecMonster)[i])->Set_DMG_Check((_float)CRandom_Number::Get_RandomNum(1),m_pRanger,Engine::CStatus::DMG_MELEE);
				const_cast<Engine::CStatus*>((((CMonsters*)(*m_pvecMonster)[i]))->GetStatus())->Damaged((((CMonsters*)(*m_pvecMonster)[i]))->Get_DMG_Check(), Engine::CStatus::DMG_MELEE);
				Engine::Call_RangerFX(Engine::R_SKILL3_TARGETHIT,&m_vTemp,true);
				Engine::Get_Scene()->AddObject(LAYER_GAMELOGIC,L"Damage",CDmgView::Create(m_pDevice, &m_pInfo->m_vPos,(_float)CRandom_Number::Get_RandomNum(1)));
				m_bDead = true;
				if(m_bEffect_First == true)
				{
					m_bEffect_First = false;
					//Engine::CEffect_Manager::GetInstance()->Off_RangerFX((Engine::RANGER_FX)m_wEffectNumber);
				}
				
				if(pPlayer->Get_PlayerJob() == CPlayers::JOB_RANGER)
					CSoundMgr::GetInstance()->PlayRangerSound3(L"RangerEffectHit2.ogg", 0.45f);
				else
					CSoundMgr::GetInstance()->PlayRangerSound3(L"RangerEffectHit2.ogg",fSoundVolume);

				break;
			}
		}

	}
	
}
void CFireArrow::Set_MonContainer(vector<CMonsters*>* pvecMon)
{
	m_pvecMonster = pvecMon;
}

HRESULT CFireArrow::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
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

CFireArrow* CFireArrow::Create(LPDIRECT3DDEVICE9 pDevice , WORD wNumber)
{
	CFireArrow*		pGameObject = new CFireArrow(pDevice,wNumber);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CFireArrow::Free(void)
{
	CBullet::Free();
}

void CFireArrow::ArrowTrans(_vec3 vPos,float fSpeed , float fTime ,CMonsters* pTarGetMon,eActBullet ActBullet ,float fAngle /*= 0.f*/)
{
	m_pTargetMonster = pTarGetMon;

	m_eActBullet = ActBullet;
	m_vTargetPos = vPos;
	m_fTimeMax = fTime;
	m_fSpeed = fSpeed;

	if(m_pTransform_Ranger == NULL || m_matHandle == NULL)
	{
		m_pTransform_Ranger = const_cast<Engine::CTransform*>(((CRanger*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front())->GetInfo());
		m_matHandle = const_cast<Engine::CDynamicMesh*>((((CUnits*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Ranger")->front())->GetMesh()))->FindFrame("BN_Weapon_R");
	}
	
	_vec3 vBowPos = ((Engine::CTransform*)((CBow*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Bow")->front())->GetComponent(L"Transform"))->m_vPos;

	m_pInfo->m_vPos = m_pTransform_Ranger->m_vPos;
	m_pInfo->m_vPos.x	+= vBowPos.x * m_pTransform_Ranger->m_vScale.x;
	m_pInfo->m_vPos.y	+= (m_pTransform_Ranger->m_vPos.y + 20.f)* m_pTransform_Ranger->m_vScale.y;

	m_pInfo->m_vPos.z	+= vBowPos.z * m_pTransform_Ranger->m_vScale.z;

	m_pInfo->m_vDir = m_pTransform_Ranger->m_vDir;

	m_pInfo->m_vScale = _vec3(0.01f,0.01f,0.01f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = m_pTransform_Ranger->m_fAngle[Engine::ANGLE_Y];

	_vec3 vAt =	((CDynamicCamera*)Engine::Get_ObjList(LAYER_UI,L"DynamicCamera")->front())->GetAt();
	_vec3 vEye = ((CDynamicCamera*)Engine::Get_ObjList(LAYER_UI,L"DynamicCamera")->front())->GetEye();

	_myvec vDir = m_pTransform_Ranger->m_vDir;
	_myvec vLook = _vec3(vDir.x,0.f,vDir.z);
	vDir.Normalize();
	vLook.Normalize();

	if(fAngle == 0.f)
	{
		m_fRadian = acosf(vDir.Dot(&vLook));

		if(m_pInfo->m_vPos.z > m_pTransform_Ranger->m_vPos.z)
			m_fRadian = -m_fRadian;
		else
			m_fRadian = m_fRadian;	
	}
	else
	{
		m_fRadian = fAngle;
	}


	m_vPrevPos = m_pInfo->m_vPos;
	m_fShotAcc = 0.f;
	m_bDead = false;
}
