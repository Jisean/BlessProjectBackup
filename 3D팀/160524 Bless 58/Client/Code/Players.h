/*!
 * \file Players.h
 * \date 2016/05/20 14:50
 *
 * \author 권오훈
 * Contact: user@company.com
 *
 * \brief 플레이어 부모
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Players_h__
#define Players_h__

#include "Units.h"
#include <bitset>
#include "Monsters.h"
#include "Status.h"

class CMonsters;
class CDynamicCamera;
class CWyverns;
class CPlayers : public CUnits
{
public:
	enum PLAYERJOB{JOB_GUARDIAN, JOB_PALADIN , JOB_MAGICIAN, JOB_RANGER, JOB_END };
	enum TRACKID{   
		/*어택*/		ATK_0,ATK_1,ATK_2,
		/*대쉬*/		DASH, 
		/*히든스킬*/	HIDDEN_SKILL,
		/*공격스킬*/	SK_0,SK_1,SK_2,SK_3,SK_4,
		/*스탠드*/		STAND_NORMAL,STAND_WP,
		/*이동*/		RUN_F,RUN_B,RUN_L,RUN_R,RUN_FL,RUN_FR,RUN_BL,RUN_BR,
		/*점프*/		JUMP_UP,JUMP_LOOP,JUMP_DOWN,
		/*넉백*/		HIT_UP_ALL,
		/*피격*/		DMG_F, DMG_B,
		/*사망*/		DIE,
		/*기절*/		STUN,
		/*시체*/		DEADBODY,
		/*마운트*/		MOUNT_DIVE,
						MOUNT_FLYING_B,
						MOUNT_FLYING_BOOST_FLUTTER,
						MOUNT_FLYING_F_FLUTTER,
						MOUNT_FLYING_F_GLIDE,
						MOUNT_FLYING_IDLE ,
						MOUNT_BRAKE	,
						MOUNT_RUN_F ,
						MOUNT_RUN_B ,
						MOUNT_ROT_R ,
						MOUNT_ROT_L ,
						MOUNT_IDLE	,
						SK_CONSTANCE,
						TRACK_DEFAULT};

	// 트랙순서
	// 공격 : 어택1 -> 어택2 -> 어택3
	// 스킬 : 대쉬 -> (백점프 or V-Def 로 시작하는거) -> 버프 -> 공격스킬
	// 상태 : 스탠드 -> 이동 -> 점프 -> 넉백 -> 피격 -> 다이 -> 기절 -> 시체 -> 마운트

	enum KEY_STATEID{STAKEY_MOVE, STAKEY_COMBINE, STAKEY_COMBO, STAKEY_SKILL , STAKEY_STATIC,STAKEY_DEFAULT,STAKEY_END};				// 키값 분리 열거체(16/05/30 현재 쓰는곳 없음 뺄거 같음)
	enum AI_STATE{AI_STAND ,AI_DIE, AI_MOUNT , AI_EVASION, AI_STUN , AI_DASH, AI_DMG , AI_DOWN , AI_MOVE, AI_SKILL ,AI_TARGET_MOVE,AI_ATTACK, AI_DEFAULT};																// AI 동작 열거체
	enum ACT_STATE{ACT_DASH,ACT_JUMP, ACT_MOUNT, ACT_STUN, ACT_CANCLE, ACT_EVASION, ACT_DOWN, ACT_DMG_HIT,ACT_HIT_UP,ACT_DIE,ACT_DEFAULT,ACT_END};	// 상태 이상 열거체
	enum ATK_COMBO{ATTACK_1,ATTACK_2,ATTACK_3, ATTACK_END};
	enum SKILL_STATE{SKILL_1,SKILL_2,SKILL_3,SKILL_4,SKILL_5,SKILL_DASH,SKILL_INNATE,SKILL_DEFAULT,SKILL_END};


	enum EVENT_WYVERN_AI {EVENT_ROOT1,EVENT_ROOT2,EVENT_ROOT3,EVENT_ROOT4,EVENT_ROOT5,EVENT_ROOT6,EVENT_ROOT7,EVENT_ROOT_END};
protected:
	bitset<ACT_END> m_bActStateSet;		// 상태이상 비트셋
	bitset<ATTACK_END> m_bAtkComboSet;	// 기본 3연타 비트셋
	bitset<STAKEY_END> m_bKeyStateset;	// 키값 상태 비트셋
	bitset<SKILL_END> m_bSkillSet;		// 스킬 상태 비트셋
	bitset<AI_DEFAULT> m_bAIset;
	bitset<SKILL_END> m_bCoolTimeSet;
protected:
	bitset<EVENT_ROOT_END> m_bEventRootSet;
	_float					m_fEventTime[3];
	bool					m_bEventEnd;
	float					m_fFirstDis;
protected:
	bool		m_bMainPlayer;							// 내가 조종하는 플레이어 셋팅 변수
	TRACKID		m_eTrackID , m_ePrev_TrackID;			// 트랙인덱스값 , 이전값
	AI_STATE	m_eAI_StateID , m_ePrev_AIstateID;		// AI 상태값 , 이전값
	KEY_STATEID m_eKey_StateID , m_ePrev_KeyStateID;	// 키상태값, 이전값 (16/05/30 현재 쓰는곳 없음 왠지 뺄거같음)
	SKILL_STATE	m_eSkill_StateID , m_ePrev_SKstateID;
	_bool		m_bTrack;
	bool		m_bRun_Jump;
	PLAYERJOB	m_eTypeJob;
	TRACKID		m_eOriTrack;
	_bool		m_bCancel;
	bool		m_bConstance;
	_bool		m_bBossCam;
protected:
	_bool		m_bEvasion_Check;
	_vec3		m_vEvasionPos;
	_float		m_fEvasion_Length;
protected:
	_vec3	m_vEffectPos[SKILL_END];
	_float	m_fSkillAngle_Effect[SKILL_END];
protected:
	_float	m_fSpeed;				// 스피드
	_float	m_fTimeDelta;			// Get_Time 변수
	_float	m_fSlowTime;			// 트랙 슬로우 타임 
	_float	m_fAcc;					// 가속도
	_float	m_fRange;				// 범위
	_int	m_iAtkCnt;				// 어택 카운트
	_float	m_fConsoleTime[5];		// 콘솔 시간제어
	_bool	m_bSkill_Input; 
	_bool   m_bCombo_Input;
	DWORD	m_dwIndex;
	_float	m_fRadian;
	_float	m_fSaveAngle;
	DWORD   m_dwTestIndex;
protected:
	bool	m_bQuestEnd_First;
	bool	m_bQuestEnd_Second;
	bool	m_bQuestEnd_End;
	_vec3	m_vQuestEnd_Start_Pos;		// 퀘스트 완료 후 시작 위치
	_vec3	m_vQuestEnd_End_Pos;		// 퀘스트 완료 후 끝 위치
protected:
	//////////////////////////////////////////////////////////////////////////AI
	_vec3	m_vFormation;			// 메인플레이어 기준으로 포메이션 위치값
	_float	m_fFomationDis;			// 포메이션 벡터까지의 거리
protected:
	////////////////////////////////////////////////////////////////////////// 몬스터 관련
	vector<CMonsters*>  m_vecMonsters;
	_float				m_fTargetDis;
	_float				m_fCurrentDis;
	CMonsters*			m_pTargetMonster;
	CMonsters*			m_pRenewalMonster;
	CPlayers*			m_pMainPlayer;
	_bool				m_bHold;
	_vec3				m_vTargetDir;
	_float				m_fTargetAngle;
	_float				m_fHeight;
protected:
////////////////////////////////////////////////////////////////////////// 스킬 쿨타임
	_float				m_fSkillCooltime[SKILL_END];
	_float				m_fSkillCooltime_Max[SKILL_END];
protected:
	_float	m_fSpecPower;
	_float	m_fSpecRange;
protected:
	Engine::CDynamicMesh*			m_pMesh_Section[10];
	Engine::DERIVED_MESHCONTAINER*	m_pMeshContainer_Section[10];
protected:
	float	m_fPeriod;
protected:
	CWyverns* m_pWyvern;				// 와이번 객체 포인터
	_matrix	m_matSaddle;
	const _matrix* m_pmatSaddle;
protected:
	CDynamicCamera* m_pDynamicCamera; // 카메라 객체 포인터
protected:
	_float m_fAttackSlow;
	_float m_fTime;
	_bool  m_bSlowCheck;
protected:
	_float m_fSoundTargetDistance;
	_float m_fSoundVolume;
	CPlayers* m_pSoundTargetPlayer;
public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};
	void	SetMainPlayer(void);			// 메인플레이어 셋팅
	void	ResetMainPlayer(void);			// 메인플레이어 리셋
	virtual void UpdateAnimation(){};
public:
	void	AngleSetting();	 // 앵글 다시 잡는다
	void	TargetAngle_Setting(CMonsters* pTargetMonster);
	void	TargetSetting(CPlayers* pPlayer = NULL); // 타겟 셋팅 함수
	void	RangeSkill_State(SKILL_STATE eSkillType,_vec3 vPos = _vec3(0,0,0),_float fDMG = 20.f,_float fRange = 20.f,CPlayers* pPlayer = NULL);
	HRESULT	TargetRange_Collision(_float fDMG,_float fRange,_vec3 vPos,CPlayers* pPlayer ,vector<CMonsters*>* pvecMonster = NULL);
	void	SetFormation(_vec3 vPos);

	void	Set_RangeSkill_Check(_vec3 vPos, bool bCheck, _float fLength);
	bool	EvasionRange();
protected:
// 키 값
/////// /////////////////////////////////////////////////////////////////// 플레이어 컨트롤
	bool	KeyInput_Move(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyInput_Combo(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyInput_Skill(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyInput_Static(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyCheck(const _float& fTimeDelta, CPlayers* pPlayer);

//////////////////////////////////////////////////////////////////////////  UI 컨트롤
	void	KeyInput_UI_Set(const _float& fTimeDelta);
////////////////////////////////////////////////////////////////////////// 마운트 컨트롤

	void	KeyInput_Static_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
	bool	KeyInput_Move_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
	bool	KeyInput_Jump_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyCheck_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
private:
	bool	RangerAttack_Link(CPlayers* pPlayer);
	bool	MagicianAttack_Link(CPlayers* pPlayer);
	bool	GuardianAttack_Link(CPlayers* pPlayer);
	bool	PaladinAttack_Link(CPlayers* pPlayer);

////////////////////////////////////////////////////////////////////////// 이동
protected:
	void	MoveOnNaviMesh(D3DXVECTOR3* pDir, CPlayers* pPlayer = NULL);

////////////////////////////////////////////////////////////////////////// AI
protected:
	virtual void AI_Stand(void){};
	virtual	void AI_Patrol(void){};
	virtual	void AI_Combat(void){};
	virtual	void Movement_Pattern(void){};
	virtual	void AI_Move(void){};
	virtual	void AI_State(void){};
	virtual	void UpdateState_AI(void){};
	virtual	void SKill_CoolTime(void);
public:
	vector<CMonsters*>* Get_vecMonsters(void){return &m_vecMonsters;}

//////////////////////////////////////////////////////////////////////////GET
public:
	TRACKID		Get_TrackID(void){return m_eTrackID;}
	AI_STATE	Get_AIstate(void){return m_eAI_StateID;}
	CWyverns*	Get_Wyvern(void){return m_pWyvern;}
	TRACKID		Get_PrevTrackID(void){return m_ePrev_TrackID;}
	_bool		Get_SKill_Input(void){return m_bSkill_Input;}
	_bool		Get_Combo_Input(void){return m_bCombo_Input;}
	PLAYERJOB	Get_PlayerJob(void){return m_eTypeJob;}
	CMonsters*	Get_TargetMonster(void){return m_pTargetMonster;}
	CMonsters*	Get_RenewalTargetMon(){return m_pRenewalMonster;}
	_bool		Get_Hold(void){return m_bHold;}
	_float		Get_CurrentDis(void){return m_fCurrentDis;}
	_float		Get_CoolTime(_int iType){return m_fSkillCooltime[iType];}
	_float		Get_CoolTime_Max(_int iType){return m_fSkillCooltime_Max[iType];}
	_bool		Get_MainPlayer(void){return m_bMainPlayer;}
//////////////////////////////////////////////////////////////////////////SET
public:
	void	Set_TrackID(TRACKID TrackID){m_eTrackID = TrackID;}
	void	Set_AIstate(AI_STATE StateID){m_eAI_StateID = StateID;}
	void	Set_SKill_Input(bool bCheck){m_bSkill_Input = bCheck;}
	void	Set_Combo_Input(bool bCheck){m_bCombo_Input = bCheck;}
	void	Set_vecMonsters(vector<CMonsters*> pvecMonsters){m_vecMonsters = pvecMonsters;}
	void	Set_TagetMonster(CMonsters* pMonster){m_pTargetMonster = pMonster;}
	void	Set_RenewalTargetMon(CMonsters* pMonster){m_pRenewalMonster = pMonster;}
	void	RenewalTarget(CMonsters* pMonster, CPlayers* pPlayer);
	void	Set_MainPlayer(CPlayers* pMainPlayer){m_pMainPlayer = pMainPlayer;}
	void	Set_Hold(_bool bHold){m_bHold = bHold;}
	void	Set_CurrentDis(_float fDis){m_fCurrentDis = fDis;}
	void	Set_DamageType(ACT_STATE state, float fDMG , Engine::CStatus::DAMAGETYPE damageType);
	void	Set_NaviIndex(DWORD dwIndex){ m_dwIndex = dwIndex; }
	void	Set_BossCam(bool bBossCam) { m_bBossCam = bBossCam; }
public:
/////////////////////// 비트셋 인터페이스 함수
////////////////////////////////////////////////////////////////////////// GET

// 행동상태 비트셋
	_bool	Any_ActBitset(void){return m_bActStateSet.any();}								
	_bool	Test_ActBitSet(ACT_STATE state , _bool bCheck = false){return m_bActStateSet.test(state);}
// 공격 연계 비트셋
	_bool	Any_AtkBitSet(void){return m_bAtkComboSet.any();}
	_bool	Test_AtkBitSet(ATK_COMBO state , _bool bCheck = false){return m_bAtkComboSet.test(state);}
// 키값 상태 비트셋
	_bool	Any_KeyBitSet(void){return m_bKeyStateset.any();}
	_bool	Test_KeyBitSet(KEY_STATEID state , _bool bCheck = false){return m_bKeyStateset.test(state);}
// 스킬 상태 비트셋
	_bool	Any_SkillBitSet(void){return m_bSkillSet.any();}
	_bool	Test_SkillBitSet(SKILL_STATE state , _bool bCheck = false){return m_bSkillSet.test(state);}
public:
////////////////////////////////////////////////////////////////////////// SET
// 행동상태 비트셋
	void	Set_ActBitset(ACT_STATE state , _bool bCheck = false)	{bCheck == true ? m_bActStateSet.set() : m_bActStateSet.set(state);}
	void	Reset_ActBitset(ACT_STATE state , _bool bCheck = false)	{bCheck == true ? m_bActStateSet.reset() : m_bActStateSet.reset(state);}
// 공격 연계 비트셋
	void	Set_AtkBitset(ATK_COMBO state , _bool bCheck = false)	{bCheck == true ? m_bAtkComboSet.set() : m_bAtkComboSet.set(state);}
	void	Reset_AtkBitset(ATK_COMBO state , _bool bCheck = false)	{bCheck == true ? m_bAtkComboSet.reset() : m_bAtkComboSet.reset(state);}
// 키값 상태 비트셋
	void	Set_KeyBitset(KEY_STATEID state , _bool bCheck = false)		{bCheck == true ? m_bKeyStateset.set() : m_bKeyStateset.set(state);}			
	void	Reset_KeyBitset(KEY_STATEID state , _bool bCheck = false)	{bCheck == true ? m_bKeyStateset.reset() : m_bKeyStateset.reset(state);}
// 스킬 상태 비트셋
	void	Set_SkillBitset(SKILL_STATE state , _bool bCheck = false)	{bCheck == true ? m_bSkillSet.set() : m_bSkillSet.set(state);}					
	void	Reset_SkillBitset(SKILL_STATE state , _bool bCheck = false)	{bCheck == true ? m_bSkillSet.reset() : m_bSkillSet.reset(state);}
public:
	void	Set_SpecPower(float fPower){m_fSpecPower += fPower;}
	void	Set_SpecRange(float fRange){m_fSpecRange >= 0.f ? m_fSpecRange += fRange : m_fSpecRange = 0.f;}
	float	Get_SpecPower(void){ return m_fSpecPower; }
	float	Get_SpecRange(void){ return m_fSpecRange; }
protected:
	explicit CPlayers(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayers(void);
	virtual void Free(void);
	
 
};

#endif // Players_h__


	