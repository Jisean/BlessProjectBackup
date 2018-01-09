/*!
 * \file Mesh.h
 * \date 2016/02/25 14:26
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

#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"
#include "CubeColor.h"


BEGIN(Engine)

class CVIBuffer;

class ENGINE_DLL CMesh
	: public CResources
{
public:
	MESHTYPE	m_eMeshType;
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh(void);

public:
	void GetMinMax(D3DXVECTOR3* const pMin, D3DXVECTOR3* const pMax);
	void SetBoundingBoxColor(DWORD dwColor);
	Engine::CCubeColor* GetBoundingBox(void)
	{
		if(m_pBoundingBox)
			return m_pBoundingBox;
		else
			return NULL;
	}
	LPD3DXMESH& GetMesh(void)
	{
		return m_pMesh;
	}

public:
	virtual CResources* CloneResource(void) PURE;

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName) PURE;
	virtual void RenderMesh_ForSDK(const D3DXMATRIX* pWorld) PURE;

protected:
	LPD3DXMESH				m_pMesh;
	D3DXVECTOR3				m_vMin, m_vMax;
	CCubeColor*				m_pBoundingBox;
	D3DXVECTOR3				m_vCenter;
	float					m_fRad;

public:
	virtual void	Free(void);
};

END

#endif // Mesh_h__