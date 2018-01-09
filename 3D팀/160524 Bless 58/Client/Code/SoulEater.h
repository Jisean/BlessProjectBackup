#ifndef SoulEater_h__
#define SoulEater_h__


#include "Monsters.h"

class CSoulEater
	: public CMonsters
{
private:
	explicit CSoulEater(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSoulEater(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	//========Get========//
	bool	Get_DMG_State(void) { return m_bDMG_State; };
	vector<CMonsters*>*	Get_Monster_Call(void) { return &m_vecSummon_Monster; };
	//========Set========//
	void	Set_DMG_State(bool bState) { m_bDMG_State = bState; };
	//======General======//
	void	AniFrameEnd(Engine::ANI_INFO* pData);
	void	Skill_State(const _float& fTimeDelta);
	void	KeyCheck(void);
	virtual void Sound(DWORD dwMyIndex);

	void	SkillAttack_Timing(float fTrack, float fTimeDelta);

public:

	float		m_fTest;
private:
	bool		m_bPush;
	//UINT		m_iAniIdx;

private:
	bool		m_bDMG_State;


private:
	vector<CMonsters*>			m_vecSummon_Monster;
	vector<_vec3>				m_vecSkillPos;
	vector<float>				m_vecPoisonDMG_delay;

private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);

public:
	static CSoulEater* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};
#endif // SoulEater_h__
