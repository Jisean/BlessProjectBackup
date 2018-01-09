#include "Mesh.h"
#include "VIBuffer.h"

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
, m_pBoundingBox(NULL)
, m_vMin(0.f, 0.f, 0.f)
, m_vMax(0.f, 0.f, 0.f)
, m_bLoadByTxt(false)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
: CResources(rhs.m_pDevice)
, m_bLoadByTxt(false)
{

}

Engine::CMesh::~CMesh(void)
{

}

void Engine::CMesh::GetMinMax(D3DXVECTOR3* const pMin, D3DXVECTOR3* const pMax)
{
	*pMin = m_vMin;
	*pMax = m_vMax;
}

void Engine::CMesh::SetBoundingBoxColor(DWORD dwColor)
{
	

	m_pBoundingBox->Change_BoxColor(dwColor);
}

void Engine::CMesh::Free(void)
{

}
