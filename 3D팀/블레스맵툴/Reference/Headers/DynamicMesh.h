/*!
 * \file DynamicMesh.h
 * \date 2016/02/29 14:27
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

#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CHierarchyLoader;
class CAniCtrl;

class ENGINE_DLL CDynamicMesh
	: public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CDynamicMesh(const CDynamicMesh& rDynamicMesh);

public:
	virtual ~CDynamicMesh(void);

public:
	DOUBLE GetTrackPosition(void);
	DOUBLE GetPeriod(void);
	const D3DXMATRIX* FindFrame(const char* pFrameName);

public:
	void SetAnimationSet(UINT iAniIdx);
	void SetTrackPos(DOUBLE dPosition);

public:
	virtual CResources* CloneResource(void);
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void RenderMesh_ForSDK(const D3DXMATRIX* pWorld);	
	virtual void RenderMesh_ForShader(LPD3DXEFFECT pEffect);	

public:
	void FrameMove(const float& fTimePerSec);

private:
	void UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	void SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame);
	void Find_MeshContainer(DERIVED_FRAME* pFrame);
	void Render_MeshContainer(DERIVED_MESHCONTAINER* pMeshContainer);
	void MakeBoundingBox(D3DXFRAME* pFrame, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax);

private:
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;

private:
	LPD3DXFRAME		m_pRootBone;
	LPD3DXEFFECT	m_pEffect;

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName);
	virtual void Free(void);
};

END

#endif // DynamicMesh_h__