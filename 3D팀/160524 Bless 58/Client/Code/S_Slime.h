#ifndef S_Slime_h__
#define S_Slime_h__


#include "Monsters.h"

class CCalculator;
class CS_Slime
	: public CMonsters
{
private:
	explicit CS_Slime(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CS_Slime(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	//========Get========//
	bool	Get_DMG_State(void) { return m_bDMG_State; };
	//========Set========//
	void	Set_DMG_State(bool bState) { m_bDMG_State = bState; };
	//void	Set_SkillTime(float fTime) { m_fSkillTime = fTime; };
	//======General======//
	void	AniFrameEnd(Engine::ANI_INFO* pData);
	virtual void Sound(DWORD dwMyIndex);

public:
	void	SetBossInfo(CMonsters* pBoss) { m_pBoss = pBoss; };
	void	Skill_State(const _float& fTimeDelta);
	 
private:
	bool		m_bPush;
	bool		m_bDMG_State;
	//UINT		m_iAniIdx;
	//float		m_fSkillTime;

private:
	CMonsters*		m_pBoss;
	


private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CS_Slime* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	void KeyCheck(void);
};
#endif // S_Slime_h__
