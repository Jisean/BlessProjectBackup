/*!
 * \file Export_Resources.h
 * \date 2015/04/05 5:18
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
#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourceMgr.h"

BEGIN(Engine)

//======================== For.ResourceMgr =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
inline HRESULT Add_Buffer(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
						 , CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey
						 , const WORD& wCntX = 0, const WORD& wCntZ = 0, const WORD& wItv = 1);

inline HRESULT Add_Texture(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
						  , TEXTURETYPE eTextureType
						  , const TCHAR* pResourceKey
						  , const TCHAR* pFilePath
						  , const WORD& wCnt = 1);

inline HRESULT Add_Mesh(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
					   , MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName, bool bLoadByTxt = false);

//--General-------------------------------------------------------//
inline CResources* Find_Resource(const WORD& wContainerIdx, const TCHAR* pResourceKey);
inline CResources* Clone_Resource(const WORD& wContainerIdx, const TCHAR* pResourceKey);
inline HRESULT Reserve_ContainerSize(const WORD& wSize);
inline void Reset_Resource(const WORD& wContainerIdx);

//======================== For.Release_Resource =======================//
inline void Release_Resource(void);
inline void Release_Scene_Resource(void);

#include "Export_Resources.inl"

END

#endif // Export_Resources_h__