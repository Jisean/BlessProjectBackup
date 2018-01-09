/*!
 * \file QuadTree.h
 * \date 2016/02/26 13:14
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
#ifndef QuadTree_h__
#define QuadTree_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CQuadTree : public CBase
{
public:
	DECLARE_SINGLETON(CQuadTree)

public:
	enum CHILD {CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END};
	enum CORNER {CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END};
	enum NEIGHBOR {NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END};

private:
	CQuadTree(void);
	~CQuadTree(void);

public:
	HRESULT InitNeighbor(const VTXTEX* pTerrainVtx, CQuadTree* pParentQuad
		, const WORD& wCntX, const WORD& wCntZ);

	HRESULT InitQuadTree(const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);
	void IsinFrustum(const VTXTEX* pTerrainVtx);
	void CullingToQuadTree(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);
	void Culling_LOD(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);

private:
	CQuadTree*		m_pChild[CHILD_END];
	CQuadTree*		m_pNeighbor[NEIGHBOR_END];
	WORD			m_wCorner[CORNER_END];
	int				m_iCenter;
	float			m_fRadius;
	bool			m_isIn;

private:
	bool LevelofDetail(const VTXTEX* pTerrainVtx);
	bool MakeNeighbor(NEIGHBOR eNeighborID
		, int& iLT, int& iRT, int& iLB, int& iRB
		, const WORD& wCntX, const WORD& wCntZ);
	CQuadTree* FindNeighbor(const VTXTEX* pTerrainVtx
		, const int& iLT, const int& iRT, const int& iLB, const int& iRB
		, const int& iCenter);

	void SetChild(const VTXTEX* pTerrainVtx);
	CQuadTree* MakeChild(CHILD eChild);
	virtual void Free(void);
};

END

#endif // QuadTree_h__