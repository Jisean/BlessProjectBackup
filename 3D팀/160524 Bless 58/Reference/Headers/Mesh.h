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

BEGIN(Engine)

class CVIBuffer;

class ENGINE_DLL CMesh
	: public CResources
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh(void);

public:
	void GetMinMax(D3DXVECTOR3* const pMin, D3DXVECTOR3* const pMax);
	void SetBoundingBoxColor(DWORD dwColor);

public:
	virtual CResources* CloneResource(void) PURE;

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt = false) PURE;
	virtual void RenderMesh_ForSDK(const D3DXMATRIX* pWorld) PURE;

protected:
	D3DXVECTOR3				m_vMin, m_vMax;
	CVIBuffer*				m_pBoundingBox;
	bool					m_bLoadByTxt;

public:
	virtual void	Free(void);
};

END

#endif // Mesh_h__