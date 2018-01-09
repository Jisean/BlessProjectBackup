/*!
 * \file MainPlayer.h
 * \date 2016/03/08 13:58
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef MainPlayer_h__
#define MainPlayer_h__

#include "Players.h"
#include "Aim.h"

class CAim;
class CCalculator;
class CMainPlayer
	: public CPlayers
{
public:
	// Quest 관련
	enum eQuestType{QUESTTYPE_MAIN, QUESTTYPE_SUB, QUESTTPE_END};
	enum eQuestStep{QUEST_BEFORE, QUEST_PROGRESS, QUEST_COMPLETE, QUEST_END};
	enum eScene{SCENE_STAGE1, SCENE_STAGE2, SCENE_END};
	
	enum eControl{CTRL_F1,CTRL_F2,CTRL_F3,CTRL_END};
private:
	enum FORMATION{FOR_GAURDIAN, FOR_PALADIN, FOR_MAGICIAN, FOR_RANGER,FOR_DEFAULT};
private:
	explicit CMainPlayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMainPlayer(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};	
	virtual void UpdateAnimation(){};
public:
	virtual void AI_Position();
public:
	bitset<CTRL_END> m_bCtrlbitset;
private:
	CPlayers*	m_pPlayer;
	PLAYERJOB	m_eMainPlayer;
	CPlayers*	m_pPlayerSet[JOB_END];
	CAim*		m_pAim;
	FORMATION   m_eFormation;
	_vec3		m_vFormation[3];
	_bool		m_bCheck;
	//퀘스트 관련
	eQuestType  m_eQuestType;
	eQuestStep  m_eQuestStep;
	eScene		m_eScene;
	_bool		m_bTargetEff;
	_vec3*		m_vStage2SaveQ;
private:
	_bool		m_bEvent_Start;
	_bool		m_bEvent_End;
	_vec3		m_vEventPos_Start;
	_vec3		m_vEventPos_End;
	_float		m_fSubAngle[3];

	_vec3		m_vNaviPos[4];
	_vec3		m_vCatMullromPos[60];
	_bool		m_bNaviStart;
public:
	void NaviReset(void);
private:
	virtual HRESULT AddComponent(void){return S_OK; }
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
	void SetMainPlayer(CPlayers* pPlayer);
	void ChangePlayer(void);
	void RangerAim(void);
	// 퀘스트 관련
	void MainQuestCheck(void);
	void MainQuestAccept(void);
	void ControlPlayers();
	void NaviEffect();
	void EventFunction();
public:
	static	 CMainPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual	void Free(void);
	void		SetPlayerSet(WORD iPlayerIndex, CPlayers* pPlayer);
	CPlayers*	Get_PlayerSet(WORD wPlayerID){return m_pPlayerSet[wPlayerID];}
	void		StageChange(void);
	// 퀘스트 관련
	eQuestStep*   Get_QuestStep(void) { return &m_eQuestStep; }
	eQuestType* Get_QuestType(void) { return &m_eQuestType; }
	void		SetScene(eScene eSceneType) { m_eScene = eSceneType; }
	eScene		Get_MainPlayerScene(void) { return m_eScene; }
public:
	bool		Get_Event_Start(){return m_bEvent_Start;}
	bool		Get_Event_End(){return m_bEvent_End;}
	void		Set_Event_Start(bool bCheck){m_bEvent_Start = bCheck;}
	void		Set_Event_End(bool bCheck){m_bEvent_End = bCheck;}
public:
	//////////////////////////////////////////////////////////////////////////GET
	CPlayers* Get_TargetPlayer() { return m_pPlayer; }
	_vec3	  Get_Formation(WORD wCnt){return m_vFormation[wCnt];}
public:
	//////////////////////////////////////////////////////////////////////////SET
	
};


#endif // MainPlayer_h__
