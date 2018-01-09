#ifndef QuadTree_h__
#define QuadTree_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CNode;
class CFrustum;
class ENGINE_DLL CQuadTree : public CBase
{
	DECLARE_SINGLETON(CQuadTree)
private:
	explicit CQuadTree(void);
	~CQuadTree(void);
public:
	HRESULT Initialize_QuadTree(const _vec3* pVtxPos, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ);
	void Decision_In(LPDIRECT3DDEVICE9 pGraphicDev, INDEX32* pIndex, _int* pTriCnt);	
private:
	CNode*			m_pParentNode;
private:
	void SetUp_NeighborCenterIdx(const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ);
public:
	virtual void Free(void);
};

END

#endif // QuadTree_h__
