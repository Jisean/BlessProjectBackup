#include "HierarchyLoader.h"
#include <tchar.h>

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
	pMeshContainer->ppNormTexture = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->ppNormTexture, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	pMeshContainer->ppSpecTexture = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->ppSpecTexture, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);


	if(NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);

		for(DWORD i = 0; i < NumMaterials; ++i)
		{
			if(pMeshContainer->pMaterials[i].pTextureFilename == NULL)
				continue;

			TCHAR		szFullPath[MAX_PATH] = L"";
			TCHAR		szFileName[MAX_PATH] = L"";
			TCHAR		szNormName[MAX_PATH] = L"";
			TCHAR		szNormPath[MAX_PATH] = L"";
			TCHAR		szSpecPath[MAX_PATH] = L"";
			TCHAR		szSpecName[MAX_PATH] = L"";

			lstrcpy(szFullPath, m_pPath);
			MultiByteToWideChar(CP_ACP
				, 0, pMeshContainer->pMaterials[i].pTextureFilename
				, strlen(pMeshContainer->pMaterials[i].pTextureFilename)
				, szFileName, MAX_PATH);
			lstrcat(szFullPath, szFileName);

			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pMeshContainer->ppTexture[i]);

			_tcscpy_s(szNormName, MAX_PATH, szFileName);

			TCHAR szFinalNormName[256] = L"";
			TCHAR changeStr[256] = L"_D_";
			TCHAR changedStr[256] =L"_N_";
			TCHAR* first;
			TCHAR* last;

			for(_uint k=0;k<_tcsclen(szNormName);++k)
			{ //szFileName 이란 배열을 모두 훑어봅니다.
				first=&szNormName[k]; // 훑어보는 포인터 first.
				if(_tcsnccmp(first,changeStr,_tcsclen(changeStr))==0){ //first에서부터 찾는 단어의 크기까지 단어가 같다면
					//이제부터 바꿔버립니다.
					last = &szNormName[k+_tcsclen(changeStr)]; // last에 바꿔버릴 단어 이후의 시작을 찍습니다.
					_tcsnccpy_s(szFinalNormName, changedStr, _tcsclen(changedStr)); // last 이후로 바꿀 문자열을 넣고,
					_tcsnccat_s(szFinalNormName, last, _tcsclen(last)); // 그이후도 다 넣습니다.
					//_tcsnccpy(first, szFinalNormName, _tcsclen(szFinalNormName)); // 바꿀 문자열의 첫 위치부터 szFinalNormName를 넣으면 바뀝니다.
					first = szFinalNormName;
				}
			}

			lstrcpy(szNormPath, m_pPath);
			lstrcat(szNormPath, szNormName);

			if(_tcsclen(szFinalNormName) !=0)
			{
				hr = D3DXCreateTextureFromFile(m_pDevice, szNormPath, &pMeshContainer->ppNormTexture[i]);
				if(hr<0)
				{
					hr = D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Mesh/NullTexture/NullTexture.tga", &pMeshContainer->ppNormTexture[i]);
				}
			}
			else
			{
				TCHAR		szNormName2[MAX_PATH] = L"";
				TCHAR		szNormPath2[MAX_PATH] = L"";
				_tcscpy_s(szNormName2, MAX_PATH, szFileName);

				TCHAR szFinalNormName2[256] = L"";
				TCHAR changeStr2[256] = L"_d_";
				TCHAR changedStr2[256] =L"_N_";
				TCHAR* first2;
				TCHAR* last2;

				for(_uint k=0;k<_tcsclen(szNormName2);++k)
				{ //szFileName 이란 배열을 모두 훑어봅니다.
					first2=&szNormName2[k]; // 훑어보는 포인터 first.
					if(_tcsnccmp(first2,changeStr2,_tcsclen(changeStr2))==0){ //first에서부터 찾는 단어의 크기까지 단어가 같다면
						//이제부터 바꿔버립니다.
						last2 = &szNormName2[k+_tcsclen(changeStr2)]; // last에 바꿔버릴 단어 이후의 시작을 찍습니다.
						_tcsnccpy_s(szFinalNormName2,changedStr2,_tcsclen(changedStr2)); // last 이후로 바꿀 문자열을 넣고,
						_tcsnccat_s(szFinalNormName2,last2,_tcsclen(last2)); // 그이후도 다 넣습니다.
						//_tcsnccpy(first2,szFinalNormName2,_tcsclen(szFinalNormName2)); // 바꿀 문자열의 첫 위치부터 szFinalNormName를 넣으면 바뀝니다.
						first2 = szFinalNormName2;
					}
				}

				lstrcpy(szNormPath2, m_pPath);
				lstrcat(szNormPath2, szNormName2);

				hr = D3DXCreateTextureFromFile(m_pDevice, szNormPath2, &pMeshContainer->ppNormTexture[i]);
				if(hr<0)
				{
					hr = D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Mesh/NullTexture/NullTexture.tga", &pMeshContainer->ppNormTexture[i]);
				}

				memset(szFinalNormName2,'\0',sizeof(szFinalNormName2)); // 임시배열 초기화
				memset(szNormName2,'\0',sizeof(szNormName2));
			}

			memset(szFinalNormName,'\0',sizeof(szFinalNormName)); // 임시배열 초기화
			memset(szNormName,'\0',sizeof(szNormName));


			_tcscpy_s(szSpecName, MAX_PATH, szFileName);

			TCHAR szFinalSpecName[256] = L"";
			TCHAR changeStr3[256] = L"_D_";
			TCHAR changedStr3[256] =L"_SP_";
			TCHAR* first3;
			TCHAR* last3;

			for(_uint k=0;k<_tcsclen(szSpecName);++k)
			{ //szFileName 이란 배열을 모두 훑어봅니다.
				first3=&szSpecName[k]; // 훑어보는 포인터 first3.
				if(_tcsnccmp(first3,changeStr3,_tcsclen(changeStr3))==0){ //first3에서부터 찾는 단어의 크기까지 단어가 같다면
					//이제부터 바꿔버립니다.
					last3 = &szSpecName[k+_tcsclen(changeStr3)]; // last3에 바꿔버릴 단어 이후의 시작을 찍습니다.
					_tcsnccpy_s(szFinalSpecName,changedStr3,_tcsclen(changedStr3)); // last3 이후로 바꿀 문자열을 넣고,
					_tcsnccat_s(szFinalSpecName,last3,_tcsclen(last3)); // 그이후도 다 넣습니다.
					//_tcsnccpy(first3,szFinalSpecName,_tcsclen(szFinalSpecName)); // 바꿀 문자열의 첫 위치부터 szFinalSpecName를 넣으면 바뀝니다.
					first3 = szFinalSpecName;
				}
			}

			lstrcpy(szSpecPath, m_pPath);
			lstrcat(szSpecPath, szSpecName);

			if(_tcsclen(szFinalSpecName) !=0)
			{
				hr = D3DXCreateTextureFromFile(m_pDevice, szSpecPath, &pMeshContainer->ppSpecTexture[i]);
				if(hr<0)
				{
					hr = D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Mesh/NullTexture/NullTexture.tga", &pMeshContainer->ppSpecTexture[i]);
				}
			}
			else
			{
				TCHAR		szSpecName2[MAX_PATH] = L"";
				TCHAR		szSpecPath2[MAX_PATH] = L"";
				_tcscpy_s(szSpecName2, MAX_PATH, szFileName);

				TCHAR szFinalSpecName2[256] = L"";
				TCHAR changeStr4[256] = L"_d_";
				TCHAR changedStr4[256] =L"_SP_";
				TCHAR* first4;
				TCHAR* last4;

				for(_uint k=0;k<_tcsclen(szSpecName2);++k)
				{ //szFileName 이란 배열을 모두 훑어봅니다.
					first4=&szSpecName2[k]; // 훑어보는 포인터 first3.
					if(_tcsnccmp(first4,changeStr4,_tcsclen(changeStr4))==0){ //first3에서부터 찾는 단어의 크기까지 단어가 같다면
						//이제부터 바꿔버립니다.
						last4 = &szSpecName2[k+_tcsclen(changeStr4)]; // last3에 바꿔버릴 단어 이후의 시작을 찍습니다.
						_tcsnccpy_s(szFinalSpecName2,changedStr4,_tcsclen(changedStr4)); // last3 이후로 바꿀 문자열을 넣고,
						_tcsnccat_s(szFinalSpecName2,last4,_tcsclen(last4)); // 그이후도 다 넣습니다.
						//_tcsnccpy(first4,szFinalSpecName2,_tcsclen(szFinalSpecName2)); // 바꿀 문자열의 첫 위치부터 szFinalSpecName를 넣으면 바뀝니다.
						first4 = szFinalSpecName2;
					}
				}

				lstrcpy(szSpecPath2, m_pPath);
				lstrcat(szSpecPath2, szSpecName2);

				hr = D3DXCreateTextureFromFile(m_pDevice, szSpecPath2, &pMeshContainer->ppSpecTexture[i]);
				if(hr<0)
				{
					hr = D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Mesh/NullTexture/NullTexture.tga", &pMeshContainer->ppSpecTexture[i]);
				}

				memset(szFinalSpecName2,'\0',sizeof(szFinalSpecName2)); // 임시배열 초기화
				memset(szSpecName2,'\0',sizeof(szSpecName2));
			}

			memset(szFinalSpecName,'\0',sizeof(szFinalSpecName)); // 임시배열 초기화
			memset(szSpecName,'\0',sizeof(szSpecName));


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

		DWORD matVertInfluences = 0;
		DWORD numBoneComboEntries = 0;

		pSkinInfo->ConvertToIndexedBlendedMesh(pMeshData->pMesh, 
												D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
												100,  // 상수...;;
												NULL,
												NULL,
												NULL,
												NULL,
												&matVertInfluences,
												&numBoneComboEntries,
												&pMeshContainer->pBoneCombinationBuf,
												&pMeshContainer->MeshData.pMesh);

		pMeshContainer->iBoneInfluences = matVertInfluences;

		pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
	
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
		Safe_Release(pMeshContainer->ppNormTexture[i]);
		Safe_Release(pMeshContainer->ppSpecTexture[i]);
	}

	if(pMeshContainer->pBoneCombinationBuf)
	{
		pMeshContainer->pBoneCombinationBuf->Release();	
	}
	Safe_Delete_Array(pMeshContainer->ppTexture);
	Safe_Delete_Array(pMeshContainer->ppNormTexture);
	Safe_Delete_Array(pMeshContainer->ppSpecTexture);
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

