#include "stdafx.h"
#include "AIMgr.h"
#include "DynamicMesh.h"
#include "Players.h"
#include "Monsters.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Collision.h"
#include "Stage.h"
#include "Status.h"
#include "NPCs.h"

#include "Skill_Range_1.h"
#include "MonsterDmg.h"
#include "MainPlayer.h"

IMPLEMENT_SINGLETON(CAIMgr)

CAIMgr::CAIMgr(void)
{

}

CAIMgr::~CAIMgr(void)
{

}


void CAIMgr::Look_Target(CUnits* pTargetr, CMonsters* pMonster)
{
	Engine::CTransform* pMonsterInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pMonster->GetComponent(L"Transform")));
	Engine::CTransform* pTargetrInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pTargetr->GetComponent(L"Transform")));

	float fDot = 0.f;
	float fAngle = 0.f;
	_vec3 Target;

	_vec3 Monster = _vec3(pMonsterInfo->m_vPos.x, 0, pMonsterInfo->m_vPos.z);

	// ���� �ڸ��� �����ִٸ� ������ �ڸ� ��������~~
	if(pMonster->Get_AttackPos() == NULL || pMonster->Get_Target_Attack() != false)	
		Target = _vec3(pTargetrInfo->m_vPos.x, 0, pTargetrInfo->m_vPos.z);
	else
		Target = _vec3(pMonster->Get_AttackPos()->vPos.x, 0, pMonster->Get_AttackPos()->vPos.z);


	_vec3 vCrossDir = pMonsterInfo->m_vDir;
	_vec3 vMonsterDir = _vec3(pMonsterInfo->m_vDir.x, 0, pMonsterInfo->m_vDir.z);

	_vec3 vDir = Target - Monster;
	vCrossDir = _vec3(vCrossDir.z, 0.f, -vCrossDir.x);

	D3DXVec3Normalize(&vCrossDir, &vCrossDir);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vMonsterDir, &vMonsterDir);

	fDot = D3DXVec3Dot(&vDir, &vCrossDir);
	fAngle = 0.f;

	fAngle = D3DXVec3Dot(&vDir, &vMonsterDir);
	fAngle = D3DXToDegree(acosf(fAngle));


	//if(fAngle < 5)
	//	return;

	float fTemp = pMonsterInfo->m_fAngle[Engine::ANGLE_Y];	

	if(fDot > 0) 
	{	  
		pMonsterInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(fAngle);
		if(!_finite(pMonsterInfo->m_fAngle[Engine::ANGLE_Y]))
			pMonsterInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}
	else
	{
		pMonsterInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(fAngle);
		if(!_finite(pMonsterInfo->m_fAngle[Engine::ANGLE_Y]))
			pMonsterInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}	 
}


void CAIMgr::Look_OriPosition(CMonsters* pMonster)
{
	Engine::CTransform* pMonsterInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pMonster->GetComponent(L"Transform")));

	float fDot = 0.f;
	float fAngle = 0.f;
	_vec3 Target = pMonster->Get_OriPos();
	 
	_vec3 Monster = _vec3(pMonsterInfo->m_vPos.x, 0, pMonsterInfo->m_vPos.z);
	
	Target = _vec3(Target.x, 0, Target.z);
	 
	_vec3 vCrossDir = pMonsterInfo->m_vDir;
	_vec3 vMonsterDir = _vec3(pMonsterInfo->m_vDir.x, 0, pMonsterInfo->m_vDir.z);

	_vec3 vDir = Target - Monster;
	vCrossDir = _vec3(vCrossDir.z, 0.f, -vCrossDir.x);

	D3DXVec3Normalize(&vCrossDir, &vCrossDir);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vMonsterDir, &vMonsterDir);

	fDot = D3DXVec3Dot(&vDir, &vCrossDir);
	fAngle = 0.f;

	fAngle = D3DXVec3Dot(&vDir, &vMonsterDir);

	fAngle = D3DXToDegree(acosf(fAngle));

	//if(fAngle < 5)
	//	return;

	float fTemp = pMonsterInfo->m_fAngle[Engine::ANGLE_Y];	

	if(fDot > 0) 
	{	  
		pMonsterInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(fAngle);
		if(!_finite(pMonsterInfo->m_fAngle[Engine::ANGLE_Y]))
			pMonsterInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}
	else
	{
		pMonsterInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(fAngle);
		if(!_finite(pMonsterInfo->m_fAngle[Engine::ANGLE_Y]))
			pMonsterInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}	 
}


void CAIMgr::Target_Search(CMonsters* pMonster)
{
	vector<CMonsters*>::iterator iter = m_pMonstersInfo->begin();
	vector<CMonsters*>::iterator iter_end = m_pMonstersInfo->end();

	const _vec3 vInfo = pMonster->GetInfo()->m_vPos;
	CUnits* pUnit = pMonster->Get_ChangeTargetInfo();


	for(; iter != iter_end; ++iter)
	{
		const _vec3 vDist = vInfo - (*iter)->GetInfo()->m_vPos;
		float fDist = D3DXVec3Length(&vDist);

		if(fDist < 3.f)
		{
			(*iter)->Set_TargetInfo(pUnit);
			(*iter)->Set_bSearch(true);
		}
	}
}



void CAIMgr::GetHit_State(CMonsters* pMonster)
{
	_uint fAniType = pMonster->Get_AniIdx();

	
	// ü�� 0 ���ϸ� �״´�
	if(pMonster->GetStatus()->m_tRealStat.m_fCurHp <= 0.f) 
	{	
		if(!pMonster->Get_bBoss_Type())
			pMonster->Set_AniIdx(STATE_DIE);
		else
			pMonster->Set_AniIdx(STATEBOSS_DIE);
			 
		//pMonster->Set_DMG_Check(0.f);
		pMonster->Set_bDieState(true);
		return;
	}

	if(pMonster->Get_DMG_Check() != 0.f && pMonster->Get_ChangeTargetInfo() != NULL)			 
	{
		//pMonster->Set_DMG_Check(0.f);

		if(pMonster->Get_bHitState())
		{		 
			if(!pMonster->Get_bBoss_Type())			// �������� Ÿ�Կ� ���� ó��
				pMonster->Set_AniIdx(STATE_DMG_F);
			else
				pMonster->Set_AniIdx(STATEBOSS_DMG_F);

			pMonster->Set_Att_DelayTime(2.f);

			//pMonster->Set_DMG_Check(0.f);
			return; 

		}

		// �ٿ� ����
		if(pMonster->Get_bDownState())
		{
			// ���ӵ� ������ ó���ϱ� ���� ������

			if(!pMonster->Get_bBoss_Type())
				if(fAniType != STATE_DOWN && fAniType != STATE_DOWN_LOOP && fAniType != STATE_GET_UP)
					pMonster->Set_AniIdx(STATE_DOWN);	
			
			//pMonster->Set_DMG_Check(0.f);
			return;
		}
		// ���� ����
		if(pMonster->Get_bStunState())
		{
			if(!pMonster->Get_bBoss_Type())
				pMonster->Set_AniIdx(STATE_STUN);
			//else
				//pMonster->Set_AniIdx(STATEBOSS_STUN);
			
			//pMonster->Set_DMG_Check(0.f);
			return;
		}
	}
}


void CAIMgr::Monster_Aggro(CMonsters* pMonster, CUnits* pTarget, float fTime)
{
	if(pTarget == NULL)
	{
		pMonster->Set_TargetInfo(pTarget);	
		return;
	}

	if(pMonster->Get_ChangeTargetInfo() != NULL)
	{
		if(!pMonster->Get_AttackEnd())
		{
			// Ÿ�� ����� NPC �� ��� �÷��̾�(����)�� ���ȴٸ� �÷��̾��(����) ���� 
			if(pTarget->Get_UnitType() == CUnits::TYPEID_NPC)
			{
				pTarget = pMonster->Get_ChangeTargetInfo();
				pMonster->Set_ChangeTargetInfo(NULL);
				pMonster->Set_TargetInfo(pTarget);	
				return;
			}

			// ����ġ �ִ� �� �� �ش� 3�ʰ� ����ġ ������ ���� �ʰ� (����, ���� ������ ��Ÿ��)
			// 3�ʰ� �Ѿ�� �ٽ� ����ġ�� ���� ���� �ش� Ÿ���� ����ȴ� 

			float fAggroTime = pMonster->Get_AggroTime();

			if(pMonster->Get_bAggro_Target_Change())
			{
				if(fAggroTime >=  pMonster->Get_TargetFix_Time())
				{
					pMonster->Set_AggroTime(0.f);
					pMonster->Set_TargetInfo(pTarget);
					pMonster->Set_bAggro_Target_Change(false);
					return;
				}
				else
				{
					pMonster->Set_AggroTime(fAggroTime + fTime);
					pMonster->Set_TargetInfo(pTarget);
					return;
				}
			}

			// ������ �ְ� �ִ� ��� ���� ����ġ�� �������� �ִ밡 �� �� Ÿ���� �ٷκ��� �ǰ� 3�ʰ��� ���� �ð��� ����
			CPlayers::PLAYERJOB eJop = ((CPlayers*)pMonster->Get_ChangeTargetInfo())->Get_PlayerJob();
			float fMax_Aggro = pMonster->Get_MAX_Aggro_Value();
			 

			switch(eJop)
			{
			case CPlayers::JOB_GUARDIAN:
				{
					if((*pMonster->Get_Aggro_Accumulate())[0] >= fMax_Aggro)		// ���� �ִ��ġ
					{
						(*pMonster->Get_Aggro_Accumulate())[0] = 0.f;
						pMonster->Set_bAggro_Target_Change(true);
						pMonster->Set_TargetInfo((CPlayers*)pMonster->Get_ChangeTargetInfo());	
					}
					else		
						(*pMonster->Get_Aggro_Accumulate())[0] += 5.f;	   // ���� ��	
				}
				break;
			case CPlayers::JOB_PALADIN:
				{
					if((*pMonster->Get_Aggro_Accumulate())[1] >= fMax_Aggro)		// ���� �ִ��ġ
					{
						(*pMonster->Get_Aggro_Accumulate())[1] = 0.f;
						pMonster->Set_bAggro_Target_Change(true);		
						pMonster->Set_TargetInfo((CPlayers*)pMonster->Get_ChangeTargetInfo());

					}
					else		
						(*pMonster->Get_Aggro_Accumulate())[1] += 5.f;		 // ���� ��
				}
				break;
			case CPlayers::JOB_MAGICIAN:
				{
					if((*pMonster->Get_Aggro_Accumulate())[2] >= fMax_Aggro)		// ���� �ִ��ġ
					{
						(*pMonster->Get_Aggro_Accumulate())[2] = 0.f;
						pMonster->Set_bAggro_Target_Change(true);
						pMonster->Set_TargetInfo((CPlayers*)pMonster->Get_ChangeTargetInfo());	
					}
					else		
						(*pMonster->Get_Aggro_Accumulate())[2] += 2.f;	   // ���� ��	
				}
				break;
			case CPlayers::JOB_RANGER:
				{
					if((*pMonster->Get_Aggro_Accumulate())[3] >= fMax_Aggro)		// ���� �ִ��ġ
					{
						(*pMonster->Get_Aggro_Accumulate())[3] = 0.f;
						pMonster->Set_bAggro_Target_Change(true);
						pMonster->Set_TargetInfo((CPlayers*)pMonster->Get_ChangeTargetInfo());	
					}
					else		
						(*pMonster->Get_Aggro_Accumulate())[3] += 2.f;	   // ���� ��	
				}
				break; 
			}
		}	 
	}
	else
		pMonster->Set_TargetInfo(pTarget);	
}


CUnits*	CAIMgr::Monster_CautionRange_Check(vector<CUnits*>* pTargetInfo, CMonsters* pMonster)
{
	Engine::CTransform* pMonsterInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pMonster->GetComponent(L"Transform")));

	vector<CUnits*>::iterator iter = pTargetInfo->begin();
	vector<CUnits*>::iterator iter_end = pTargetInfo->end();
	
	float M_PDist = 100.f;
	float fDist = 0.f;
	CUnits* pTarget = NULL;

	bool	bAttStyle = false;

	// Ÿ���� ���� ���� �ʴٸ� ����ش�
	if(!pMonster->Get_TargetInfo() != NULL)
	{
		for(; iter != iter_end; ++iter)
		{
			if(!pMonster->Get_bAttackStyle())
				bAttStyle = (*iter)->Get_AttPositionFull();		
			else
				bAttStyle = (*iter)->Get_Long_AttPositionFull();

			// ���� �ش� ���� �ڸ��� ��� ���� �޾Ҵٸ� 
			// ���� ª�� �Ÿ��� �ִ� Ÿ������ ������ ��ġ ������ �ش�
			if(!bAttStyle)
			{
				_vec3 vPlayerPos = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>( (*iter)->GetComponent(L"Transform")))->m_vPos;
				_vec3 vMonsterPos = pMonsterInfo->m_vPos;
				_vec3 vLength = vPlayerPos - vMonsterPos;

				float fLength = D3DXVec3Length(&vLength);

				if((*iter)->Get_UnitType() == CUnits::TYPEID_NPC)
					if((((CNPCs*)(*iter)))->Get_bDieState())
						continue;
						
				if(pMonster->Get_CautionRange() > fLength)
				{
					_vec3 vDist = vPlayerPos - vMonsterPos;
					fDist = D3DXVec3Length(&vDist);

					if(fDist < M_PDist)
					{
						M_PDist = fDist;
						pTarget = (*iter);
					}
				}
			}
			
		}
		return pTarget;
	}
	else	// �ִٸ� �Ÿ� ���
	{	
		if(((CNPCs*)pMonster->Get_TargetInfo())->Get_bDieState())
			return NULL;

		_vec3 vPlayerPos = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pMonster->Get_TargetInfo()->GetComponent(L"Transform")))->m_vPos;
		_vec3 vMonsterPos = pMonsterInfo->m_vPos;
		_vec3 vLength = vPlayerPos - vMonsterPos;

		float fLength = D3DXVec3Length(&vLength);

		if(!pMonster->Get_bSearch())
		{
			if(pMonster->Get_CautionRange() > fLength)
				return pMonster->Get_TargetInfo();
		}
		else
		{
			_vec3 vDist = pMonster->GetInfo()->m_vPos - pMonster->Get_OriPos();
			float fDist = D3DXVec3Length(&vDist);

			if(fDist > 10.f)  // ���� ��ġ���� �ʹ� �־��� ��� �ٽ� ���ƿ´� 
			{
				pMonster->Set_bSearch(false);
				pMonster->Set_ChangeTargetInfo(NULL);
		
				(*pMonster->Get_Aggro_Accumulate())[0] = 0.f;
				(*pMonster->Get_Aggro_Accumulate())[1] = 0.f;
				(*pMonster->Get_Aggro_Accumulate())[2] = 0.f;
				(*pMonster->Get_Aggro_Accumulate())[3] = 0.f;
				return NULL;
			}
			return pMonster->Get_TargetInfo();
		}
			
			
		if(pMonster->Get_TargetInfo() != NULL)
		{
			if(!pMonster->Get_bAttackStyle())
				pMonster->Get_TargetInfo()->Set_AttPositionFull(false);		
			else
				pMonster->Get_TargetInfo()->Set_Long_AttPositionFull(false);
		}	
	}
	return NULL;
}
 
void CAIMgr::Monster_Target_Chase(CMonsters* pMonster, const float& fTimeDelta)
{
	Engine::CTransform* pMonsterInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pMonster->GetComponent(L"Transform")));
	Engine::CTransform* pTargetInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pMonster->Get_TargetInfo()->GetComponent(L"Transform")));
	
	float fTime = pMonster->Get_Start_Chase();
	float fSpeed = pMonster->Get_Speed();

	bool		bAttStyle = false;
	bAttStyle = pMonster->Get_bAttackStyle();
	
	if(!pMonster->Get_bRelocation())
	{
		if(fTime < 1.f)
		{
			pMonster->Set_AniIdx(STATE_IDLE_C);
			pMonster->Set_Start_Chase(fTime + fTimeDelta);
			return;
		}
	}

	if(pMonster->Get_AttackPos() != NULL)
	{
		pMonster->Get_AttackPos()->bPossession = false;
		pMonster->Set_AttackPos(NULL);
	}

	if(pMonster->Get_AttackPos() == NULL)
	{
		vector<Engine::ATTPOSITION*>::iterator iter;   
		vector<Engine::ATTPOSITION*>::iterator iter_end;
		vector<Engine::ATTPOSITION*>::iterator iterTemp;
		
		if(!bAttStyle)
		{
			iter = pMonster->Get_TargetInfo()->Get_AttackPosition()->begin();
			iter_end = pMonster->Get_TargetInfo()->Get_AttackPosition()->end();
			iterTemp = iterTemp = iter_end;	 
		}
		else
		{
			iter = pMonster->Get_TargetInfo()->Get_LongAttackPosition()->begin();
			iter_end = pMonster->Get_TargetInfo()->Get_LongAttackPosition()->end();
			iterTemp = iterTemp = iter_end;
		}
		 
		float fShortDist = 100.f;

		for(; iter != iter_end; ++iter)
		{
			if((*iter)->bPossession == false)
			{
				_vec3 vDist = (*iter)->vPos - pMonsterInfo->m_vPos;

				float fDist = D3DXVec3Length(&vDist);
				if(fDist < fShortDist)
				{
					fShortDist = fDist;
					iterTemp = iter;
				}
			}		 
		}
		 
		if(iter == iter_end)
		{
			if(iterTemp == iter_end)
			{
				if(!bAttStyle)
					pMonster->Get_TargetInfo()->Set_AttPositionFull(true);	
				else
					pMonster->Get_TargetInfo()->Set_Long_AttPositionFull(true);	

				pMonster->Set_TargetInfo(NULL);
				pMonster->Set_TargetInfo(Monster_CautionRange_Check(m_pPlayer_Npc_Info, pMonster));			
				return;
			}
			(*iterTemp)->bPossession = true;
			(*iterTemp)->fAttDist = pMonster->Get_AttackDist();
			pMonster->Set_AttackPos((*iterTemp));
		}
	}

	//_vec3 vTargetPos = pTargetInfo->m_vPos;
	//_vec3 vTargetDir = vTargetPos - pMonsterInfo->m_vPos;
	//float fTargetDist = D3DXVec3Length(&vTargetDir);

	//_vec3 vAttackPos = pMonster->Get_AttackPos()->vPos;
	//_vec3 vDir = vAttackPos - pMonsterInfo->m_vPos;
	//_vec3 vMonsterDir = pMonsterInfo->m_vDir;
	//float fPosiDist = D3DXVec3Length(&vDir);

	_vec3 vTargetPos = pTargetInfo->m_vPos;
	_vec3 vTargetDir = vTargetPos - pMonsterInfo->m_vPos;
	vTargetDir = _vec3(vTargetDir.x, 0, vTargetDir.z);
	float fTargetDist = D3DXVec3Length(&vTargetDir);

	_vec3 vAttackPos = pMonster->Get_AttackPos()->vPos;
	_vec3 vDir = vAttackPos - pMonsterInfo->m_vPos;
	vDir = _vec3(vDir.x, 0, vDir.z);
	_vec3 vMonsterDir = pMonsterInfo->m_vDir;
	vMonsterDir = _vec3(vMonsterDir.x, 0, vMonsterDir.z);
	float fPosiDist = D3DXVec3Length(&vDir);

	 
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vMonsterDir, &vMonsterDir);


	// ������ Ÿ���� �÷��̾��϶��� NPC �϶� �̵� ���ǽ��� �ٸ� 

	if(pMonster->Get_TargetInfo()->Get_UnitType() == CUnits::TYPEID_PLAYER)
	{
		// ���� Ÿ���� �÷��̾��� ��� ó�� 
		if( ((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_F || ((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_FL || 
			((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_B || ((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_FR || 
			((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_L || ((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_BL ||
			((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_R || ((CPlayers*)(pMonster->Get_TargetInfo()))->Get_TrackID() == CPlayers::RUN_BR)
		{
			if(fTargetDist > pMonster->Get_CollisionRange())
			{
				pMonster->Set_Target_Attack(true);

				pMonster->Set_bAttack(false);
				pMonster->Set_bRelocation(false);

				vTargetDir = _vec3(vTargetDir.x, 0, vTargetDir.z);

				pMonster->Set_DwIndex(Engine::MoveOnNaviMesh(&pMonsterInfo->m_vPos, &(vTargetDir * fTimeDelta), pMonster->Get_DwIndex(), 0));    
				//pMonsterInfo->m_vPos += vTargetDir * fTimeDelta; 
			}
			else
			{
				pMonster->Set_Target_Attack(true);
				pMonster->Set_bAttack(true);
				pMonster->Set_bRelocation(true);
				pMonster->Set_Att_DelayTime(2.f);
				return;
			} 
		}
		else
		{
			if(fPosiDist > 0.1f)
			{ 
				if(!pMonster->Get_bAttackStyle())
				{
					if(fTargetDist > pMonster->Get_CollisionRange())
					{		 
						pMonster->Set_Target_Attack(false);
						pMonster->Set_bAttack(false);
						vDir = _vec3(vDir.x, 0, vDir.z);
						pMonster->Set_DwIndex(Engine::MoveOnNaviMesh(&pMonsterInfo->m_vPos, &(vDir * fTimeDelta), pMonster->Get_DwIndex(), 0));    
						//pMonsterInfo->m_vPos += vDir * fTimeDelta; 
					}
					else
					{
						_vec3 vTargetDir = pTargetInfo->m_vDir;
						_vec3 vAttDir = vAttackPos - pTargetInfo->m_vPos;

						D3DXVec3Normalize(&vTargetDir, &vTargetDir);
						D3DXVec3Normalize(&vAttDir, &vAttDir);
						
						float fAngle = 0.f;
						fAngle = D3DXVec3Dot(&vTargetDir, &vAttDir);
						fAngle = D3DXToDegree(acosf(fAngle));

						if(fAngle <= 50.f)
						{	
							pMonster->Get_TargetInfo()->Set_AttPositionFull(false);		

							pMonster->Get_AttackPos()->bPossession = false;

							pMonster->Set_Target_Attack(true);
							pMonster->Set_bAttack(true);
							pMonster->Set_bRelocation(false);
							pMonster->Set_Att_DelayTime(2.f);
							return;
						}
						else
						{
							pMonster->Set_Target_Attack(false);
							pMonster->Set_bAttack(false);
							vDir = _vec3(vDir.x, 0, vDir.z);
							pMonster->Set_DwIndex(Engine::MoveOnNaviMesh(&pMonsterInfo->m_vPos, &(vDir * fTimeDelta), pMonster->Get_DwIndex(), 0));    
							//pMonsterInfo->m_vPos += vDir * fTimeDelta * fSpeed; 
						}
					}
				}
				else
				{
					if(pMonster->Get_AttackDist() >= fTargetDist)
					{
						if(!pMonster->Get_bAttackStyle())
							pMonster->Get_TargetInfo()->Set_AttPositionFull(false);		
						else
							pMonster->Get_TargetInfo()->Set_Long_AttPositionFull(false);

						pMonster->Get_AttackPos()->bPossession = false;
						
						pMonster->Set_Target_Attack(true);
						pMonster->Set_bAttack(true);
						pMonster->Set_bRelocation(false);
						pMonster->Set_Att_DelayTime(2.f);
						return;
					}
					else
					{
						pMonster->Set_Target_Attack(false);
						pMonster->Set_bAttack(false);
						vDir = _vec3(vDir.x, 0, vDir.z);
						pMonster->Set_DwIndex(Engine::MoveOnNaviMesh(&pMonsterInfo->m_vPos, &(vDir * fTimeDelta), pMonster->Get_DwIndex(), 0));    
						//pMonsterInfo->m_vPos += vDir * fTimeDelta; 
					}
				}	 
			}
			else
			{ 
				pMonster->Get_AttackPos()->bPossession = true;
				pMonster->Set_Target_Attack(true);
				pMonster->Set_bAttack(true);
				pMonster->Set_bRelocation(false);
				pMonster->Set_Att_DelayTime(2.f);
				return;
			}	
		}
	}
	else	// NPC�� ��� ó��
	{
		if(fPosiDist > 0.1f)
		{ 
			if(fTargetDist > pMonster->Get_CollisionRange())
			{
				pMonster->Set_Target_Attack(false);
				pMonster->Set_bAttack(false);
				vDir = _vec3(vDir.x, 0, vDir.z);
				pMonsterInfo->m_vPos += vDir * fTimeDelta;
			}
			else
			{
				_vec3 vTargetDir = pTargetInfo->m_vDir;
				_vec3 vAttDir = vAttackPos - pTargetInfo->m_vPos;

				D3DXVec3Normalize(&vTargetDir, &vTargetDir);
				D3DXVec3Normalize(&vAttDir, &vAttDir);

				float fAngle = 0.f;
				fAngle = D3DXVec3Dot(&vTargetDir, &vAttDir);
				fAngle = D3DXToDegree(acosf(fAngle));

				if(fAngle <= 50.f)
				{	
					pMonster->Get_TargetInfo()->Set_AttPositionFull(false);		

					pMonster->Get_AttackPos()->bPossession = false;

					pMonster->Set_Target_Attack(true);
					pMonster->Set_bAttack(true);
					pMonster->Set_bRelocation(false);
					pMonster->Set_Att_DelayTime(2.f);
					return;
				}
				else
				{
					pMonster->Set_Target_Attack(false);
					pMonster->Set_bAttack(false);
					vDir = _vec3(vDir.x, 0, vDir.z);
					pMonsterInfo->m_vPos += vDir * fTimeDelta * fSpeed; 
				}
			}	 
		}
		else
		{ 
			pMonster->Set_Target_Attack(true);
			pMonster->Set_bAttack(true);
			pMonster->Set_bRelocation(false);
			pMonster->Set_Att_DelayTime(2.f);
			return;
		}	
	}

	// �޸��� �ൿ���� ����
	pMonster->Set_AniIdx(STATE_RUN);
	pMonster->Set_AttackEnd(false);
	pMonster->Set_Att_DelayTime(2.f);
}

void CAIMgr::Monster_AttackTiming(CMonsters* pMonster)
{
	//if(pMonster->Get_TargetInfo()->Get_UnitType() == CUnits::TYPEID_NPC)
	//	return;

	_uint fAniType = pMonster->Get_AniIdx(); 

	DOUBLE fTrack = const_cast<Engine::CDynamicMesh*>(pMonster->GetMesh())->GetTrackPosition();
	 
	// !! ������ �������� �� �ȵ��´�
	if(pMonster->Get_AniIdx() == STATE_ATK_1 || pMonster->Get_AniIdx() == STATE_ATK_2 || pMonster->Get_AniIdx() == STATE_SK_1)
	{
		switch(fAniType)
		{
		case STATE_ATK_1:
			{
				float fTiming_1 = (*pMonster->Get_Att_DMG_Timing())[0]->fInterval_1;
				float fTiming_2 = (*pMonster->Get_Att_DMG_Timing())[0]->fInterval_2;

				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					if(!pMonster->Get_bDMG_Timing())
					{
						pMonster->Set_bDMG_Timing(true);
						float fDamege = (_float)CRandom_Number::Get_RandomNum(7,2);
						
						Engine::CGameObject* pGameObject = NULL;
						if(pMonster->Get_TargetInfo()->Get_UnitType() == CUnits::TYPEID_PLAYER)
						{
							((CPlayers*)(pMonster->Get_TargetInfo()))->Set_DamageType((CPlayers::ACT_STATE)pMonster->Get_Player_ACT_Type(), fDamege, Engine::CStatus::DMG_MELEE);		
						}
						else
						{	
							if(!(((CNPCs*)(pMonster->Get_TargetInfo())))->m_bStage2)
								const_cast<Engine::CStatus*>(((CNPCs*)(pMonster->Get_TargetInfo()))->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
							
							
							//pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(pMonster->Get_TargetInfo()))->GetInfo()->m_vPos, fDamege);
							//Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);							
						}
					}
				}
			}	
			break;

		case STATE_ATK_2:
			{
				float fTiming_1 = (*pMonster->Get_Att_DMG_Timing())[1]->fInterval_1;
				float fTiming_2 = (*pMonster->Get_Att_DMG_Timing())[1]->fInterval_2;
				
				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					if(!pMonster->Get_bDMG_Timing())
					{
						pMonster->Set_bDMG_Timing(true);
						float fDamege = (_float)CRandom_Number::Get_RandomNum(6,2);

						Engine::CGameObject* pGameObject = NULL;
						if(pMonster->Get_TargetInfo()->Get_UnitType() == CUnits::TYPEID_PLAYER)
						{
							((CPlayers*)(pMonster->Get_TargetInfo()))->Set_DamageType((CPlayers::ACT_STATE)pMonster->Get_Player_ACT_Type(), fDamege, Engine::CStatus::DMG_MELEE);
						}
						else
						{	
							if(!(((CNPCs*)(pMonster->Get_TargetInfo())))->m_bStage2)
								const_cast<Engine::CStatus*>(((CNPCs*)(pMonster->Get_TargetInfo()))->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
							//pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(pMonster->Get_TargetInfo()))->GetInfo()->m_vPos, fDamege);
							//Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);			
						}	
					}	
				}
			}
			break;

		case STATE_SK_1:
			break;
		}	 
	}
}

 
void CAIMgr::Monster_Pattern(vector<CPlayers*>* pPlatyersInfo, CMonsters* pMonster, CCollision* pCollision, const float& fTimeDelta)
{
	// ���� �� ���� ó�� 
	GetHit_State(pMonster);
	
	_uint fAniType = pMonster->Get_AniIdx();
	
	if(fAniType == STATE_DMG_F || fAniType == STATE_DOWN || fAniType == STATE_DOWN_LOOP || fAniType == STATE_GET_UP || fAniType == STATE_STUN || fAniType == STATE_DIE)
	{ 
		Change_State(pMonster);		// �´� �����̳� ������ ������
		return;
	}
	
	CUnits* pTargetInfo = NULL;
	Engine::CTransform* pMonsterInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pMonster->GetComponent(L"Transform")));
	 
	pTargetInfo = Monster_CautionRange_Check(m_pPlayer_Npc_Info, pMonster);		
	
	//// ���� ���� �� Ÿ���� ����� �����ؾ� �Ѵ�  NPC->���� ���� ��  ���� ����
	//										//   ���� �÷��̾ ������ ���� �� ���� Ȯ��?? ����ġ ??  �� ���� ���� 


	Monster_Aggro(pMonster, pTargetInfo, fTimeDelta);


	vector<CMonsters*>::iterator iterMon = m_pMonstersInfo->begin();
	vector<CMonsters*>::iterator iterMon_end = m_pMonstersInfo->end();
	for(; iterMon != iterMon_end; ++iterMon)
	{
		Engine::CTransform* pInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>((*iterMon)->GetComponent(L"Transform")));

		pCollision->CollisionSphere(pMonsterInfo, pInfo);
	}

	
	if(pMonster->Get_TargetInfo() != NULL)
	{
		//Look_Target(pTargetInfo, pMonster);

		if(pMonster->Get_AttackPos() != NULL)	// �Ҵ�� �ڸ��� ���ٸ� ���� �ް� �̵� 
		{	
			// ������ �ڸ��� ���� �� ����  

			pMonster->Skill_State(fTimeDelta);

			if(pMonster->Get_bSkillState())
			{
				pMonster->Set_bRelocation(false);
				Change_State(pMonster);
				return;
			}

			if(pMonster->Get_bAttack())
			{	
				//if(!pMonster->Get_AttackPos()->bPossession)
				//	pMonster->Get_AttackPos()->bPossession = true;

				// ������ ó��
				Monster_AttackTiming(pMonster);
				
				if(!pMonster->Get_AttackEnd())
				{	 
					if(pMonster->Get_Att_DelayTime() >= 2.f)			// Attack Delay
					{
						Look_Target(pTargetInfo, pMonster);

						if(!pMonster->Get_bAttackStyle())				// ���ݸ�� �ε��� �ο�	
							pMonster->Set_AniIdx((_uint)(rand() % 2 + 4));
						
						else
							pMonster->Set_AniIdx(STATE_ATK_2);

						pMonster->Set_AttackEnd(true);
						pMonster->Set_Att_DelayTime(0.f);
					}
					else
					{
						pMonster->Set_AniIdx(STATE_IDLE_C);
						float fTime = pMonster->Get_Att_DelayTime() + fTimeDelta;
						pMonster->Set_Att_DelayTime(fTime);
					}	 
				}
				pMonster->Set_Start_Chase(0.f);	
			}
			else   // �ƴ϶�� �̵�
			{
				if(!pMonster->Get_AttackEnd())
					Look_Target(pTargetInfo, pMonster);

				Monster_Target_Chase(pMonster, fTimeDelta);
			}
		}
		else
		{
			Look_Target(pTargetInfo, pMonster);
			Monster_Target_Chase(pMonster, fTimeDelta);	
		}
	}
	else	// �� ������ Idle ������
	{
		if(pMonster->Get_ChangeTargetInfo() != NULL)
		{	 
			Target_Search(pMonster);
		}
		else
		{
			if(pMonster->Get_AttackPos() != NULL)
			{	 
				pMonster->Get_AttackPos()->bPossession = false;
				pMonster->Set_AttackPos(NULL);
			}

			_vec3 vDist = pMonster->Get_OriPos() - pMonsterInfo->m_vPos;
			vDist = _vec3(vDist.x, 0.f, vDist.z);
			_float fDist = D3DXVec3Length(&vDist);	  
			D3DXVec3Normalize(&vDist, &vDist);

			if(fDist > 0.1f)		// ���� �ڸ��� ���ư���
			{
				Look_OriPosition(pMonster);
				pMonster->Set_AniIdx(STATE_RUN);
				pMonsterInfo->m_vPos += vDist * fTimeDelta;
			}		 
			else
			{
				pMonsterInfo->m_fAngle[Engine::ANGLE_Y] = pMonster->Get_OriAngle();
				pMonster->Set_AniIdx(STATE_IDLE_P);
			}
		}

		pMonster->Set_Start_Chase(0.f);
		pMonster->Set_Att_DelayTime(0.f);
	}	

	Change_State(pMonster);
}

CUnits*	CAIMgr::BOSS_CautionRange_Check(vector<CUnits*>* pTargetInfo, CMonsters* pBoss)
{
	Engine::CTransform* pMonsterInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pBoss->GetComponent(L"Transform")));

	vector<CUnits*>::iterator iter = pTargetInfo->begin();
	vector<CUnits*>::iterator iter_end = pTargetInfo->end();

	float M_PDist = 100.f;
	float fDist = 0.f;
	CUnits* pTarget = NULL;

	// Ÿ���� ���� ���� �ʴٸ� ����ش�
	if(!pBoss->Get_TargetInfo() != NULL)
	{
		for(; iter != iter_end; ++iter)
		{	
			_vec3 vPlayerPos = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>( (*iter)->GetComponent(L"Transform")))->m_vPos;
			_vec3 vMonsterPos = pMonsterInfo->m_vPos;
			_vec3 vLength = vPlayerPos - vMonsterPos;

			float fLength = D3DXVec3Length(&vLength);

			if(pBoss->Get_CautionRange() > fLength)
			{
				_vec3 vDist = vPlayerPos - vMonsterPos;
				fDist = D3DXVec3Length(&vDist);

				if(fDist < M_PDist)
				{
					M_PDist = fDist;
					pTarget = (*iter);
				}
			}	
		}
		return pTarget;
	}
	else	// �ִٸ� �Ÿ� ���
	{		
		_vec3 vPlayerPos = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pBoss->Get_TargetInfo()->GetComponent(L"Transform")))->m_vPos;
		_vec3 vMonsterPos = pMonsterInfo->m_vPos;
		_vec3 vLength = vPlayerPos - vMonsterPos;

		float fLength = D3DXVec3Length(&vLength);

		if(pBoss->Get_CautionRange() > fLength)
			return pBoss->Get_TargetInfo();
	}
	return NULL;
}

 

void CAIMgr::BOSS_Target_Chase(CMonsters* pBoss, const float& fTimeDelta)
{
	Engine::CTransform* pBossInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pBoss->GetComponent(L"Transform")));
	Engine::CTransform* pTargetInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pBoss->Get_TargetInfo()->GetComponent(L"Transform")));

	float fTime = pBoss->Get_Start_Chase();

	if(!pBoss->Get_bRelocation())
	{
		if(fTime < 0.4f)
		{
			pBoss->Set_AniIdx(STATEBOSS_IDLE_C);
			pBoss->Set_Start_Chase(fTime + fTimeDelta);
			return;
		}
	}

	_vec3 vTargetPos = pTargetInfo->m_vPos;
	_vec3 vTargetDir = vTargetPos - pBossInfo->m_vPos;
	vTargetDir = _vec3(vTargetDir.x, 0, vTargetDir.z);
	float fTargetDist = D3DXVec3Length(&vTargetDir);

	D3DXVec3Normalize(&vTargetDir, &vTargetDir);

	if(fTargetDist > pBoss->Get_CollisionRange())
	{
		pBoss->Set_Target_Attack(true);

		pBoss->Set_bAttack(false);
		pBoss->Set_bRelocation(false);

		pBoss->Set_bSkillState(false);

		vTargetDir = _vec3(vTargetDir.x, 0, vTargetDir.z);

		pBoss->Set_DwIndex(Engine::MoveOnNaviMesh(&pBossInfo->m_vPos, &(vTargetDir * fTimeDelta), pBoss->Get_DwIndex(), 0));    
		//pBossInfo->m_vPos += vTargetDir * fTimeDelta; 
	}
	else
	{
		pBoss->Set_Target_Attack(true);
		pBoss->Set_bAttack(true);
		pBoss->Set_bRelocation(true);
		return;
	} 

	// �޸��� �ൿ���� ����
	pBoss->Set_AniIdx(STATEBOSS_RUN);
	pBoss->Set_AttackEnd(false);
	pBoss->Set_Att_DelayTime(2.f);
}


void CAIMgr::BOSS_AttackTiming(CMonsters* pBoss)
{
	if(pBoss->Get_TargetInfo()->Get_UnitType() == CUnits::TYPEID_NPC)
		return;
	_uint fAniType = pBoss->Get_AniIdx(); 

	DOUBLE fTrack = const_cast<Engine::CDynamicMesh*>(pBoss->GetMesh())->GetTrackPosition();

	// !! ������ �������� �� �ȵ��´�
	if(pBoss->Get_AniIdx() == STATEBOSS_ATK_1 || pBoss->Get_AniIdx() == STATEBOSS_ATK_2 || pBoss->Get_AniIdx() == STATEBOSS_SK_1 || 
																						   pBoss->Get_AniIdx() == STATEBOSS_SK_2 || 
																						   pBoss->Get_AniIdx() == STATEBOSS_SK_3 || 
																						   pBoss->Get_AniIdx() == STATEBOSS_SK_4 || 
																						   pBoss->Get_AniIdx() == STATEBOSS_SK_5 || 
																						   pBoss->Get_AniIdx() == STATEBOSS_SK_6 || 
																				   pBoss->Get_AniIdx() == STATEBOSS_SK_7)
	{
		// �⺻ ���� ��ġ�� ������ ���Ͱ� ������ �ִ� �������� �ؾ� �ҵ�
		switch(fAniType)
		{
		case STATEBOSS_ATK_1:
			{
				float fTiming_1 = (*pBoss->Get_Att_DMG_Timing())[0]->fInterval_1;
				float fTiming_2 = (*pBoss->Get_Att_DMG_Timing())[0]->fInterval_2;
				
				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					if(!pBoss->Get_bDMG_Timing())
					{
						float fDamege = (_float)CRandom_Number::Get_RandomNum(9,2);
						pBoss->Set_bDMG_Timing(true);
						Engine::CGameObject* pGameObject = NULL;

						if(pBoss->Get_TargetInfo()->Get_UnitType() == CUnits::TYPEID_PLAYER)
						{
							((CPlayers*)(pBoss->Get_TargetInfo()))->Set_DamageType((CPlayers::ACT_STATE)pBoss->Get_Player_ACT_Type(), fDamege, Engine::CStatus::DMG_MELEE);			
						}
						else
						{
							//const_cast<Engine::CStatus*>(((CNPCs*)(pBoss->Get_TargetInfo()))->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
							//pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(pBoss->Get_TargetInfo()))->GetInfo()->m_vPos, fDamege);
							//Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);	
						}
					}
				}
			}	
			break;

		case STATEBOSS_ATK_2:
			{
				float fTiming_1 = (*pBoss->Get_Att_DMG_Timing())[1]->fInterval_1;
				float fTiming_2 = (*pBoss->Get_Att_DMG_Timing())[1]->fInterval_2;

				if(fTrack > fTiming_1 && fTrack < fTiming_2)
				{
					if(!pBoss->Get_bDMG_Timing())
					{
						float fDamege = (_float)CRandom_Number::Get_RandomNum(1,3);
						pBoss->Set_bDMG_Timing(true);
						Engine::CGameObject* pGameObject = NULL;

						if(pBoss->Get_TargetInfo()->Get_UnitType() == CUnits::TYPEID_PLAYER)
						{
							((CPlayers*)(pBoss->Get_TargetInfo()))->Set_DamageType((CPlayers::ACT_STATE)pBoss->Get_Player_ACT_Type(), fDamege, Engine::CStatus::DMG_MELEE);			
						}
						else
						{
							//const_cast<Engine::CStatus*>(((CNPCs*)(pBoss->Get_TargetInfo()))->GetStatus())->Damaged(fDamege, Engine::CStatus::DMG_MELEE);
							//pGameObject = CMonsterDmg::Create(Engine::Get_GraphicDev(), &((CNPCs*)(pBoss->Get_TargetInfo()))->GetInfo()->m_vPos, fDamege);
							//Engine::Get_Scene()->AddObject(LAYER_UI, L"MonsterDamage", pGameObject);	
						}
					}
				}
			}
			break;

		case STATEBOSS_SK_1:
			{
			}
			break;
		case STATEBOSS_SK_2:
			{
			}
			break;
		case STATEBOSS_SK_3:
			{
			}
			break;
		case STATEBOSS_SK_4:
			{
			}
			break;
		case STATEBOSS_SK_5:
			break;
		case STATEBOSS_SK_6:
			break;
		case STATEBOSS_SK_7:
			break;
		}	 
	}
}

void CAIMgr::BOSS_Pattern(vector<CPlayers*>* pPlatyersInfo, CMonsters* pBoss, CCollision* pCollision,const float& fTimeDelta)
{
	_uint fAniType = pBoss->Get_AniIdx();

	// ���� �� ���� ó�� 
	GetHit_State(pBoss);

	if(fAniType == STATEBOSS_STUN || fAniType == STATEBOSS_DMG_F || fAniType == STATEBOSS_DIE)
	{
		if(pBoss->Get_Skill_Range_1() != NULL)
			pBoss->Get_Skill_Range_1()->Set_bCancle(true);
		pBoss->Set_bSkill_Range(false);

		if(pBoss->Get_CoolTime_FullIndex() != 7)
			pBoss->Full_SkillCoolTime_Reset(pBoss->Get_CoolTime_FullIndex());

		BOSS_Change_State(pBoss);		// �´� �����̳� ������ ������
		return;
	}

	CUnits* pTargetInfo = NULL;
	Engine::CTransform* pBossInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pBoss->GetComponent(L"Transform")));

	pTargetInfo = BOSS_CautionRange_Check(m_pPlayer_Npc_Info, pBoss);		
	pBoss->Set_TargetInfo(pTargetInfo);

	Monster_Aggro(pBoss, pTargetInfo, fTimeDelta);

	if(pBoss->Get_TargetInfo() != NULL)
	{
		pBoss->Skill_State(fTimeDelta);

		if(!pBoss->Get_bSkillState())
		{
			if(pBoss->Get_bAttack())
			{
				if(!pBoss->Get_AttackEnd())
				{	 		 
					if(pBoss->Get_Att_DelayTime() >= 2.f)			// Attack Delay
					{
						pBoss->Set_AniIdx((_uint)(rand() % 2 + 4));	 // ���ݸ�� �ε��� �ο� 
						 
						pBoss->Set_AttackEnd(true);
						pBoss->Set_Att_DelayTime(0.f);
					}
					else
					{
						pBoss->Set_AniIdx(STATEBOSS_IDLE_C);
						float fTime = pBoss->Get_Att_DelayTime() + fTimeDelta;
						pBoss->Set_Att_DelayTime(fTime);
					}	 
				}
				pBoss->Set_Start_Chase(0.f);	
			}
			else   // �ƴ϶�� �̵�
			{
				Look_Target(pTargetInfo, pBoss);	
				BOSS_Target_Chase(pBoss, fTimeDelta);
			}
		}
		else
		{
			// ��ų ����� ���� �� ��񵿾��� ��� ���·� ���� �� ���� ���� ����  
			if(pBoss->Get_SkillEnd())
			{
				_float fTime = pBoss->Get_fNext_DelayTime();
				if(fTime > 0.5f)
				{
					pBoss->Set_fNext_DelayTime(0.f);
					pBoss->Set_bSkillState(false);
					pBoss->Set_SkillEnd(false);

					// �÷��̾�� ��ų ������ ���� ���ϱ� ���
					vector<CUnits*>::iterator iterInfo = m_pPlayer_Npc_Info->begin();
					vector<CUnits*>::iterator iterInfo_end = m_pPlayer_Npc_Info->end();

					for(; iterInfo != iterInfo_end; ++iterInfo)
					{
						if((*iterInfo)->Get_UnitType() == CUnits::TYPEID_PLAYER)
							((CPlayers*)(*iterInfo))->Set_RangeSkill_Check(_vec3(0.f, 0.f, 0.f) , false, 0.f);
					}
				}
				else
				{
					pBoss->Set_fNext_DelayTime(fTime + fTimeDelta);
					pBoss->Set_AniIdx(STATEBOSS_IDLE_C);
				}
			}		 
		}
		// ������ ó��
		BOSS_AttackTiming(pBoss);
	}
	else
	{ 
		_vec3 vDist = pBoss->Get_OriPos() - pBossInfo->m_vPos;
		vDist = _vec3(vDist.x, 0.f, vDist.z);
		_float fDist = D3DXVec3Length(&vDist);	  
		D3DXVec3Normalize(&vDist, &vDist);

		if(fDist > 0.1f)		// ���� �ڸ��� ���ư���
		{
			Look_OriPosition(pBoss);
			pBoss->Set_AniIdx(STATEBOSS_RUN);
			pBoss->Set_DwIndex(Engine::MoveOnNaviMesh(&pBossInfo->m_vPos, &(vDist * fTimeDelta), pBoss->Get_DwIndex(), 0));    
			//pMonsterInfo->m_vPos += vDist * fTimeDelta;
		}		 
		else
			pBoss->Set_AniIdx(STATEBOSS_IDLE_P);

		pBoss->Set_Start_Chase(0.f);
		pBoss->Set_Att_DelayTime(0.f);
	}	


	BOSS_Change_State(pBoss);
}

CUnits* CAIMgr::NPC_CautionRange_Check(vector<CMonsters*>* pTargetInfo, CNPCs* pNpc)
{
	Engine::CTransform* pNPCInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pNpc->GetComponent(L"Transform")));

	vector<CMonsters*>::iterator iter = pTargetInfo->begin();
	vector<CMonsters*>::iterator iter_end = pTargetInfo->end();

	float M_PDist = 100.f;
	float fDist = 0.f;
	CUnits* pTarget = NULL;

	bool	bAttStyle = false;

	// Ÿ���� ���� ���� �ʴٸ� ����ش�
	if(!pNpc->Get_TargetInfo() != NULL)
	{
		for(; iter != iter_end; ++iter)
		{
			if(!pNpc->Get_bAttackStyle())
				bAttStyle = (*iter)->Get_AttPositionFull();		
			else
				bAttStyle = (*iter)->Get_Long_AttPositionFull();

				
			if(!bAttStyle)
			{
				_vec3 vTargetPos = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>( (*iter)->GetComponent(L"Transform")))->m_vPos;
				_vec3 vNpcPos = pNPCInfo->m_vPos;
				_vec3 vLength = vTargetPos - vNpcPos;

				float fLength = D3DXVec3Length(&vLength);

				if(((CMonsters*)(*iter))->Get_bDieState())
					continue;

				if((*iter)->Get_AniIdx() != STATE_DIE)
				{
					if(pNpc->Get_CautionRange() > fLength)
					{
						_vec3 vDist = vTargetPos - vNpcPos;
						fDist = D3DXVec3Length(&vDist);

						if(fDist < M_PDist)
						{
							M_PDist = fDist;
							pTarget = (*iter);
						}
					}
				}			 
			}
		}

		return pTarget;
	}
	else	// �ִٸ� �Ÿ� ���
	{ 
		if(((CMonsters*)pNpc->Get_TargetInfo())->Get_bDieState())
			return NULL;

		 
		_vec3 vTargetPos = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pNpc->Get_TargetInfo()->GetComponent(L"Transform")))->m_vPos;
		_vec3 vNpcPos = pNPCInfo->m_vPos;
		_vec3 vLength = vTargetPos - vNpcPos;

		float fLength = D3DXVec3Length(&vLength);

		if(pNpc->Get_CautionRange() > fLength)
			return pNpc->Get_TargetInfo();

		if(pNpc->Get_TargetInfo() != NULL)
		{
			if(!pNpc->Get_bAttackStyle())
				pNpc->Get_TargetInfo()->Set_AttPositionFull(false);		
			else
				pNpc->Get_TargetInfo()->Set_Long_AttPositionFull(false);
		}
	}
	return NULL;
}

void CAIMgr::NPC_Look_Target(CUnits* pTarget, CNPCs* pNPC)
{
	Engine::CTransform* pNPCInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pNPC->GetComponent(L"Transform")));
	Engine::CTransform* pTargetrInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pTarget->GetComponent(L"Transform")));

	float fDot = 0.f;
	float fAngle = 0.f;
	_vec3 Target;

	_vec3 vNPC = _vec3(pNPCInfo->m_vPos.x, 0, pNPCInfo->m_vPos.z);

	// ���� �ڸ��� �����ִٸ� ������ �ڸ� ��������~~
	if(pNPC->Get_AttackPos() == NULL || pNPC->Get_Target_Attack() != false)	
		Target = _vec3(pTargetrInfo->m_vPos.x, 0, pTargetrInfo->m_vPos.z);
	else
		Target = _vec3(pNPC->Get_AttackPos()->vPos.x, 0, pNPC->Get_AttackPos()->vPos.z);


	_vec3 vCrossDir = pNPCInfo->m_vDir;
	_vec3 vNPCDir = _vec3(pNPCInfo->m_vDir.x, 0, pNPCInfo->m_vDir.z);

	_vec3 vDir = Target - vNPC;
	vCrossDir = _vec3(vCrossDir.z, 0.f, -vCrossDir.x);

	D3DXVec3Normalize(&vCrossDir, &vCrossDir);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vNPCDir, &vNPCDir);

	fDot = D3DXVec3Dot(&vDir, &vCrossDir);
	fAngle = 0.f;

	fAngle = D3DXVec3Dot(&vDir, &vNPCDir);
	fAngle = D3DXToDegree(acosf(fAngle));


	//if(fAngle < 5)
	//	return;

	float fTemp = pNPCInfo->m_fAngle[Engine::ANGLE_Y];	

	if(fDot > 0) 
	{	  
		pNPCInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(fAngle);
		if(!_finite(pNPCInfo->m_fAngle[Engine::ANGLE_Y]))
			pNPCInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}
	else
	{
		pNPCInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(fAngle);
		if(!_finite(pNPCInfo->m_fAngle[Engine::ANGLE_Y]))
			pNPCInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}	 
}

void CAIMgr::NPC_Look_OriPosition(CNPCs* pNPC)
{
	Engine::CTransform* pNPCInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pNPC->GetComponent(L"Transform")));

	float fDot = 0.f;
	float fAngle = 0.f;
	_vec3 vTarget;	

	if(!pNPC->Get_bDungeonNPC())
		vTarget = pNPC->Get_OriPos();
	else
		vTarget = pNPC->Get_MainPlayer()->GetInfo()->m_vPos;


	_vec3 vNPC = _vec3(pNPCInfo->m_vPos.x, 0, pNPCInfo->m_vPos.z);

	vTarget = _vec3(vTarget.x, 0, vTarget.z);

	_vec3 vCrossDir = pNPCInfo->m_vDir;
	_vec3 vNPCDir = _vec3(pNPCInfo->m_vDir.x, 0, pNPCInfo->m_vDir.z);

	_vec3 vDir = vTarget - vNPC;
	vCrossDir = _vec3(vCrossDir.z, 0.f, -vCrossDir.x);

	D3DXVec3Normalize(&vCrossDir, &vCrossDir);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vNPCDir, &vNPCDir);

	fDot = D3DXVec3Dot(&vDir, &vCrossDir);
	fAngle = 0.f;

	fAngle = D3DXVec3Dot(&vDir, &vNPCDir);

	fAngle = D3DXToDegree(acosf(fAngle));

	//if(fAngle < 5)
	//	return;

	float fTemp = pNPCInfo->m_fAngle[Engine::ANGLE_Y];	

	if(fDot > 0) 
	{	  
		pNPCInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(fAngle);
		if(!_finite(pNPCInfo->m_fAngle[Engine::ANGLE_Y]))
			pNPCInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}
	else
	{
		pNPCInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(fAngle);
		if(!_finite(pNPCInfo->m_fAngle[Engine::ANGLE_Y]))
			pNPCInfo->m_fAngle[Engine::ANGLE_Y] = fTemp;
	}	 
}
 

void CAIMgr::NPC_Target_Chase(CNPCs* pNPC, const float& fTimeDelta)
{
	Engine::CTransform* pNPCInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pNPC->GetComponent(L"Transform")));
	Engine::CTransform* pTargetInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pNPC->Get_TargetInfo()->GetComponent(L"Transform")));

	float fTime = pNPC->Get_Start_Chase();

	bool		bAttStyle = false;

	if(!pNPC->Get_bRelocation())
	{
		if(fTime < 0.4f)
		{
			pNPC->Set_AniIdx(STATENPC_IDLE_C);
			pNPC->Set_Start_Chase(fTime + fTimeDelta);
			return;
		}
	}

	if(pNPC->Get_AttackPos() != NULL)
	{
		pNPC->Get_AttackPos()->bPossession = false;
		pNPC->Set_AttackPos(NULL);
	}

	if(pNPC->Get_AttackPos() == NULL)
	{
		vector<Engine::ATTPOSITION*>::iterator iter;   
		vector<Engine::ATTPOSITION*>::iterator iter_end;
		vector<Engine::ATTPOSITION*>::iterator iterTemp;

		if(!bAttStyle)
		{
			iter = pNPC->Get_TargetInfo()->Get_AttackPosition()->begin();
			iter_end = pNPC->Get_TargetInfo()->Get_AttackPosition()->end();
			iterTemp = iterTemp = iter_end;	 
		}
		else
		{
			iter = pNPC->Get_TargetInfo()->Get_LongAttackPosition()->begin();
			iter_end = pNPC->Get_TargetInfo()->Get_LongAttackPosition()->end();
			iterTemp = iterTemp = iter_end;
		}

		float fShortDist = 1000.f;

		for(; iter != iter_end; ++iter)
		{
			if((*iter)->bPossession == false)
			{
				_vec3 vDist = (*iter)->vPos - pNPCInfo->m_vPos;

				float fDist = D3DXVec3Length(&vDist);
				if(fDist < fShortDist)
				{
					fShortDist = fDist;
					iterTemp = iter;
				}
			}		 
		}


		if(iter == iter_end)
		{
			if(iterTemp == iter_end)
			{
				if(!bAttStyle)
					pNPC->Get_TargetInfo()->Set_AttPositionFull(true);	
				else
					pNPC->Get_TargetInfo()->Set_Long_AttPositionFull(true);	

				pNPC->Set_TargetInfo(NULL);
				pNPC->Set_TargetInfo(NPC_CautionRange_Check(m_pMonstersInfo, pNPC));			
				return;
			}
			(*iterTemp)->bPossession = true;
			(*iterTemp)->fAttDist = pNPC->Get_AttackDist();
			pNPC->Set_AttackPos((*iterTemp));
		}
	}

	_vec3 vTargetPos = pTargetInfo->m_vPos;
	_vec3 vTargetDir = vTargetPos - pNPCInfo->m_vPos;
	vTargetDir = _vec3(vTargetDir.x, 0, vTargetDir.z);
	float fTargetDist = D3DXVec3Length(&vTargetDir);

	_vec3 vAttackPos = pNPC->Get_AttackPos()->vPos;
	_vec3 vDir = vAttackPos - pNPCInfo->m_vPos;
	vDir = _vec3(vDir.x, 0, vDir.z);
	_vec3 vNPCDir = pNPCInfo->m_vDir;
	vNPCDir = _vec3(vNPCDir.x, 0, vNPCDir.z);
	float fPosiDist = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vTargetDir, &vTargetDir);
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vNPCDir, &vNPCDir);

	if(fPosiDist > 0.1f)
	{ 
		pNPC->Set_Target_Attack(false);
		pNPC->Set_bAttack(false);
		vDir = _vec3(vDir.x, 0, vDir.z);
		if(pNPC->Get_BossCam())
			pNPC->Set_DwIndex(Engine::MoveOnNaviMesh(&pNPCInfo->m_vPos, &(vDir * fTimeDelta * 1.2f), pNPC->Get_DwIndex(), 0));    
		else
			pNPC->Set_DwIndex(Engine::MoveOnNaviMesh(&pNPCInfo->m_vPos, &(vDir * fTimeDelta), pNPC->Get_DwIndex(), 0));
		//pNPCInfo->m_vPos += vDir * fTimeDelta;
	}
	else
	{ 
		pNPC->Set_Target_Attack(true);
		pNPC->Set_bAttack(true);
		pNPC->Set_bRelocation(false);
		pNPC->Set_Att_DelayTime(2.f);
		return;
	}	

	// �޸��� �ൿ���� ����
	pNPC->Set_AniIdx(STATENPC_RUN);
	pNPC->Set_AttackEnd(false);
	pNPC->Set_Att_DelayTime(2.f);
}


void CAIMgr::NPC_Get_HitState(CNPCs* pNPC)
{
	_uint fAniType = pNPC->Get_AniIdx();

	// ü�� 0 ���ϸ� �״´�
	if(pNPC->GetStatus()->m_tRealStat.m_fCurHp <= 0.f) 
	{	
		pNPC->Set_AniIdx(STATENPC_DIE);
		pNPC->Set_bDieState(true);
		return;
	}
}


void CAIMgr::NPC_Pattern(CNPCs* pNpc, CCollision* pCollision, const float& fTimeDelta)
{
	// ���� �� ���� ó�� 
	NPC_Get_HitState(pNpc);

	_uint fAniType = pNpc->Get_AniIdx();

	if(fAniType == STATENPC_DIE)
	{
		NPC_Change_State(pNpc);		// �´� �����̳� ������ ������
		return;
	}

	CUnits* pTargetInfo = NULL;
	Engine::CTransform* pNPCInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pNpc->GetComponent(L"Transform")));

	pTargetInfo = NPC_CautionRange_Check(m_pMonstersInfo, pNpc);		
	pNpc->Set_TargetInfo(pTargetInfo);
	
	// NPC ���� �浹
	vector<CUnits*>::iterator iterNPC = m_pPlayer_Npc_Info->begin();
	vector<CUnits*>::iterator iterNPC_end = m_pPlayer_Npc_Info->end();
	CCollision* pp = pNpc->Get_collsion();
	for(; iterNPC != iterNPC_end; ++iterNPC)
	{
		Engine::CTransform* pInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>((*iterNPC)->GetComponent(L"Transform")));
		
		if(!pNpc->Get_bNPC_Collision())
		{
			if(pp->CollisionSphere(pNPCInfo, pInfo))
			{
				if(((CNPCs*)(*iterNPC))->Get_bNPC_Collision())
				{
					pNpc->Set_bNPC_Collision(true);	
					break;
				}
			}
		}
	}

	// Player ���� �浹
	vector<CPlayers*>::iterator iterPlayer = m_pPlatyersInfo->begin();
	vector<CPlayers*>::iterator iterPlayer_end = m_pPlatyersInfo->end();

	for(; iterPlayer != iterPlayer_end; ++iterPlayer)
	{
		Engine::CTransform* pInfo = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>((*iterPlayer)->GetComponent(L"Transform")));
		pp->CollisionSphere(pNPCInfo, pInfo);
	}

	if(pNpc->Get_TargetInfo() != NULL)
	{
		NPC_Look_Target(pTargetInfo, pNpc);

		if(pNpc->Get_AttackPos() != NULL)	// �Ҵ�� �ڸ��� ���ٸ� ���� �ް� �̵� 
		{	

			// ������ �ڸ��� ���� �� ����  

			if(pNpc->Get_bAttack())
			{	
				if(pNpc->Get_MainPlayer() != NULL)
				{	
					if(!pNpc->m_bStage2)
					{
						// �����÷��̾���� �����Ÿ� ��� ��� �ǵ��� �´�
						_vec3 vDist2 = pNpc->Get_MainPlayer()->GetInfo()->m_vPos - pNpc->GetInfo()->m_vPos;
						float fDist2 = D3DXVec3Length(&vDist2);
						if(fDist2 > 6.f)
						{
							_vec3 vReversDir = pNpc->Get_MainPlayer()->GetInfo()->m_vDir * -1;
							D3DXVec3Normalize(&vReversDir, &vReversDir);

							pNPCInfo->m_vPos = pNpc->Get_MainPlayer()->GetInfo()->m_vPos + vReversDir * 1.f;

							//pNPCInfo->m_vPos = pNpc->Get_MainPlayer()->GetInfo()->m_vPos;
							pNpc->Set_AniIdx(STATENPC_IDLE_P);	
							pNpc->Set_Start_Chase(0.f);
							pNpc->Set_Att_DelayTime(0.f);
							NPC_Change_State(pNpc);

							pNpc->Set_DwIndex(Engine::Get_CellIndex(&pNPCInfo->m_vPos));
							return;
						}
					}				 
				}

				if(!pNpc->Get_AttackPos()->bPossession)
					pNpc->Get_AttackPos()->bPossession = true;

				//// ������ ó��
				//Monster_AttackTiming(pMonster);

				if(!pNpc->Get_AttackEnd())
				{	 
					if(pNpc->Get_Att_DelayTime() >= 2.f)			// Attack Delay
					{
						if(!pNpc->Get_bAttackStyle())				// ���ݸ�� �ε��� �ο�	
							pNpc->Set_AniIdx((_uint)(rand() % 2 + 5));
						else
							pNpc->Set_AniIdx(STATENPC_ATK_1);

						pNpc->Set_AttackEnd(true);
						pNpc->Set_Att_DelayTime(0.f);
					}
					else
					{
						pNpc->Set_AniIdx(STATENPC_IDLE_C);
						float fTime = pNpc->Get_Att_DelayTime() + fTimeDelta;
						pNpc->Set_Att_DelayTime(fTime);
					}	 
				}
				pNpc->Set_Start_Chase(0.f);	
			}
			else   // �ƴ϶�� �̵�
				NPC_Target_Chase(pNpc, fTimeDelta);
		}
		else
			NPC_Target_Chase(pNpc, fTimeDelta);

	}
	else	// �� ������ Idle ������
	{
		if(pNpc->Get_AttackPos() != NULL)
		{	 
			pNpc->Get_AttackPos()->bPossession = false;
			pNpc->Set_AttackPos(NULL);
		}
	 
		if(!pNpc->Get_bDungeonNPC())
		{
			//if(!pNpc->m_bStage2)
			//{
				_vec3 vDist = pNpc->Get_OriPos() - pNPCInfo->m_vPos;
				vDist = _vec3(vDist.x, 0.f, vDist.z);
				_float fDist = D3DXVec3Length(&vDist);	  
				D3DXVec3Normalize(&vDist, &vDist);

				if(fDist > 0.1f)		// ���� �ڸ��� ���ư���
				{
					NPC_Look_OriPosition(pNpc);
					pNpc->Set_AniIdx(STATENPC_RUN);
					if(pNpc->Get_BossCam())
						pNpc->Set_DwIndex(Engine::MoveOnNaviMesh(&pNPCInfo->m_vPos, &(vDist * fTimeDelta * 1.2f), pNpc->Get_DwIndex(), 0));    
					else
						pNpc->Set_DwIndex(Engine::MoveOnNaviMesh(&pNPCInfo->m_vPos, &(vDist * fTimeDelta), pNpc->Get_DwIndex(), 0));    
					//pNPCInfo->m_vPos += vDist * fTimeDelta;
				}		 
				else
				{
					pNPCInfo->m_fAngle[Engine::ANGLE_Y] = pNpc->Get_OriAngle();
					pNpc->Set_AniIdx(STATENPC_IDLE_P);
				}
			//}
			 
		}
		else
		{
			if(pNpc->Get_MainPlayer() != NULL)
			{	
				// �����÷��̾���� �����Ÿ� ��� ��� �ǵ��� �´�
				_vec3 vDist2 = pNpc->Get_MainPlayer()->GetInfo()->m_vPos - pNpc->GetInfo()->m_vPos;
				float fDist2 = D3DXVec3Length(&vDist2);
				
				if(fDist2 > 6.f)
				{
					_vec3 vReversDir = pNpc->Get_MainPlayer()->GetInfo()->m_vDir * -1;
					D3DXVec3Normalize(&vReversDir, &vReversDir);

					pNPCInfo->m_vPos = pNpc->Get_MainPlayer()->GetInfo()->m_vPos + vReversDir * 1.f;

					//pNPCInfo->m_vPos = pNpc->Get_MainPlayer()->GetInfo()->m_vPos;
					pNpc->Set_AniIdx(STATENPC_IDLE_P);	
					pNpc->Set_Start_Chase(0.f);
					pNpc->Set_Att_DelayTime(0.f);
					NPC_Change_State(pNpc);

					pNpc->Set_DwIndex(Engine::Get_CellIndex(&pNPCInfo->m_vPos));
					return;
				}
			}
			 


			if( (pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_F || (pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_FL || 
				(pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_B || (pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_FR || 
				(pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_L || (pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_BL ||
				(pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_R || (pNpc->Get_MainPlayer()->Get_TrackID()) == CPlayers::RUN_BR)
			{
				pNpc->Set_bNPC_Collision(false);
			}


			_vec3 vDist = pNpc->Get_MainPlayer()->GetInfo()->m_vPos - pNPCInfo->m_vPos;
			vDist = _vec3(vDist.x, 0.f, vDist.z);
			_float fDist = D3DXVec3Length(&vDist);	  
			D3DXVec3Normalize(&vDist, &vDist);
			
			if(fDist > 1.f)		// ���� �ڸ��� ���ư���
			{ 
				NPC_Look_OriPosition(pNpc);

				if(!pNpc->Get_bNPC_Collision())
				{
					pNpc->Set_AniIdx(STATENPC_RUN);		
					if(pNpc->Get_BossCam())
						pNpc->Set_DwIndex(Engine::MoveOnNaviMesh(&pNPCInfo->m_vPos, &(vDist * fTimeDelta * 1.2f), pNpc->Get_DwIndex(), 0));    
					else
						pNpc->Set_DwIndex(Engine::MoveOnNaviMesh(&pNPCInfo->m_vPos, &(vDist * fTimeDelta), pNpc->Get_DwIndex(), 0));
				}
				else
					pNpc->Set_AniIdx(STATENPC_IDLE_P);		
			}		 
			else
			{
				pNpc->Set_bNPC_Collision(true);
				pNpc->Set_AniIdx(STATENPC_IDLE_P);
			}
			
			
		}
		pNpc->Set_Start_Chase(0.f);
		pNpc->Set_Att_DelayTime(0.f);
	}	

	NPC_Change_State(pNpc);
}




void CAIMgr::Change_State(CMonsters* pMonster/*Engine::CDynamicMesh& pMesh, UINT& iAniIdx, UINT& iPrevAniIdx*/)
{
	UINT					 iAniIdx = 0;
	UINT					 iPrevAniIdx = 0;
	
	Engine::CDynamicMesh*	 pMesh = NULL;

	iAniIdx = pMonster->Get_AniIdx();
	iPrevAniIdx = pMonster->Get_PrevAniIdx();


	pMesh = ((Engine::CDynamicMesh*)pMonster->GetMesh());

	if(iAniIdx != iPrevAniIdx)
	{		 
		switch(iAniIdx)
		{
		case STATE_IDLE_P:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_IDLE_C:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_WALK:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_RUN:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_ATK_1:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_ATK_2:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_SK_1:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_DMG_F:		
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_DOWN:		
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_DOWN_LOOP:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_GET_UP:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_STUN:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATE_DIE:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		}
		pMonster->Set_PrevAniIdx(iAniIdx);
	}	
}

void CAIMgr::BOSS_Change_State(CMonsters* pBoss/*Engine::CDynamicMesh& pMesh, UINT& iAniIdx, UINT& iPrevAniIdx*/)
{
	UINT					 iAniIdx = 0;
	UINT					 iPrevAniIdx = 0;

	Engine::CDynamicMesh*	 pMesh = NULL;

	iAniIdx = pBoss->Get_AniIdx();
	iPrevAniIdx = pBoss->Get_PrevAniIdx();


	pMesh = ((Engine::CDynamicMesh*)pBoss->GetMesh());

	if(iAniIdx != iPrevAniIdx)
	{		 
		switch(iAniIdx)
		{
		case STATEBOSS_IDLE_P:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_IDLE_C:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_WALK:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_RUN:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_ATK_1:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_ATK_2:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_DMG_F:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_STUN:		
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_DIE:		
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_SK_1:			
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_SK_2:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_SK_3:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_SK_4:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_SK_5:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_SK_6:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATEBOSS_SK_7:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		}
		pBoss->Set_PrevAniIdx(iAniIdx);
	}	 
}
void CAIMgr::NPC_Change_State(CNPCs* pNpc/*Engine::CDynamicMesh& pMesh, UINT& iAniIdx, UINT& iPrevAniIdx*/)
{
	UINT					 iAniIdx = 0;
	UINT					 iPrevAniIdx = 0;

	Engine::CDynamicMesh*	 pMesh = NULL;

	iAniIdx = pNpc->Get_AniIdx();
	iPrevAniIdx = pNpc->Get_PrevAniIdx();


	pMesh = ((Engine::CDynamicMesh*)pNpc->GetMesh());

	if(iAniIdx != iPrevAniIdx)
	{		 
		switch(iAniIdx)
		{
		case STATENPC_IDLE_P:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATENPC_IDLE_C:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATENPC_WALK:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATENPC_RUN:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATENPC_DIE:
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATENPC_ATK_1:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		case STATENPC_ATK_2:	
			pMesh->SetAnimationSet(iAniIdx);
			pMesh->SetTrackPos(0.0);
			break;
		}
		pNpc->Set_PrevAniIdx(iAniIdx);
	}
}

void CAIMgr::Free()
{
	m_pMonstersInfo->clear();
	m_pPlayer_Npc_Info->clear();
}
 