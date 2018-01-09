#include "TerrainBuffer.h"

Engine::CTerrainBuffer::CTerrainBuffer(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CTerrainBuffer::~CTerrainBuffer(void)
{

}

Engine::CResources* Engine::CTerrainBuffer::CloneResource(void)
{

	return new CTerrainBuffer(*this);
}

HRESULT Engine::CTerrainBuffer::CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv)
{
	m_dwVtxSize		= sizeof(VTXTEX);
	m_dwVtxCnt		= wCntX * wCntZ;
	m_dwVtxFVF		= VTXFVF_TEX;
	m_dwTriCnt		= (wCntX - 1) * (wCntZ - 1) * 2;
	m_dwIdxSize		= sizeof(INDEX32);
	m_IdxFmt		= D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	int	 iIndex = 0;

	for(int z = 0; z < wCntZ; ++z)
	{
		for(int x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;

			pVtxTex[iIndex].vPos    = D3DXVECTOR3(float(x) * wItv, 0.f, float(z) * wItv);
			pVtxTex[iIndex].vNormal = D3DXVECTOR3(0.f, 0.f, 0.f);
			pVtxTex[iIndex].vTexUV	= D3DXVECTOR2((float)x / (wCntX - 1.f) * 8.f,(float) z /(wCntZ - 1.f) * 8.f); 

		}
	}

	INDEX32*	pIndex = NULL;

	m_pIB->Lock(0, 0 , (void**)&pIndex, 0);

	int	iTriCnt = 0;

	for(int z = 0; z < wCntZ - 1; ++z)
	{
		for(int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + wCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;

			D3DXVECTOR3		vDest, vSour, vNormal;
			vDest = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPos - pVtxTex[pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;

			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			vDest = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPos - pVtxTex[pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;

		}
	}

	for(DWORD i = 0; i < m_dwVtxCnt; ++i)
	{
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	//Engine::Safe_Delete_Array(pdwPixel);

	return S_OK;
}

Engine::CTerrainBuffer* Engine::CTerrainBuffer::Create(LPDIRECT3DDEVICE9 pDevice, 
													   const WORD& wCntX, const WORD& wCntZ, const WORD& vItv)
{
	CTerrainBuffer*		pTerrainBuffer = new CTerrainBuffer(pDevice);
	if(FAILED(pTerrainBuffer->CreateBuffer(wCntX, wCntZ, vItv)))
		Engine::Safe_Delete(pTerrainBuffer);

	return pTerrainBuffer;
}