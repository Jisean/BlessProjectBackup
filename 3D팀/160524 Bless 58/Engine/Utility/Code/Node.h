#ifndef Node_h__
#define Node_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

const _ulong MAX = -1;

class CFrustum;
class ENGINE_DLL CNode : public Engine::CBase
{
private:
	
public:
	enum NODE {NODE_LT, NODE_RT, NODE_LB, NODE_RB, NODE_END};
	enum CORNER {CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END};
	enum NEIGHBORCENTER {CENTER_LEFT, CENTER_TOP, CENTER_RIGHT, CENTER_BOTTOM, CENTER_END};
private:
	explicit CNode(void);
	virtual ~CNode(void);
public:
	void Init_Node(const _vec3* pVtxPos, const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB);
	void Decision_In(LPDIRECT3DDEVICE9 pGraphicDev, CFrustum* pFrustum, INDEX32* pIndex, _int* pTriCnt);
	void SetUp_NeighborCenterIdx(const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ);
	
public:
	static CNode* Create(const _vec3* pVtxPos, const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB);
private:
	CNode*			m_pChildNode[NODE_END];
	_ulong			m_dwCorner[CORNER_END];
	_ulong			m_dwCenter;
	_float			m_fRadius;
	const _vec3*	m_pVtxPos;
	_ulong			m_dwNeighborCenter[CENTER_END];
private:
	_bool LevelofDetail(LPDIRECT3DDEVICE9 pGraphicDev);
	_bool LevelofDetail(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCenter);
	void Compute_NeighborCenter(const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ);
public:
	virtual void Free(void);
};

END

#endif // Node_h__
