#ifndef CityGuard2_h__
#define CityGuard2_h__

#include "NPCs.h"

class CCalculator;

class CCityGuard2
	: public CNPCs
{
private:
	explicit CCityGuard2(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCityGuard2(void);

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
	static CCityGuard2* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};

#endif // CityGuard2_h__
