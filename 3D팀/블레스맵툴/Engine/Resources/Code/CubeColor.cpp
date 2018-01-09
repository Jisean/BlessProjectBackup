#include "CubeColor.h"

Engine::CCubeColor::CCubeColor(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CCubeColor::~CCubeColor(void)
{
	
}

Engine::CResources* Engine::CCubeColor::CloneResource(void)
{
	return new CCubeColor(*this);
}

HRESULT Engine::CCubeColor::CreateBuffer(const D3DXVECTOR3& vMin
										 , const D3DXVECTOR3& vMax
										 , const DWORD& dwColor)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = VTXFVF_COL;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = 12;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	/*
	(-1, 1)		(1, 1)
			----
			|	|
			----
	(-1, -1)	(1, -1)
	*/

	VTXCOL*		pVtxCube = NULL;
	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
	pVtxCube[0].dwColor = dwColor;

	pVtxCube[1].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
	pVtxCube[1].dwColor = dwColor;

	pVtxCube[2].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
	pVtxCube[2].dwColor = dwColor;

	pVtxCube[3].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
	pVtxCube[3].dwColor = dwColor;


	pVtxCube[4].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
	pVtxCube[4].dwColor = dwColor;   

	pVtxCube[5].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
	pVtxCube[5].dwColor = dwColor;   

	pVtxCube[6].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
	pVtxCube[6].dwColor = dwColor;   

	pVtxCube[7].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
	pVtxCube[7].dwColor = dwColor;
	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	/*
		4		5
	0-------1
	| 	7	|	6
	|		|
	3-------2
	*/

	//+x
	pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
	pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;

	//-x
	pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
	pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;

	//+y
	pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
	pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;

	//-y
	pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
	pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;

	//+z
	pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
	pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;

	//-z
	pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
	pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CCubeColor* Engine::CCubeColor::Create(LPDIRECT3DDEVICE9 pDevice
											   , const D3DXVECTOR3& vMin/* = D3DXVECTOR3(-1.f, -1.f, -1.f)*/
											   , const D3DXVECTOR3& vMax/* = D3DXVECTOR3(1.f, 1.f, 1.f)*/
											   , const DWORD& dwColor/* = D3DXCOLOR(1.f, 0.f, 1.f, 1.f)*/)
{
	CCubeColor* pBuffer = new CCubeColor(pDevice);
	if(FAILED(pBuffer->CreateBuffer(vMin, vMax, dwColor)))
		Engine::Safe_Release(pDevice);

	return pBuffer;
}

void Engine::CCubeColor::Free(void)
{
	Engine::CVIBuffer::Free();
}
