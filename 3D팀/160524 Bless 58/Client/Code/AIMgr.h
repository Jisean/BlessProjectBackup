#ifndef AIMgr_h__
#define AIMgr_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Units.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}

class CCollision;
class CPlayers;
class CMonsters;
class CNPCs;
class CAIMgr : public Engine::CBase
{
	DECLARE_SINGLETON(CAIMgr)

private:
	CAIMgr(void);
	~CAIMgr(void);

public:
	enum STATEID { STATE_IDLE_P, STATE_IDLE_C, STATE_WALK, STATE_RUN, STATE_ATK_1, STATE_ATK_2, STATE_SK_1, 
					STATE_DMG_F, STATE_DOWN, STATE_DOWN_LOOP, STATE_GET_UP, STATE_STUN, STATE_DIE};

	enum STATEBOSSID { STATEBOSS_IDLE_P, STATEBOSS_IDLE_C, STATEBOSS_WALK, STATEBOSS_RUN, 
						STATEBOSS_ATK_1, STATEBOSS_ATK_2, 
						STATEBOSS_DMG_F, STATEBOSS_STUN, STATEBOSS_DIE,
						STATEBOSS_SK_1, STATEBOSS_SK_2, STATEBOSS_SK_3, STATEBOSS_SK_4, STATEBOSS_SK_5, STATEBOSS_SK_6, STATEBOSS_SK_7};

	enum STATENPCID { STATENPC_IDLE_P, STATENPC_IDLE_C, STATENPC_WALK, STATENPC_RUN, STATENPC_DIE, STATENPC_ATK_1, STATENPC_ATK_2};

public:
	// 인식
	void			Look_Target(CUnits* pTarget, CMonsters* pMonster);								// 타겟을 바로 본다				
	void			GetHit_State(CMonsters* pMonster);
	void			Target_Search(CMonsters* pMonster);
	void			Look_OriPosition(CMonsters* pMonster);
	

public:	
	// 대기
	CUnits*		Monster_CautionRange_Check(vector<CUnits*>* pTargetInfo, CMonsters* pMonster);		// 경계 범위					
	// 이동 	(공격)
	void		Monster_Target_Chase(CMonsters* pMonster, const float& fTimeDelta);      // 타겟 추격 공격자리로 이동함	

	void		Monster_AttackTiming(CMonsters* pMonster);
	// 대기 -> 인식 -> 공격 순으로 이루어 진다  
	void		Monster_Pattern(vector<CPlayers*>* pPlatyersInfo, CMonsters* pMonster, CCollision* pCollision,const float& fTimeDelta);
	

	// Boss Patten

	// 대기
	CUnits*		BOSS_CautionRange_Check(vector<CUnits*>* pTargetInfo, CMonsters* pBoss);		// 경계 범위					
	// 이동 (공격)
	void		BOSS_Target_Chase(CMonsters* pBoss, const float& fTimeDelta);      // 타겟 추격 공격자리로 이동함	

	void		BOSS_AttackTiming(CMonsters* pBoss);		 

	void		BOSS_Pattern(vector<CPlayers*>* pPlatyersInfo, CMonsters* pBoss, CCollision* pCollision,const float& fTimeDelta);


	// NPC Patten

	// 대기
	CUnits*		NPC_CautionRange_Check(vector<CMonsters*>* pTargetInfo, CNPCs* pNpc);		// 경계 범위					
	
	// 이동 	(공격)
	void		NPC_Target_Chase(CNPCs* pMonster, const float& fTimeDelta);      // 타겟 추격 공격자리로 이동함	

	// 인식
	void		NPC_Look_Target(CUnits* pTarget, CNPCs* pNPC);								// 타겟을 바로 본다				

	void		NPC_Look_OriPosition(CNPCs* pNPC);
 
	void		NPC_Get_HitState(CNPCs* pNPC);
	
	void		NPC_Pattern(CNPCs* pNpc, CCollision* pCollision, const float& fTimeDelta);
	 

public:
	void	Change_State(CMonsters* pMonster/*Engine::CDynamicMesh& pMesh, UINT&	iAniIdx, UINT& iPrevAniIdx*/);
	void	BOSS_Change_State(CMonsters* pBoss/*Engine::CDynamicMesh& pMesh, UINT&	iAniIdx, UINT& iPrevAniIdx*/);
	void	NPC_Change_State(CNPCs* pNpc/*Engine::CDynamicMesh& pMesh, UINT&	iAniIdx, UINT& iPrevAniIdx*/);

public:
	void	Set_MonstersInfo(vector<CMonsters*>* pMonstersInfo) { m_pMonstersInfo = pMonstersInfo; };
	void	Set_Player_NPC_Info(vector<CUnits*>* pPlayer_Npc_Info) { m_pPlayer_Npc_Info = pPlayer_Npc_Info; };
	void	Set_NPC_Info(vector<CNPCs*>* pNpc_Info) { m_pNPCInfo = pNpc_Info; };
	void	Set_PlayerInfo(vector<CPlayers*>* pPlayerInfo) { m_pPlatyersInfo = pPlayerInfo; };


public:
	void	Monster_Aggro(CMonsters* pMonster, CUnits* pTargetInfo, float fTime);			// 어그로로 인한 타겟 변경

public:
	vector<CUnits*>* Get_player_NPC_Info(void) { return m_pPlayer_Npc_Info; };
	vector<CNPCs*>* Get_NPCInfo(void) { return m_pNPCInfo; };
	vector<CPlayers*>* Get_PlayerInfo(void) { return m_pPlatyersInfo; };

private:
	vector<CUnits*>*		m_pPlayer_Npc_Info;
	vector<CPlayers*>*		m_pPlatyersInfo;
	vector<CMonsters*>*		m_pMonstersInfo;
	vector<CNPCs*>*			m_pNPCInfo;

public:
	void	Free(void);
};

#endif // AIMgr_h__
