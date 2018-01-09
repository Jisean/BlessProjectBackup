#include "ResourceMgr.h"
#include "Resources.h"
#include "RcTex.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "CubeTex.h"
#include "TerrainBuffer.h"
#include "CubeColor.h"
#include "Trail.h"

IMPLEMENT_SINGLETON(Engine::CResourceMgr)

Engine::CResourceMgr::CResourceMgr(void)
: m_wReservedSize(0)
, m_phtResource(NULL)
{

}

Engine::CResourceMgr::~CResourceMgr(void)
{

}

Engine::CResources* Engine::CResourceMgr::FindResource(const WORD& wContainerIdx
													   , const TCHAR* pResourceKey)
{
	CResources* pResource = (CResources*)(m_phtResource[wContainerIdx].Search_TableData(pResourceKey));

	return pResource;
}

Engine::CResources* Engine::CResourceMgr::CloneResource(const WORD& wContainerIdx
														, const TCHAR* pResourceKey)
{
	CResources*		pResource = FindResource(wContainerIdx, pResourceKey);
	NULL_CHECK_RETURN(pResource, NULL);

	return pResource->CloneResource();
}

HRESULT Engine::CResourceMgr::ReserveContainerSize(const WORD& wSize)
{
	if(m_wReservedSize != 0	|| m_phtResource != NULL)
		return E_FAIL;

	m_phtResource = new HTable[wSize];
	for (int i = 0; i < wSize; ++i)
	{
		if((RESOURCETYPE)i == RESOURCE_STAGE || (RESOURCETYPE)i == RESOURCE_STATIC || (RESOURCETYPE)i == RESOURCE_REMAIN)
			m_phtResource[i].Ready_Table(1019);
		else
			m_phtResource[i].Ready_Table(199);
	}

	m_wReservedSize = wSize;
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice
										, const WORD& wContainerIdx 
										, CVIBuffer::BUFFERTYPE eBufferType
										, const TCHAR* pResourceKey 
										, const WORD& wCntX /*= 0*/
										, const WORD& wCntZ /*= 0*/
										, const WORD& wItv /*= 1*/)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_phtResource, E_FAIL, L"phtResource Not Failed")

	CResources*		pResource = FindResource(wContainerIdx, pResourceKey);
	if(pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"버퍼가 이미 추가 되어 있음");
		return E_FAIL;
	}
	pDevice->AddRef();

	CVIBuffer*		pBuffer = NULL;

	switch(eBufferType)
	{
	case CVIBuffer::BUFFER_RCTEX:
		pBuffer = CRcTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_TERRAINTEX:
		pBuffer = CTerrainTex::Create(pDevice, wCntX, wCntZ, wItv);
		break;

	case CVIBuffer::BUFFER_CUBETEX:
		pBuffer = CCubeTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_TERRAIN:
		pBuffer = CTerrainBuffer::Create(pDevice, wCntX, wCntZ, wItv);
		break;

	case CVIBuffer::BUFFER_CUBECOL:
		pBuffer = CCubeColor::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_TRAIL:
		pBuffer = CTrail::Create(pDevice);
		break;
	}
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_phtResource[wContainerIdx].Insert_Table(pResourceKey, pBuffer);
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice
										 , const WORD& wContainerIdx 
										 , TEXTURETYPE eTextureType
										 , const TCHAR* pResourceKey
										 , const TCHAR* pFilePath
										 , const WORD& wCnt /*= 1*/)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_phtResource, E_FAIL, L"m_phtResource Not Failed");

	CResources*		pResource = FindResource(wContainerIdx, pResourceKey);
	if(pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"텍스쳐가 이미 추가 되어 있음");
		return E_FAIL;
	}
	pDevice->AddRef();

	pResource = CTexture::Create(pDevice, eTextureType, pFilePath, wCnt);
	NULL_CHECK_RETURN_MSG(pResource, E_FAIL, L"리소스 할당 실패");

	m_phtResource[wContainerIdx].Insert_Table(pResourceKey, pResource);

	return S_OK;
}

HRESULT Engine::CResourceMgr::AddMesh(LPDIRECT3DDEVICE9 pDevice
									  , const WORD& wContainerIdx 
									  , MESHTYPE eMeshType
									  , const TCHAR* pMeshKey
									  , const TCHAR* pFilePath
									  , const TCHAR* pFileName
									  , bool bLoadByTxt/* = false*/)
{
	if(m_wReservedSize == NULL)
		return E_FAIL;

	CResources*	pResource = FindResource(wContainerIdx, pMeshKey);
	if(pResource != NULL)
		return E_FAIL;

	pDevice->AddRef();

	switch(eMeshType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pDevice, pFilePath, pFileName, bLoadByTxt);
		break;

	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pDevice, pFilePath, pFileName, bLoadByTxt);
		break;

	case MESH_EFFECT:
		pResource = CStaticMesh::CreateEffect(pDevice, pFilePath, pFileName);
		break;
	}
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_phtResource[wContainerIdx].Insert_Table(pMeshKey, pResource);
	return S_OK;
}


void Engine::CResourceMgr::ResourceReset(const WORD& wContainerIdx)
{
	size_t iTableSize =	m_phtResource[wContainerIdx].Get_TableSize();

	for(size_t i = 0; i < iTableSize; ++i)
	{
		CResources*		pResource = (CResources*)m_phtResource[wContainerIdx].Erase_Slot(i);

		if(NULL != pResource)
		{
			Safe_Release(pResource);
		}
	}
	m_phtResource[wContainerIdx].Clear_Table();
}

void Engine::CResourceMgr::Free(void)
{
	if(0 == m_wReservedSize)
		return;

	for (size_t i = 0; i < m_wReservedSize; ++i)
	{
		HTable	phtResource = m_phtResource[i];
		size_t iTableSize =	phtResource.Get_TableSize();

		for(size_t j = 0; j < iTableSize; ++j)
		{
			CResources*		pResource = (CResources*)phtResource.Erase_Slot(j);
			if(NULL != pResource)
				Safe_Release(pResource);
		}
		phtResource.Erase_Table();
	}	
	Engine::Safe_Delete_Array(m_phtResource);
}
