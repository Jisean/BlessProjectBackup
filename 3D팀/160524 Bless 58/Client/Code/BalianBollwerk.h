#ifndef BalianBollwerk_h__
#define BalianBollwerk_h__


#include "NPCs.h"

class CCalculator;
class CBalianBollwerk
	: public CNPCs
{
private:
	explicit CBalianBollwerk(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBalianBollwerk(void);

public:
	D3DXMATRIX*		m_pWeaponHand_L;
	D3DXMATRIX*		m_pWeaponHand_R;
	D3DXMATRIX*		m_pHand_L;
	D3DXMATRIX*		m_pHand_R;

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	

public:
	D3DXMATRIX*		Get_Hand_L(void) { return m_pHand_L; };
	D3DXMATRIX*		Get_Hand_R(void) { return m_pHand_R; };

	void	AniFrameEnd(/*Engine::ANI_INFO* pData*/);

private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);


public:
	static CBalianBollwerk* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};
#endif // BalianBollwerk_h__
