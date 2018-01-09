/*!
 * \file StaticMesh.h
 * \date 2016/02/25 14:31
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

#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CVIBuffer;

class ENGINE_DLL CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void RenderMesh_ForSDK(const D3DXMATRIX* pWorld);
	virtual void RenderMesh_ForShader(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorld);


private:
	void MakeBoundingBox(void);

private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	D3DMATERIAL9*			m_pMtrl;

private:
	LPD3DXBUFFER			m_pSubset;
	DWORD					m_dwSubsetCnt;
	LPD3DXEFFECT	m_pEffect;

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName);
	virtual void	Free(void);
};

END

#endif // StaticMesh_h__