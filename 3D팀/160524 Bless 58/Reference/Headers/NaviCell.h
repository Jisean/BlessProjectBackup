/*!
 * \file NaviCell.h
 * \date 2016/03/09 13:16
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

#ifndef NaviCell_h__
#define NaviCell_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CLine2D;
class ENGINE_DLL CNaviCell : public CBase
{
public:
	enum POINT {POINT_A, POINT_B, POINT_C, POINT_END};
	enum LINE {LINE_AB, LINE_BC, LINE_CA, LINE_END};
	enum NEIGHBOR {NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};

private:
	explicit CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);
	virtual ~CNaviCell(void);

public:
	const D3DXVECTOR3* GetPoint(POINT ePointID);
	CNaviCell* GetNeighbor(NEIGHBOR eNeighbor);
	DWORD GetIndex(void);
	Engine::CLine2D*	GetLine(LINE eLineID);
	DWORD GetCellType(void);

public:
	void SetNeighbor(NEIGHBOR eNeighborID, CNaviCell* pNeighbor);

public:
	HRESULT Init_Cell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx,const DWORD& dwType);
	bool ComparePoint(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, CNaviCell* pNeighbor);
	bool CheckPass(const D3DXVECTOR3* pPos
		, const D3DXVECTOR3* pDir
		, NEIGHBOR* peNeighborID);
	void Render(void);
	D3DXVECTOR3 SlidingVector(D3DXVECTOR3& vDir, DWORD dwNeighborID);
	bool CheckPass(const D3DXVECTOR3* pPos 
		, const D3DXVECTOR3* pDir 
		, DWORD dwNeighborID);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CLine2D*				m_pLine2D[LINE_END];
	CNaviCell*				m_pNeighbor[NEIGHBOR_END];

private:
	D3DXVECTOR3			m_vPoint[POINT_END];
	LPD3DXLINE			m_pLine;
	DWORD				m_dwIndex;
	DWORD				m_dwType;

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC
		, const DWORD& dwIdx, const DWORD& dwType);

private:
	virtual void Free(void);
	
};

END

#endif // NaviCell_h__