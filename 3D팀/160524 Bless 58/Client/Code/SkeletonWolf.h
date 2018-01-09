#ifndef SkeletonWolf_h__
#define SkeletonWolf_h__

#include "Monsters.h"

class CCalculator;
class CSkeletonWolf
	: public CMonsters
{
private:
	explicit CSkeletonWolf(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSkeletonWolf(void);

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

private:
	bool		m_bPush;
	//UINT		m_iAniIdx;

private:
	bool		m_bDMG_State;

private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CSkeletonWolf* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	void KeyCheck(void);
};
#endif // SkeletonWolf_h__
