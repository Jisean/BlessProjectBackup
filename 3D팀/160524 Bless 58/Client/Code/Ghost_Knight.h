#ifndef Ghost_Knight_h__
#define Ghost_Knight_h__

#include "Monsters.h"
#include "Texture.h"


struct METEOIFNO
{
	float fTime;
	_vec3 vPos;
	bool	bCheck;
};
class CSkill_Range;
class CSkill_Range_1;
class CGhost_Knight
	: public CMonsters
{
private:
	explicit CGhost_Knight(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGhost_Knight(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	//========Get========//
	bool	Get_DMG_State(void) { return m_bDMG_State; };
	vector<CMonsters*>*	Get_Monster_Call(void) { return &m_vecCall_Monster; };
	//========Set========//
	void	Set_DMG_State(bool bState) { m_bDMG_State = bState; }
	void	Set_BossCam(bool bBossCam) { m_bBossCam = bBossCam; }
	//======General======//
	void	AniFrameEnd(Engine::ANI_INFO* pData);
	void	Skill_State(const _float& fTimeDelta);
	void	KeyCheck(void);
	virtual void Sound(DWORD dwMyIndex);
	
private:
	bool		m_bPush;

private:
	bool		m_bDMG_State;

	bool		m_bTest222;
	float		m_fTesttime;

private:
	//CSkill_Range*				m_pSkillRange;
	//CSkill_Range_1*			m_pSkillRange_1;
	_vec3						m_vOraPos;
	_bool						m_bBossCam;
	_bool						m_bBossCamSK1;
	_bool						m_bBossCamEff;
	_float						m_fBossCamTime;

private:
	vector<METEOIFNO*>			m_vecMeteoInfo;
	vector<CMonsters*>			m_vecCall_Monster;
	vector<float>				m_vecSkillAngle;
	float						m_fExploAngle[16];
	D3DXVECTOR3					m_vExploPos[16];

private:
	virtual HRESULT AddComponent(void);
	void ActionCamMove(_float fTimeDelta);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);

public:
	static CGhost_Knight* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};
#endif // Ghost_Knight_h__
