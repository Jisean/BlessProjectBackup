/*!
 * \file HierarchyLoader.h
 * \date 2016/03/02 13:46
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

#ifndef HierarchyLoader_h__
#define HierarchyLoader_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyLoader
	: public ID3DXAllocateHierarchy
{
public:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath);
	~CHierarchyLoader(void);

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, 
		LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	 STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree); 
	 STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree); 

private:
	void AllocateName(const CHAR* const pName, CHAR** ppNewName);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	const TCHAR*			m_pPath;
};

END

#endif // HierarchyLoader_h__