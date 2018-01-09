#include "QuadTree.h"
#include "Node.h"
#include "Frustum.h"

USING(Engine)
IMPLEMENT_SINGLETON(CQuadTree)

Engine::CQuadTree::CQuadTree(void)
{

}

Engine::CQuadTree::~CQuadTree(void)
{

}

HRESULT CQuadTree::Initialize_QuadTree(const _vec3* pVtxPos, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ)
{
	m_pParentNode = CNode::Create(pVtxPos, dwVtxCntX * dwVtxCntZ - dwVtxCntX, dwVtxCntX * dwVtxCntZ - 1, 0, dwVtxCntX - 1);

	SetUp_NeighborCenterIdx(dwVtxCntX, dwVtxCntZ);

	if(NULL == m_pParentNode)
		return E_FAIL;
	return S_OK;
}

void CQuadTree::Decision_In(LPDIRECT3DDEVICE9 pGraphicDev, INDEX32* pIndex, _int* pTriCnt)
{
	CFrustum* pFrustum = Engine::CFrustum::GetInstance();

	m_pParentNode->Decision_In(pGraphicDev, pFrustum, pIndex, pTriCnt);
}

void CQuadTree::SetUp_NeighborCenterIdx(const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ)
{
	if(NULL == m_pParentNode)
		return;

	m_pParentNode->SetUp_NeighborCenterIdx(dwVtxCntX, dwVtxCntZ);

	_int a = 10;
}

void Engine::CQuadTree::Free(void)
{
	Engine::Safe_Release(m_pParentNode);
}

