#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"
#include "CubeColor.h"

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_pRootBone(NULL)
, m_pLoader(NULL)
, m_pAniCtrl(NULL)
, m_pEffect(NULL)
, m_fAniSpeed(1.f)
{
	 m_pMyMatrixTex = NULL;
}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
: CMesh(rhs.m_pDevice)
, m_pRootBone(rhs.m_pRootBone)
, m_pLoader(rhs.m_pLoader)
, m_pEffect(rhs.m_pEffect)
, m_fAniSpeed(rhs.m_fAniSpeed)
{
	m_pAniCtrl = new CAniCtrl(*(rhs.m_pAniCtrl));

	m_pBoundingBox = rhs.m_pBoundingBox;
	m_vMin = rhs.m_vMin;
	m_vMax = rhs.m_vMax;
	m_pMyMatrixTex = rhs.m_pMyMatrixTex;

	++m_dwRefCnt;
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{
	
}

Engine::CResources* Engine::CDynamicMesh::CloneResource(void)
{
	return new CDynamicMesh(*this);
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt)
{

	m_pLoader = new CHierarchyLoader(m_pDevice, pPath);

	TCHAR		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	m_pAniCtrl = new CAniCtrl;

	HRESULT		hr = D3DXLoadMeshHierarchyFromX(szFullPath
		, D3DXMESH_MANAGED
		, m_pDevice
		, m_pLoader
		, NULL
		, &m_pRootBone
		, m_pAniCtrl->GetAniCtrl());
	FAILED_CHECK(hr);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	InitFrameMatrix((DERIVED_FRAME*)m_pRootBone, &matIdentity);
	SetUpFrameMatrixPointer((DERIVED_FRAME*)m_pRootBone);

	MakeBoundingBox(m_pRootBone, m_vMin, m_vMax);
	m_pBoundingBox = CCubeColor::Create(m_pDevice, m_vMin, m_vMax, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	m_pDevice->CreateTexture(32, 32, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, &m_pMyMatrixTex, NULL);

	return S_OK;
}

void Engine::CDynamicMesh::RenderMesh_ForSDK(const D3DXMATRIX* pWorld)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pWorld);
	Find_MeshContainer((DERIVED_FRAME*)m_pRootBone);

	//BoundingBox
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoundingBox->Render(pWorld);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Engine::CDynamicMesh::RenderMesh_ForShader(LPD3DXEFFECT pEffect)
{
	m_pEffect = pEffect;
	m_pEffect->AddRef();

	Find_MeshContainer((DERIVED_FRAME*)m_pRootBone);

	Safe_Release(m_pEffect);

	// 셰이더 초기화	- 혜은추가
	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetFloat("g_fSpecMapping", 0.f);
	m_pEffect->SetFloat("g_fWeaponCheck", 0.f);
	m_pEffect->SetFloat("g_fSpecRange", 5.f);
	m_pEffect->SetFloat("g_fSpecPow", 1.f);
}

void Engine::CDynamicMesh::FrameMove(const float& fTimePerSec)
{
	m_pAniCtrl->FrameMove(fTimePerSec * m_fAniSpeed);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	UpdateFrameMatrix((DERIVED_FRAME*)m_pRootBone, &matIdentity);
}

void Engine::CDynamicMesh::Free(void)
{
	if(m_dwRefCnt == 0)
	{
		m_pLoader->DestroyFrame(m_pRootBone);
		Safe_Delete(m_pLoader);
		Safe_Delete(m_pAniCtrl);
		Safe_Release(m_pBoundingBox);
	}
	else
	{
		Safe_Delete(m_pAniCtrl);
		--m_dwRefCnt;
	}
}

void Engine::CDynamicMesh::InitFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix)
{
//	if(pFrame != NULL)
//	{
////		pFrame->pParentsMatrix = const_cast<D3DXMATRIX*>(pParentMatrix);
//		pFrame->CombinedMatrix = pFrame->TransformationMatrix * (*pParentMatrix);
////		m_vecFrame.push_back(pFrame);
//	}
//
//	if(pFrame->pFrameFirstChild != NULL)
//		InitFrameMatrix((DERIVED_FRAME*)pFrame->pFrameFirstChild, &pFrame->CombinedMatrix);
//
//	if(pFrame->pFrameSibling != NULL)
//		InitFrameMatrix((DERIVED_FRAME*)pFrame->pFrameSibling, pParentMatrix);
	if(pFrame != NULL)
	{
		pFrame->CombinedMatrix = pFrame->TransformationMatrix * (*pParentMatrix);
	}

	if(pFrame->pFrameFirstChild != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameFirstChild, &pFrame->CombinedMatrix);

	if(pFrame->pFrameSibling != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameSibling, pParentMatrix);
}

void Engine::CDynamicMesh::UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix)
{
	if(pFrame != NULL)
	{
//		pFrame->pParentsMatrix = const_cast<D3DXMATRIX*>(pParentMatrix);
		pFrame->CombinedMatrix = pFrame->TransformationMatrix * (*pParentMatrix);
	}

	if(pFrame->pFrameFirstChild != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameFirstChild, &pFrame->CombinedMatrix);

	if(pFrame->pFrameSibling != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameSibling, pParentMatrix);
}

void Engine::CDynamicMesh::SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame)
{
	if(pFrame == NULL)
		return;

	if(pFrame->pMeshContainer != NULL)
	{
		if(pFrame->pMeshContainer->pSkinInfo == NULL)
			return ;

		DWORD	dwNumFrames = pFrame->pMeshContainer->pSkinInfo->GetNumBones();

		((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameCombinedMatrix = new D3DXMATRIX*[dwNumFrames];
		ZeroMemory(((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameCombinedMatrix, sizeof(D3DXMATRIX*) * dwNumFrames);

		for(DWORD i = 0; i < dwNumFrames; ++i)
		{
			((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameCombinedMatrix[i] = 
				&((DERIVED_FRAME*)D3DXFrameFind(m_pRootBone, pFrame->pMeshContainer->pSkinInfo->GetBoneName(i)))->CombinedMatrix;
		}
	}

	if(pFrame->pFrameFirstChild != NULL)
		SetUpFrameMatrixPointer(((DERIVED_FRAME*)pFrame->pFrameFirstChild));

	if(pFrame->pFrameSibling != NULL)
		SetUpFrameMatrixPointer(((DERIVED_FRAME*)pFrame->pFrameSibling));
}

void Engine::CDynamicMesh::Find_MeshContainer(DERIVED_FRAME* pFrame)
{
	if(pFrame == NULL)
		return;

	if(pFrame->pMeshContainer != NULL)
		Render_MeshContainer((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer);

	if(pFrame->pFrameFirstChild != NULL)
		Find_MeshContainer((DERIVED_FRAME*)pFrame->pFrameFirstChild);

	if(pFrame->pFrameSibling != NULL)
		Find_MeshContainer((DERIVED_FRAME*)pFrame->pFrameSibling);
}

void Engine::CDynamicMesh::Render_MeshContainer(DERIVED_MESHCONTAINER* pMeshContainer)
{
	//if(pMeshContainer->pSkinInfo != NULL)
	//{
	//	DWORD dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();

	//	if(pMeshContainer->pResultMatrix == NULL)
	//		pMeshContainer->pResultMatrix = new D3DXMATRIX[dwNumBones];

	//	for(DWORD i = 0; i < dwNumBones; ++i)
	//	{
	//		pMeshContainer->pResultMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * *pMeshContainer->ppFrameCombinedMatrix[i];
	//	}

	//	BYTE*		pSour;
	//	BYTE*		pDest;

	//	//스키닝
	//	pMeshContainer->pOriMesh->LockVertexBuffer(0, (void**)&pSour);
	//	pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (void**)&pDest);

	//	pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pResultMatrix
	//		, NULL, pSour, pDest);

	//	pMeshContainer->pOriMesh->UnlockVertexBuffer();
	//	pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();


	//	for(DWORD i = 0; i < pMeshContainer->NumMaterials; ++i)
	//	{
	//		m_pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);			
	//		m_pEffect->CommitChanges();

	//		pMeshContainer->MeshData.pMesh->DrawSubset(i);
	//	}
	//}
	if(pMeshContainer->pSkinInfo != NULL)
	{

		// 본 컴비네이션 테이블을 가져온다.
		LPD3DXBONECOMBINATION pBoneComb =
			reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		DWORD dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();
		if(pMeshContainer->pResultMatrix == NULL)
		{
			pMeshContainer->pResultMatrix = new D3DXMATRIX[dwNumBones];
			ZeroMemory(pMeshContainer->pResultMatrix, sizeof(D3DXMATRIX) * dwNumBones);
		}

		// 모든 속성 그룹
		for(int i = 0; i < (int)pMeshContainer->NumAttributeGroups; i++)
		{		 
			int iPaletteEntry = 0;

			// 본 컴비네이션 테이블로부터 본들의 아이디를 얻고, 행렬 팔레트를 구성한다.
			for(; iPaletteEntry < 100; iPaletteEntry++)
			{
				int iMatrixIndex = pBoneComb[i].BoneId[iPaletteEntry];
				if(iMatrixIndex != UINT_MAX)
				{
					D3DXMatrixMultiply(&pMeshContainer->pResultMatrix[iPaletteEntry],
						&pMeshContainer->pFrameOffsetMatrix[iMatrixIndex],
						pMeshContainer->ppFrameCombinedMatrix[iMatrixIndex]);
				}
				else			
					break;
			}
			////------------하드웨어 스키닝 -------------//
			//// Shader_HardwareSkinning 변경 
			//// 셰이더에게 행렬 팔레트를 업로드한다.
			//m_pEffect->SetMatrixArray("g_matMatrixPalette", pMeshContainer->pResultMatrix, iPaletteEntry);
			//m_pEffect->SetInt("g_iNumBoneInfluences", pMeshContainer->iBoneInfluences);

			//int mtrlIndex = pBoneComb[i].AttribId;
			////m_pDevice->SetMaterial(&(pMeshContainer->pMaterials[mtrlIndex]));
			//m_pDevice->SetTexture(0, pMeshContainer->ppTexture[mtrlIndex]);		
			//m_pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[mtrlIndex]);

			//m_pEffect->CommitChanges();
			//pMeshContainer->MeshData.pMesh->DrawSubset(mtrlIndex);

			//------------ VTF ------------//
			// Shader_VTF 변경 
			D3DLOCKED_RECT lockRect = {0};

			m_pMyMatrixTex->LockRect(0, &lockRect, NULL, 0);
			memcpy(lockRect.pBits, pMeshContainer->pResultMatrix, sizeof(D3DXMATRIX) * iPaletteEntry);
			m_pMyMatrixTex->UnlockRect(0);

			// Shader 에 Texture 를 올린다.
			m_pEffect->SetTexture("g_MatrixTexture", m_pMyMatrixTex);
			m_pEffect->SetInt("g_iNumBoneInfluences", pMeshContainer->iBoneInfluences);

			int mtrlIndex = pBoneComb[i].AttribId;
			//m_pDevice->SetMaterial(&(pMeshContainer->pMaterials[mtrlIndex]));
			m_pDevice->SetTexture(0, pMeshContainer->ppTexture[mtrlIndex]);		
			m_pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[mtrlIndex]);
			m_pEffect->SetTexture("g_NormalTexture", pMeshContainer->ppNormTexture[mtrlIndex]);

			m_pEffect->CommitChanges();

			pMeshContainer->MeshData.pMesh->DrawSubset(mtrlIndex);
		}
	}
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt)
{
	CDynamicMesh*	pMesh = new CDynamicMesh(pDevice);
	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName, bLoadByTxt)))
		Safe_Release(pMesh);

	return pMesh;
}

void Engine::CDynamicMesh::SetAnimationSet(UINT iAniIdx)
{
	m_pAniCtrl->SetAnimationSet(iAniIdx);
}

void Engine::CDynamicMesh::SetAniSpeed(float fAniSpeed)
{
	m_fAniSpeed = fAniSpeed;
}

DOUBLE Engine::CDynamicMesh::GetTrackPosition(void)
{
	return m_pAniCtrl->GetTrackPosition();
}

DOUBLE Engine::CDynamicMesh::GetPeriod(void)
{
	return m_pAniCtrl->GetPeriod();
}

void Engine::CDynamicMesh::SetTrackPos(DOUBLE dPosition)
{
	m_pAniCtrl->SetTrackPos(dPosition);
}

void Engine::CDynamicMesh::MakeBoundingBox(D3DXFRAME* pFrame, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
{
	if(pFrame->pMeshContainer != NULL)
	{
		LPD3DXMESH		pMesh = pFrame->pMeshContainer->MeshData.pMesh;
		if(pMesh != NULL)
		{
			DWORD		dwFVF = pMesh->GetFVF();
			if(dwFVF & D3DFVF_XYZ)
			{
				D3DXVECTOR3	vTempMin = D3DXVECTOR3(0.f, 0.f, 0.f);
				D3DXVECTOR3	vTempMax = D3DXVECTOR3(0.f, 0.f, 0.f);

				D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];
				pMesh->GetDeclaration(VtxFVF);

				int		iIndex = 0;
				DWORD	dwOffSet = 0;

				while(true)
				{
					if(VtxFVF[iIndex].Usage == D3DDECLUSAGE_POSITION)
					{
						dwOffSet = VtxFVF[iIndex].Offset;
						break;
					}
					++iIndex;
				}

				void*		pVtxInfo = NULL;

				pMesh->LockVertexBuffer(0, &pVtxInfo);

				BYTE*		pByteVtxInfo = (BYTE*)pVtxInfo;
				pVtxInfo = &pByteVtxInfo[dwOffSet];

				DWORD		dwNumVertices = pMesh->GetNumVertices();
				DWORD		dwStride = D3DXGetFVFVertexSize(dwFVF);

				D3DXComputeBoundingBox((D3DXVECTOR3*)pVtxInfo, dwNumVertices, dwStride, &vTempMin, &vTempMax);

				pMesh->UnlockVertexBuffer();

				vMax.x = max(vTempMax.x, vMax.x);
				vMax.y = max(vTempMax.y, vMax.y);
				vMax.z = max(vTempMax.z, vMax.z);

				vMin.x = min(vTempMin.x, vMin.x);
				vMin.y = min(vTempMin.y, vMin.y);
				vMin.z = min(vTempMin.z, vMin.z);
			}
		}
	}

	if(pFrame->pFrameFirstChild != NULL)
		MakeBoundingBox(pFrame->pFrameFirstChild, vMin, vMax);

	if(pFrame->pFrameSibling != NULL)
		MakeBoundingBox(pFrame->pFrameSibling, vMin, vMax);
}

const D3DXMATRIX* Engine::CDynamicMesh::FindFrame(const char* pFrameName)
{
	DERIVED_FRAME*	pFrame = ((DERIVED_FRAME*)D3DXFrameFind(m_pRootBone, pFrameName));

	return &pFrame->CombinedMatrix;
}

void Engine::CDynamicMesh::SetAniInfo(Engine::ANI_INFO AniInfo)
{
	 m_pAniCtrl->SetAniInfo(AniInfo);
}

