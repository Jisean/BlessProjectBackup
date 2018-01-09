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
	CNaviMgr(void);
	~CNaviMgr(void);

public:
	HRESULT Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize);
	HRESULT Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwType);
	void Link_Cell(void);
	void Unlink_Cell(void);
	void Render(void);
	DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir
		, const DWORD& dwCurrentIdx);
public:
	int GetSize(void);
	void GetNearPoint(D3DXVECTOR3& vPoint, float fRange);
	bool SetOnePoint(D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2, D3DXVECTOR3 vPoint3);
	bool SetTwoPoint(D3DXVECTOR2 vDir, Engine::CNaviCell* pNaviClose, D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2);
	void ChangePoint(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour);
	void DeletePoint(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour);
	void DeleteTriangle(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour);
	void ResetNavimesh(void);
	void ResetIndex(void);


private:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	DWORD		m_dwReserveSize;

	typedef vector<CNaviCell*>		VECCELL;
	VECCELL		m_vecNaviMesh;
public:
	vector<CNaviCell*>*	GetCell(void)
	{
		return &m_vecNaviMesh;
	}

private:
	DWORD		m_dwIdxCnt;

private:
	virtual void Free(void);
};

END

#endif // NaviMgr_h__