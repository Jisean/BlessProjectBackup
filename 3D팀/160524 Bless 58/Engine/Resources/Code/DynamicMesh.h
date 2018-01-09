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
	void SetAniSpeed(float fAniSpeed);
	void SetAniInfo(Engine::ANI_INFO AniInfo);

public:
	virtual CResources* CloneResource(void);
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt);
	virtual void RenderMesh_ForSDK(const D3DXMATRIX* pWorld);	
	virtual void RenderMesh_ForShader(LPD3DXEFFECT pEffect);	

public:
	void FrameMove(const float& fTimePerSec);

public://-----------------�߰�------------------//
	CAniCtrl* GetAniCtrl(void) { return m_pAniCtrl; }
	void	CreateVTFTexture(void);
private:
	void InitFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	void UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	void SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame);
	void Find_MeshContainer(DERIVED_FRAME* pFrame);
	void Render_MeshContainer(DERIVED_MESHCONTAINER* pMeshContainer);
	void MakeBoundingBox(D3DXFRAME* pFrame, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax);

private:
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;
	float					m_fAniSpeed;
	vector<Engine::DERIVED_FRAME*>		m_vecFrame;

private:
	LPD3DXFRAME		m_pRootBone;
	LPD3DXEFFECT	m_pEffect;
	//---------------- �߰�------------------//
LPDIRECT3DTEXTURE9		m_pMyMatrixTex;

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt);
	virtual void Free(void);
};

END

#endif // DynamicMesh_h__