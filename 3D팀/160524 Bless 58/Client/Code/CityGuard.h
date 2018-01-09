/*!
 * \file Ursa_01.cpp
 * \date 2016/05/21 21:46
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

#ifndef CityGuard_h__
#define CityGuard_h__

#include "NPCs.h"

class CCalculator;
class CCityGuard
	: public CNPCs
{
private:
	explicit CCityGuard(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCityGuard(void);

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
	static CCityGuard* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};

#endif // CityGuard_h__
