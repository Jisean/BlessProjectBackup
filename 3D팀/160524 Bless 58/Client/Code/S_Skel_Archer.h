#ifndef S_Skel_Archer_h__
#define S_Skel_Archer_h__  

#include "Monsters.h"

class CCalculator;
class CS_Skel_Archer
	: public CMonsters
{
private:
	explicit CS_Skel_Archer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CS_Skel_Archer(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	//========Get========//
	bool	Get_DMG_State(void) { return m_bDMG_State; };
	//========Set========//
	void	Set_DMG_State(bool bState) { m_bDMG_State = bState; };
	//======General======//
	void	AniFrameEnd(Engine::ANI_INFO* pData);
	virtual void Sound(DWORD dwMyIndex);

public:
	void	SetBossInfo(CMonsters* pBoss) { m_pBoss = pBoss; };

private:
	bool		m_bPush;
	//UINT		m_iAniIdx;

	CMonsters*		m_pBoss;

private:
	bool		m_bDMG_State;

private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CS_Skel_Archer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	void KeyCheck(void);
};
#endif // S_Skel_Archer_h__
