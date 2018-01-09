/*!
 * \file LandObject.h
 * \date 2016/02/26 14:34
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

#ifndef LandObject_h__
#define LandObject_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CTransform;
}

class CCollision;
class CLandObject
	: public Engine::CGameObject
{
protected:
	explicit CLandObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLandObject(void);

public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};

public:
	bool	Get_bCulled(void) { return m_bCulled; };
protected:
	void Culling_Frustum_Object(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);
	void Set_ConstantTable(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix, const D3DLIGHT9* pLightInfo, const D3DMATERIAL9* pMtrlInfo);
protected:
	_uint						m_iCullingCnt;
	Engine::CTransform*			m_pInfo;
	CCollision*					m_pCollision;
	bool						m_bCollision;
	bool						m_bCulled;
};

#endif // LandObject_h__