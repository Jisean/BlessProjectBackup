#include "HierarchyLoader.h"

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath)
: m_pDevice(pDevice)
, m_pPath(pPath)
{

}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	DERIVED_FRAME*		pNewFrame = new DERIVED_FRAME;
	ZeroMemory(pNewFrame, sizeof(DERIVED_FRAME));

	if(Name != NULL)
	{
		AllocateName(Name, &pNewFrame->Name);
	}

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	pNewFrame->CombinedMatrix = pNewFrame->TransformationMatrix = matIdentity;

	*ppNewFrame = pNewFrame;
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name
														   , CONST D3DXMESHDATA *pMeshData
														   , CONST D3DXMATERIAL *pMaterials
														   , CONST D3DXEFFECTINSTANCE *pEffectInstances
														   , DWORD NumMaterials
														   , CONST DWORD *pAdjacency
														   , LPD3DXSKININFO pSkinInfo
														   , LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	LPD3DXMESH		pMesh = pMeshData->pMesh;

	if(pMesh->GetFVF() == 0)
		return E_FAIL;

	HRESULT		hr = NULL;
	DERIVED_MESHCONTAINER*		pMeshContainer = new DERIVED_MESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(DERIVED_MESHCONTAINER));
	
	if(Name != NULL)
		AllocateName(Name, &pMeshContainer->Name);

	//인접점의 정보
	DWORD	dwNumFaces = pMesh->GetNumFaces();
	pMeshContainer->pAdjacency = new DWORD[dwNumFaces * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwNumFaces * 3);

	//속성정보 셋팅
	pMeshContainer->MeshData.Type =	D3DXMESHTYPE_MESH;	
	if( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, m_pDevice
			, &pMeshContainer->MeshData.pMesh);
		FAILED_CHECK(hr);

		D3DXComputeNormals(pMeshContainer->MeshData.pMesh, pMeshContainer->pAdjacency);
	}
	else
	{
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), m_pDevice
			, &pMeshContainer->MeshData.pMesh);
		FAILED_CHECK(hr);

		pMesh->AddRef();
	}

	//재질정보 셋팅
	pMeshContainer->NumMaterials = max(NumMaterials, 1);

	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);
	pMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	if(NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);

		for(DWORD i = 0; i < NumMaterials; ++i)
		{
			if(pMeshContainer->pMaterials[i].pTextureFilename == NULL)
				continue;

			TCHAR		szFullPath[MAX_PATH] = L"";
			TCHAR		szFileName[MAX_PATH] = L"";

			lstrcpy(szFullPath, m_pPath);
			MultiByteToWideChar(CP_ACP
				, 0, pMeshContainer->pMaterials[i].pTextureFilename
				, strlen(pMeshContainer->pMaterials[i].pTextureFilename)
				, szFileName, MAX_PATH);
			lstrcat(szFullPath, szFileName);

			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pMeshContainer->ppTexture[i]);
			FAILED_CHECK(hr);
		}
	}
	else
	{
		ZeroMemory(&pMeshContainer->pMaterials[0].MatD3D, sizeof(D3DXMATERIAL));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.a = 1.f;
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
	}
	
	//스키닝정보 셋팅
	if(pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOriMesh = pMesh;

		DWORD		dwNumBones = pSkinInfo->GetNumBones();

		pMeshContainer->pFrameOffsetMatrix = new D3DXMATRIX[dwNumBones];
		ZeroMemory(pMeshContainer->pFrameOffsetMatrix, sizeof(D3DXMATRIX) * dwNumBones);

		for(DWORD i = 0; i < dwNumBones; ++i)
		{
			pMeshContainer->pFrameOffsetMatrix[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}
	else
	{
		pMeshContainer->pSkinInfo = NULL;
		pMeshContainer->pFrameOffsetMatrix = NULL;
		pMeshContainer->pOriMesh = NULL;
	}

	*ppNewMeshContainer = pMeshContainer;
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if(pFrameToFree == NULL)
		return E_FAIL;

	if(pFrameToFree->pMeshContainer != NULL)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if(pFrameToFree->pFrameFirstChild != NULL)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if(pFrameToFree->pFrameSibling != NULL)
		DestroyFrame(pFrameToFree->pFrameSibling);

	Safe_Delete_Array(pFrameToFree->Name);
	Safe_Delete(pFrameToFree);
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	DERIVED_MESHCONTAINER*	pMeshContainer = static_cast<DERIVED_MESHCONTAINER*>(pMeshContainerToFree);

	for(DWORD i = 0; i < pMeshContainerToFree->NumMaterials; ++i)
	{
		Safe_Release(pMeshContainer->ppTexture[i]);
	}

	Safe_Delete_Array(pMeshContainer->ppTexture);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pMeshContainer->pResultMatrix);
	Safe_Delete_Array(pMeshContainer->pAdjacency);

	Safe_Release(pMeshContainer->pOriMesh);
	Safe_Release(pMeshContainer->MeshData.pPMesh);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete(pMeshContainer);
	return S_OK;
}

void Engine::CHierarchyLoader::AllocateName(const CHAR* const pName, CHAR** ppNewName)
{
	int		iLength = strlen(pName);

	*ppNewName = new CHAR[iLength + 1];

	strcpy_s(*ppNewName, iLength + 1, pName);
}

