

//======================== For.ResourceMgr =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
HRESULT Add_Buffer(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
				  , CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey
				  , const WORD& wCntX/* = 0*/, const WORD& wCntZ/* = 0*/, const WORD& wItv/* = 1*/)
{
	//return CResourceMgr::GetInstance()->AddBuffer(pDevice, wContainerIdx, eBufferType, pResourceKey, wCntX = 0, wCntZ = 0, wItv = 1);
	return CResourceMgr::GetInstance()->AddBuffer(pDevice, wContainerIdx, eBufferType, pResourceKey, wCntX, wCntZ, wItv);
}

HRESULT Add_Texture(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
				   , TEXTURETYPE eTextureType
				   , const TCHAR* pResourceKey
				   , const TCHAR* pFilePath
				   , const WORD& wCnt/* = 1*/)
{
	return CResourceMgr::GetInstance()->AddTexture(pDevice, wContainerIdx, eTextureType, pResourceKey, pFilePath, wCnt);
}

HRESULT Add_Mesh(LPDIRECT3DDEVICE9 pDevice, const WORD& wContainerIdx
				, MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName, bool bLoadByTxt)
{
	return CResourceMgr::GetInstance()->AddMesh(pDevice, wContainerIdx, eMeshType, pMeshKey, pFilePath, pFileName, bLoadByTxt);
}

//--General-------------------------------------------------------//
CResources* Find_Resource(const WORD& wContainerIdx, const TCHAR* pResourceKey)
{
	return CResourceMgr::GetInstance()->FindResource(wContainerIdx, pResourceKey);
}

CResources* Clone_Resource(const WORD& wContainerIdx, const TCHAR* pResourceKey)
{
	return CResourceMgr::GetInstance()->CloneResource(wContainerIdx, pResourceKey);
}

HRESULT Reserve_ContainerSize(const WORD& wSize)
{	
	return CResourceMgr::GetInstance()->ReserveContainerSize(wSize);
}

void Reset_Resource(const WORD& wContainerIdx)
{
	CResourceMgr::GetInstance()->ResourceReset(wContainerIdx);
}

//======================== For.Release_Resource =======================//
void Release_Resource(void)
{
	CResourceMgr::GetInstance()->DestroyInstance();
}

void Release_Scene_Resource(void)
{
	CResourceMgr::GetInstance()->ResourceReset(0);
	CResourceMgr::GetInstance()->ResourceReset(1);
	CResourceMgr::GetInstance()->ResourceReset(2);
}