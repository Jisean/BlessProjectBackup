/*!
 * \file Players.h
 * \date 2016/05/20 14:50
 *
 * \author �ǿ���
 * Contact: user@company.com
 *
 * \brief �÷��̾� �θ�
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
		/*����*/		ATK_0,ATK_1,ATK_2,
		/*�뽬*/		DASH, 
		/*���罺ų*/	HIDDEN_SKILL,
		/*���ݽ�ų*/	SK_0,SK_1,SK_2,SK_3,SK_4,
		/*���ĵ�*/		STAND_NORMAL,STAND_WP,
		/*�̵�*/		RUN_F,RUN_B,RUN_L,RUN_R,RUN_FL,RUN_FR,RUN_BL,RUN_BR,
		/*����*/		JUMP_UP,JUMP_LOOP,JUMP_DOWN,
		/*�˹�*/		HIT_UP_ALL,
		/*�ǰ�*/		DMG_F, DMG_B,
		/*���*/		DIE,
		/*����*/		STUN,
		/*��ü*/		DEADBODY,
		/*����Ʈ*/		MOUNT_DIVE,
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

	// Ʈ������
	// ���� : ����1 -> ����2 -> ����3
	// ��ų : �뽬 -> (������ or V-Def �� �����ϴ°�) -> ���� -> ���ݽ�ų
	// ���� : ���ĵ� -> �̵� -> ���� -> �˹� -> �ǰ� -> ���� -> ���� -> ��ü -> ����Ʈ

	enum KEY_STATEID{STAKEY_MOVE, STAKEY_COMBINE, STAKEY_COMBO, STAKEY_SKILL , STAKEY_STATIC,STAKEY_DEFAULT,STAKEY_END};				// Ű�� �и� ����ü(16/05/30 ���� ���°� ���� ���� ����)
	enum AI_STATE{AI_STAND ,AI_DIE, AI_MOUNT , AI_EVASION, AI_STUN , AI_DASH, AI_DMG , AI_DOWN , AI_MOVE, AI_SKILL ,AI_TARGET_MOVE,AI_ATTACK, AI_DEFAULT};																// AI ���� ����ü
	enum ACT_STATE{ACT_DASH,ACT_JUMP, ACT_MOUNT, ACT_STUN, ACT_CANCLE, ACT_EVASION, ACT_DOWN, ACT_DMG_HIT,ACT_HIT_UP,ACT_DIE,ACT_DEFAULT,ACT_END};	// ���� �̻� ����ü
	enum ATK_COMBO{ATTACK_1,ATTACK_2,ATTACK_3, ATTACK_END};
	enum SKILL_STATE{SKILL_1,SKILL_2,SKILL_3,SKILL_4,SKILL_5,SKILL_DASH,SKILL_INNATE,SKILL_DEFAULT,SKILL_END};


	enum EVENT_WYVERN_AI {EVENT_ROOT1,EVENT_ROOT2,EVENT_ROOT3,EVENT_ROOT4,EVENT_ROOT5,EVENT_ROOT6,EVENT_ROOT7,EVENT_ROOT_END};
protected:
	bitset<ACT_END> m_bActStateSet;		// �����̻� ��Ʈ��
	bitset<ATTACK_END> m_bAtkComboSet;	// �⺻ 3��Ÿ ��Ʈ��
	bitset<STAKEY_END> m_bKeyStateset;	// Ű�� ���� ��Ʈ��
	bitset<SKILL_END> m_bSkillSet;		// ��ų ���� ��Ʈ��
	bitset<AI_DEFAULT> m_bAIset;
	bitset<SKILL_END> m_bCoolTimeSet;
protected:
	bitset<EVENT_ROOT_END> m_bEventRootSet;
	_float					m_fEventTime[3];
	bool					m_bEventEnd;
	float					m_fFirstDis;
protected:
	bool		m_bMainPlayer;							// ���� �����ϴ� �÷��̾� ���� ����
	TRACKID		m_eTrackID , m_ePrev_TrackID;			// Ʈ���ε����� , ������
	AI_STATE	m_eAI_StateID , m_ePrev_AIstateID;		// AI ���°� , ������
	KEY_STATEID m_eKey_StateID , m_ePrev_KeyStateID;	// Ű���°�, ������ (16/05/30 ���� ���°� ���� ���� ���Ű���)
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
	_float	m_fSpeed;				// ���ǵ�
	_float	m_fTimeDelta;			// Get_Time ����
	_float	m_fSlowTime;			// Ʈ�� ���ο� Ÿ�� 
	_float	m_fAcc;					// ���ӵ�
	_float	m_fRange;				// ����
	_int	m_iAtkCnt;				// ���� ī��Ʈ
	_float	m_fConsoleTime[5];		// �ܼ� �ð�����
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
	_vec3	m_vQuestEnd_Start_Pos;		// ����Ʈ �Ϸ� �� ���� ��ġ
	_vec3	m_vQuestEnd_End_Pos;		// ����Ʈ �Ϸ� �� �� ��ġ
protected:
	//////////////////////////////////////////////////////////////////////////AI
	_vec3	m_vFormation;			// �����÷��̾� �������� �����̼� ��ġ��
	_float	m_fFomationDis;			// �����̼� ���ͱ����� �Ÿ�
protected:
	////////////////////////////////////////////////////////////////////////// ���� ����
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
////////////////////////////////////////////////////////////////////////// ��ų ��Ÿ��
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
	CWyverns* m_pWyvern;				// ���̹� ��ü ������
	_matrix	m_matSaddle;
	const _matrix* m_pmatSaddle;
protected:
	CDynamicCamera* m_pDynamicCamera; // ī�޶� ��ü ������
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
	void	SetMainPlayer(void);			// �����÷��̾� ����
	void	ResetMainPlayer(void);			// �����÷��̾� ����
	virtual void UpdateAnimation(){};
public:
	void	AngleSetting();	 // �ޱ� �ٽ� ��´�
	void	TargetAngle_Setting(CMonsters* pTargetMonster);
	void	TargetSetting(CPlayers* pPlayer = NULL); // Ÿ�� ���� �Լ�
	void	RangeSkill_State(SKILL_STATE eSkillType,_vec3 vPos = _vec3(0,0,0),_float fDMG = 20.f,_float fRange = 20.f,CPlayers* pPlayer = NULL);
	HRESULT	TargetRange_Collision(_float fDMG,_float fRange,_vec3 vPos,CPlayers* pPlayer ,vector<CMonsters*>* pvecMonster = NULL);
	void	SetFormation(_vec3 vPos);

	void	Set_RangeSkill_Check(_vec3 vPos, bool bCheck, _float fLength);
	bool	EvasionRange();
protected:
// Ű ��
/////// /////////////////////////////////////////////////////////////////// �÷��̾� ��Ʈ��
	bool	KeyInput_Move(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyInput_Combo(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyInput_Skill(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyInput_Static(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyCheck(const _float& fTimeDelta, CPlayers* pPlayer);

//////////////////////////////////////////////////////////////////////////  UI ��Ʈ��
	void	KeyInput_UI_Set(const _float& fTimeDelta);
////////////////////////////////////////////////////////////////////////// ����Ʈ ��Ʈ��

	void	KeyInput_Static_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
	bool	KeyInput_Move_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
	bool	KeyInput_Jump_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
	void	KeyCheck_To_Wyvern(const _float& fTimeDelta, CPlayers* pPlayer);
private:
	bool	RangerAttack_Link(CPlayers* pPlayer);
	bool	MagicianAttack_Link(CPlayers* pPlayer);
	bool	GuardianAttack_Link(CPlayers* pPlayer);
	bool	PaladinAttack_Link(CPlayers* pPlayer);

////////////////////////////////////////////////////////////////////////// �̵�
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
/////////////////////// ��Ʈ�� �������̽� �Լ�
////////////////////////////////////////////////////////////////////////// GET

// �ൿ���� ��Ʈ��
	_bool	Any_ActBitset(void){return m_bActStateSet.any();}								
	_bool	Test_ActBitSet(ACT_STATE state , _bool bCheck = false){return m_bActStateSet.test(state);}
// ���� ���� ��Ʈ��
	_bool	Any_AtkBitSet(void){return m_bAtkComboSet.any();}
	_bool	Test_AtkBitSet(ATK_COMBO state , _bool bCheck = false){return m_bAtkComboSet.test(state);}
// Ű�� ���� ��Ʈ��
	_bool	Any_KeyBitSet(void){return m_bKeyStateset.any();}
	_bool	Test_KeyBitSet(KEY_STATEID state , _bool bCheck = false){return m_bKeyStateset.test(state);}
// ��ų ���� ��Ʈ��
	_bool	Any_SkillBitSet(void){return m_bSkillSet.any();}
	_bool	Test_SkillBitSet(SKILL_STATE state , _bool bCheck = false){return m_bSkillSet.test(state);}
public:
////////////////////////////////////////////////////////////////////////// SET
// �ൿ���� ��Ʈ��
	void	Set_ActBitset(ACT_STATE state , _bool bCheck = false)	{bCheck == true ? m_bActStateSet.set() : m_bActStateSet.set(state);}
	void	Reset_ActBitset(ACT_STATE state , _bool bCheck = false)	{bCheck == true ? m_bActStateSet.reset() : m_bActStateSet.reset(state);}
// ���� ���� ��Ʈ��
	void	Set_AtkBitset(ATK_COMBO state , _bool bCheck = false)	{bCheck == true ? m_bAtkComboSet.set() : m_bAtkComboSet.set(state);}
	void	Reset_AtkBitset(ATK_COMBO state , _bool bCheck = false)	{bCheck == true ? m_bAtkComboSet.reset() : m_bAtkComboSet.reset(state);}
// Ű�� ���� ��Ʈ��
	void	Set_KeyBitset(KEY_STATEID state , _bool bCheck = false)		{bCheck == true ? m_bKeyStateset.set() : m_bKeyStateset.set(state);}			
	void	Reset_KeyBitset(KEY_STATEID state , _bool bCheck = false)	{bCheck == true ? m_bKeyStateset.reset() : m_bKeyStateset.reset(state);}
// ��ų ���� ��Ʈ��
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


	