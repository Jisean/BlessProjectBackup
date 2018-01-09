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

	//�������� ����
	DWORD	dwNumFaces = pMesh->GetNumFaces();
	pMeshContainer->pAdjacency = new DWORD[dwNumFaces * 3];
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwNumFaces * 3);

	//�Ӽ����� ����
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

	//�������� ����
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
			{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
				first=&szNormName[k]; // �Ⱦ�� ������ first.
				if(_tcsnccmp(first,changeStr,_tcsclen(changeStr))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
					//�������� �ٲ�����ϴ�.
					last = &szNormName[k+_tcsclen(changeStr)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
					_tcsnccpy_s(szFinalNormName, changedStr, _tcsclen(changedStr)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
					_tcsnccat_s(szFinalNormName, last, _tcsclen(last)); // �����ĵ� �� �ֽ��ϴ�.
					//_tcsnccpy(first, szFinalNormName, _tcsclen(szFinalNormName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
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
				{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
					first2=&szNormName2[k]; // �Ⱦ�� ������ first.
					if(_tcsnccmp(first2,changeStr2,_tcsclen(changeStr2))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
						//�������� �ٲ�����ϴ�.
						last2 = &szNormName2[k+_tcsclen(changeStr2)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
						_tcsnccpy_s(szFinalNormName2,changedStr2,_tcsclen(changedStr2)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
						_tcsnccat_s(szFinalNormName2,last2,_tcsclen(last2)); // �����ĵ� �� �ֽ��ϴ�.
						//_tcsnccpy(first2,szFinalNormName2,_tcsclen(szFinalNormName2)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
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

				memset(szFinalNormName2,'\0',sizeof(szFinalNormName2)); // �ӽù迭 �ʱ�ȭ
				memset(szNormName2,'\0',sizeof(szNormName2));
			}

			memset(szFinalNormName,'\0',sizeof(szFinalNormName)); // �ӽù迭 �ʱ�ȭ
			memset(szNormName,'\0',sizeof(szNormName));


			_tcscpy_s(szSpecName, MAX_PATH, szFileName);

			TCHAR szFinalSpecName[256] = L"";
			TCHAR changeStr3[256] = L"_D_";
			TCHAR changedStr3[256] =L"_SP_";
			TCHAR* first3;
			TCHAR* last3;

			for(_uint k=0;k<_tcsclen(szSpecName);++k)
			{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
				first3=&szSpecName[k]; // �Ⱦ�� ������ first3.
				if(_tcsnccmp(first3,changeStr3,_tcsclen(changeStr3))==0){ //first3�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
					//�������� �ٲ�����ϴ�.
					last3 = &szSpecName[k+_tcsclen(changeStr3)]; // last3�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
					_tcsnccpy_s(szFinalSpecName,changedStr3,_tcsclen(changedStr3)); // last3 ���ķ� �ٲ� ���ڿ��� �ְ�,
					_tcsnccat_s(szFinalSpecName,last3,_tcsclen(last3)); // �����ĵ� �� �ֽ��ϴ�.
					//_tcsnccpy(first3,szFinalSpecName,_tcsclen(szFinalSpecName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalSpecName�� ������ �ٲ�ϴ�.
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
				{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
					first4=&szSpecName2[k]; // �Ⱦ�� ������ first3.
					if(_tcsnccmp(first4,changeStr4,_tcsclen(changeStr4))==0){ //first3�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
						//�������� �ٲ�����ϴ�.
						last4 = &szSpecName2[k+_tcsclen(changeStr4)]; // last3�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
						_tcsnccpy_s(szFinalSpecName2,changedStr4,_tcsclen(changedStr4)); // last3 ���ķ� �ٲ� ���ڿ��� �ְ�,
						_tcsnccat_s(szFinalSpecName2,last4,_tcsclen(last4)); // �����ĵ� �� �ֽ��ϴ�.
						//_tcsnccpy(first4,szFinalSpecName2,_tcsclen(szFinalSpecName2)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalSpecName�� ������ �ٲ�ϴ�.
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

				memset(szFinalSpecName2,'\0',sizeof(szFinalSpecName2)); // �ӽù迭 �ʱ�ȭ
				memset(szSpecName2,'\0',sizeof(szSpecName2));
			}

			memset(szFinalSpecName,'\0',sizeof(szFinalSpecName)); // �ӽù迭 �ʱ�ȭ
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
	
	//��Ű������ ����
	if(pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		DWORD matVertInfluences = 0;
		DWORD numBoneComboEntries = 0;

		pSkinInfo->ConvertToIndexedBlendedMesh(pMeshData->pMesh, 
												D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
												100,  // ���...;;
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

