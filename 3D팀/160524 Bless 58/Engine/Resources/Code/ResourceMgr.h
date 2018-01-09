/*!
 * \file ResourceMgr.h
 * \date 2015/04/03 16:11
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
#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Include.h"
#include "VIBuffer.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CResourceMgr : public CBase
{
	DECLARE_SINGLETON(CResourceMgr)
private:
	enum RESOURCETYPE {RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_STATIC, RESOURCE_UI, RESOURCE_REMAIN, RESOURCE_END};
private:
	CResourceMgr(void);
	~CResourceMgr(void);

public:
	CResources* FindResource(const WORD& wContainerIdx, const TCHAR* pResourceKey);
	CResources* CloneResource(const WORD& wContainerIdx, const TCHAR* pResourceKey);

public:
	HRESULT ReserveContainerSize(const WORD& wSize);

public:
	HRESULT AddBuffer(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
		, CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey
		, const WORD& wCntX = 0, const WORD& wCntZ = 0, const WORD& wItv = 1);

	HRESULT AddTexture(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
		, TEXTURETYPE eTextureType
		, const TCHAR* pResourceKey
		, const TCHAR* pFilePath
		, const WORD& wCnt = 1);

	HRESULT AddMesh(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
		, MESHTYPE eMeshType
		, const TCHAR* pMeshKey
		, const TCHAR* pFilePath
		, const TCHAR* pFileName
		, bool bLoadByTxt = false);

public:
	void ResourceReset(const WORD& wContainerIdx);

private:
	typedef HTable		HTRESOURCE;
	HTRESOURCE*			m_phtResource;
	WORD				m_wReservedSize;

private:
	virtual void Free(void);
};

END

#endif // ResourceMgr_h__