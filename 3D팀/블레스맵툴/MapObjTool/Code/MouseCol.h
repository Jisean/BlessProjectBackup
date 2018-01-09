/*!
 * \file MouseCol.h
 * \date 2016/02/03 21:29
 *
 * \author goknown
 * Contact: user@company.com
 *
 * \brief mousecol	툴 마우스 충돌
 *
 * TODO: long description
 *
 * \note
*/

#ifndef MouseCol_h__
#define MouseCol_h__

#include "Include.h"
#include "MapObjToolView.h"

namespace Engine
{
	class CGameObject;
	class CNaviCell;
}

//class CTCubeObj;
//class CCameraObserver;
class CMouseCol
{
private:
	int		m_iCntX;
	int		m_iCntZ;
	int		m_iItv;
public:
	CMouseCol(void);
	CMouseCol(LPDIRECT3DDEVICE9 pDevice);
	~CMouseCol(void);

private:
	//CCameraObserver*			m_pCamObserver;
	LPDIRECT3DDEVICE9			m_pDevice;
	D3DXVECTOR3					m_vPivotPos;
	D3DXVECTOR3					m_vRayDir;
	D3DXVECTOR3					m_CurMPos;
	//Engine::CInfoSubject*		m_pInfoSubject;

private:
	void	Translation_ViewSpace(void);
	void	Translation_Local(const D3DXMATRIX*	pmatWolrd);
	bool	Translation_ViewSpace2(void);

public:
	static POINT	GetMousePos(void);
	void PickTerrain(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx);
	void PickTerrainVertex(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx);
	bool PickObject(D3DXVECTOR3* pOut, const Engine::VTXTEX* pVertex, const D3DXMATRIX* pmatWorld);
	bool PickObject(D3DXVECTOR3* pOut, Engine::VTXCOL* pVertex, Engine::INDEX16* pIndex, const D3DXMATRIX* pmatWorld);

	bool PickTerrainIndex(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx);
	void PickSetTerrain(D3DXVECTOR3* pOut, Engine::VTXTEX* pTerrainVtx, LPDIRECT3DVERTEXBUFFER9* pVB, bool bUpDown, float fRange);
	//bool PickObject(D3DXVECTOR3* pOut, CToolsView::MAPCUBE* pMapCube, size_t iMode);
	void SetSize(int iCntX, int iCntZ, int iItv);
	bool MeshPick(D3DXVECTOR3* pOut, LPD3DXMESH pMesh, D3DXMATRIX maWorld);
	bool PickNaviCell(D3DXVECTOR3* pOut, Engine::CNaviCell* pCell, int* pOutIndex);
	bool PickOccBox(D3DXVECTOR3* pOut, Engine::VTXCOL* pVertex, Engine::INDEX16* pIndex, const D3DXMATRIX* pmatWorld);


public:
	HRESULT	Initialize(void);

private:
	void Release(void);
};

#endif // MouseCol_h__