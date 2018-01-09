/*!
 * \file Player.h
 * \date 2016/03/08 13:58
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

#ifndef WyvernBrown_h__
#define WyvernBrown_h__

#include "Wyverns.h"


namespace Engine
{
	class CDynamicMesh;
}

class CPlayers;
class CCalculator;
class CWyvernBrown
	: public CWyverns
{	
private:
	explicit CWyvernBrown(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWyvernBrown(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
	virtual void UpdateAnimation(CWyverns::WYVERNANI eTrack ,float fTrackTime);
public:
	////////////////////////////////////////////////////////////////////////// 와이번 애니메이션 상태 함수
	void	Ani_Jump_To_Wyvern(float fTrackTime);
	void	Ani_Move_To_Wyvern(float fTrackTime);
	void	Ani_Fly_To_Wyvern(float fTrackTime);
	void	Ani_Stand_To_Wyvern(float fTrackTime);
private:
	virtual HRESULT AddComponent(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CWyvernBrown* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};


#endif // WyvernBrown_h__
