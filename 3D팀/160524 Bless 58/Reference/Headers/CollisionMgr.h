/*!
 * \file CollisionMgr.h
 * \date 2016/03/11 14:21
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

#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr
	: public CBase
{
public:
	DECLARE_SINGLETON(CCollisionMgr)

private:
	typedef struct tagOBB
	{
		D3DXVECTOR3		vPoint[8];
		D3DXVECTOR3		vCenter;	
		D3DXVECTOR3		vProjAxis[3];
		D3DXVECTOR3		vAxis[3];
	}OBB;

private:
	CCollisionMgr(void);
	~CCollisionMgr(void);

public:
	bool Collision_OBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax, const D3DXMATRIX* pDestWorld
		, const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax, const D3DXMATRIX* pSourWorld);

	bool Collision_AABB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax
		, const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax);

private:
	void InitPoint(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax);
	void InitAxis(OBB* pOBB);

private:
	OBB			m_OBB[2];

public:
	virtual void Free(void);
};

END

#endif // CollisionMgr_h__