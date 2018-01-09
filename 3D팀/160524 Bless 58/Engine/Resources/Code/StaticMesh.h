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
#define _CRT_SECURE_NO_WARNINGS
#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
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
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt);
	HRESULT LoadEffectMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void RenderMesh_ForSDK(const D3DXMATRIX* pWorld);
	virtual void RenderMesh_ForShader(LPD3DXEFFECT pEffect, const _matrix* pBoudingBoxMatrix);
	virtual void RenderMesh_ForShader(CTexture* pTexture, CTexture* pMaskTexture, CTexture* pNormalTexture, LPD3DXEFFECT pEffect, const _matrix* pBoudingBoxMatrix, _uint cPasses = 0, _uint iPassIndex = 0);
	//========Get========//
	LPD3DXMESH* Get_Mesh(void){ return &m_pMesh; }
	LPDIRECT3DTEXTURE9* Get_Texture(DWORD i) { return &m_ppTexture[i]; }
	D3DMATERIAL9*		Get_Mtrl(DWORD i) { return &m_pMtrl[i]; }
	//========Set========//
	void	Set_BoxRender(bool bBoxRender){ m_bBoxRender = bBoxRender; }
private:
	void MakeBoundingBox(void);

private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	LPDIRECT3DTEXTURE9*		m_ppNormTexture;
	LPDIRECT3DTEXTURE9*		m_ppSpecTexture;
	D3DMATERIAL9*			m_pMtrl;

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pSubset;
	DWORD					m_dwSubsetCnt;
	bool					m_bEffect;
	bool					m_bBoxRender;

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt);
	static CStaticMesh* CreateEffect(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName);
	virtual void	Free(void);
};

END

#endif // StaticMesh_h__