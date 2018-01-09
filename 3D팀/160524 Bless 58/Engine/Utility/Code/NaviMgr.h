/*!
 * \file NaviMgr.h
 * \date 2016/03/09 13:14
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

#ifndef NaviMgr_h__
#define NaviMgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)
class CNaviCell;
class ENGINE_DLL CNaviMgr : public CBase
{
public:
	DECLARE_SINGLETON(CNaviMgr)

private:
	explicit CNaviMgr(void);
	virtual ~CNaviMgr(void);

public:
	void Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize);
	HRESULT Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwType);
	void Link_Cell(void);
	void Render(void);
	DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir
		, const DWORD& dwCurrentIdx, float fHeight);
	DWORD GetCellIndex(D3DXVECTOR3* pPos);
	DWORD GetCellType(DWORD dwIndex);

private:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	DWORD		m_dwReserveSize;

	typedef vector<CNaviCell*>		VECCELL;
	VECCELL		m_vecNaviMesh;

private:
	DWORD		m_dwIdxCnt;

private:
	virtual void Free(void);
};

END

#endif // NaviMgr_h__