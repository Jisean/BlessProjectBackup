#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(Engine::CCollisionMgr)

Engine::CCollisionMgr::CCollisionMgr(void)
{

}

Engine::CCollisionMgr::~CCollisionMgr(void)
{

}

bool Engine::CCollisionMgr::Collision_AABB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax)
{
	float		fMin = 0.f;
	float		fMax = 0.f;

	//xÃà
	fMin = max(pDestMin->x, pSourMin->x);
	fMax = min(pDestMax->x, pSourMax->x);
	if(fMin > fMax)
		return false;

	//yÃà
	fMin = max(pDestMin->y, pSourMin->y);
	fMax = min(pDestMax->y, pSourMax->y);
	if(fMin > fMax)
		return false;

	//zÃà
	fMin = max(pDestMin->z, pSourMin->z);
	fMax = min(pDestMax->z, pSourMax->z);
	if(fMin > fMax)
		return false;

	return true;
}

bool Engine::CCollisionMgr::Collision_OBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax
										  , const D3DXMATRIX* pDestWorld 
										  , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax
										  , const D3DXMATRIX* pSourWorld)
{
	ZeroMemory(m_OBB, sizeof(OBB) * 2);

	InitPoint(&m_OBB[0], pDestMin, pDestMax);
	InitPoint(&m_OBB[1], pSourMin, pSourMax);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_OBB[0].vPoint[i], &m_OBB[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&m_OBB[1].vPoint[i], &m_OBB[1].vPoint[i], pSourWorld);
	}
	D3DXVec3TransformCoord(&m_OBB[0].vCenter, &m_OBB[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&m_OBB[1].vCenter, &m_OBB[1].vCenter, pSourWorld);

	InitAxis(&m_OBB[0]);
	InitAxis(&m_OBB[1]);

	float		fDistance[3];

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[0], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[1], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[2], &m_OBB[i].vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[0], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[1], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[2], &m_OBB[i].vAxis[j]));

			D3DXVECTOR3		vTemp = m_OBB[1].vCenter - m_OBB[0].vCenter;

			fDistance[2] = fabs(D3DXVec3Dot(&vTemp, &m_OBB[i].vAxis[j]));

			if(fDistance[2] > fDistance[0] + fDistance[1])
				return false;
		}
	}
	return true;
}

void Engine::CCollisionMgr::InitPoint(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax)
{
	pOBB->vPoint[0] = D3DXVECTOR3(pMin->x, pMax->y, pMin->z);
	pOBB->vPoint[1] = D3DXVECTOR3(pMax->x, pMax->y, pMin->z);
	pOBB->vPoint[2] = D3DXVECTOR3(pMax->x, pMin->y, pMin->z);
	pOBB->vPoint[3] = D3DXVECTOR3(pMin->x, pMin->y, pMin->z);

	pOBB->vPoint[4] = D3DXVECTOR3(pMin->x, pMax->y, pMax->z);
	pOBB->vPoint[5] = D3DXVECTOR3(pMax->x, pMax->y, pMax->z);
	pOBB->vPoint[6] = D3DXVECTOR3(pMax->x, pMin->y, pMax->z);
	pOBB->vPoint[7] = D3DXVECTOR3(pMin->x, pMin->y, pMax->z);

	for(int i = 0; i < 8; ++i)
	{
		pOBB->vCenter += pOBB->vPoint[i];
	}
	pOBB->vCenter *= 0.125f;
}

void Engine::CCollisionMgr::InitAxis(OBB* pOBB)
{
	//+x
	pOBB->vProjAxis[0] = (pOBB->vPoint[1] + pOBB->vPoint[2] + pOBB->vPoint[5] + pOBB->vPoint[6]) * 0.25 - pOBB->vCenter;
	pOBB->vAxis[0] = pOBB->vPoint[1] - pOBB->vPoint[0];

	//+y
	pOBB->vProjAxis[1] = (pOBB->vPoint[0] + pOBB->vPoint[1] + pOBB->vPoint[4] + pOBB->vPoint[5]) * 0.25 - pOBB->vCenter;
	pOBB->vAxis[1] = pOBB->vPoint[0] - pOBB->vPoint[3];

	//+z
	pOBB->vProjAxis[2] = (pOBB->vPoint[4] + pOBB->vPoint[5] + pOBB->vPoint[6] + pOBB->vPoint[7]) * 0.25 - pOBB->vCenter;
	pOBB->vAxis[2] = pOBB->vPoint[7] - pOBB->vPoint[3];

	for(int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pOBB->vAxis[i], &pOBB->vAxis[i]);
}

void Engine::CCollisionMgr::Free(void)
{
	
}
