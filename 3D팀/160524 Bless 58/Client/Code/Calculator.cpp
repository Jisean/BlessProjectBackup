#include "stdafx.h"
#include "Calculator.h"
#include "Export_Function.h"
#include "Stage.h"
#include "TerrainTex.h"

CCalculator::CCalculator(void)
{

}

CCalculator::~CCalculator(void)
{

}

_float CCalculator::Compute_HeightOnTerrain(const _vec3* pPosition, const TCHAR* pTerrainTag)
{
	Engine::CVIBuffer* pBufferCom = (Engine::CVIBuffer*)Engine::Get_Component(LAYER_GAMELOGIC, pTerrainTag, L"Buffer");

	const _vec3* pVtxPos = pBufferCom->Get_VtxPosArrayPointer();

	_ulong dwVtxCntX = ((Engine::CTerrainTex*)pBufferCom)->Get_VtxCntX();
	_ulong dwVtxCntZ = ((Engine::CTerrainTex*)pBufferCom)->Get_VtxCntZ();

	_int		iIndex = int(pPosition->z / VTXITV) * dwVtxCntX + _int(pPosition->x / VTXITV);

	_float		fRatioX = (pPosition->x - pVtxPos[iIndex + dwVtxCntX].x) / VTXITV;
	_float		fRatioZ = (pVtxPos[iIndex + dwVtxCntX].z - pPosition->z) / VTXITV;

	// 평면을 정의한느 구조체 : D3DXPLANE
	D3DXPLANE			Plane;
	float				fDistance;

	if(fRatioX > fRatioZ)	
		D3DXPlaneFromPoints(&Plane, &pVtxPos[iIndex + dwVtxCntX], &pVtxPos[iIndex + dwVtxCntX + 1], &pVtxPos[iIndex + 1]);
	else
		D3DXPlaneFromPoints(&Plane, &pVtxPos[iIndex + dwVtxCntX], &pVtxPos[iIndex + 1], &pVtxPos[iIndex]);
	
	fDistance = D3DXPlaneDotCoord(&Plane, pPosition); 

	return pPosition->y - fDistance;
}

CCalculator* CCalculator::Create(void)
{
	CCalculator* pInstance = new CCalculator;

	return pInstance;	
}

void CCalculator::Free(void)
{
	
}

