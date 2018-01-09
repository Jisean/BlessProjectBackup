#include "StaticMesh.h"
#include "CubeColor.h"
#include "ResourceMgr.h"
#include "Texture.h"

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_ppTexture(NULL)
, m_pMtrl(NULL)
, m_pSubset(NULL)
, m_dwSubsetCnt(0)
{
	m_pMesh = NULL;
}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
: CMesh(rhs.m_pDevice)
, m_ppTexture(rhs.m_ppTexture)
, m_pMtrl(rhs.m_pMtrl)
, m_pSubset(rhs.m_pSubset)
, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
{
	m_pBoundingBox = rhs.m_pBoundingBox;
	m_vMin = rhs.m_vMin;
	m_vMax = rhs.m_vMax;	
	m_pMesh = rhs.m_pMesh;

	++m_dwRefCnt;
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

Engine::CResources* Engine::CStaticMesh::CloneResource(void)
{
	return new CStaticMesh(*this);
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT		hr = NULL;

	TCHAR		szFullPath[256] = L"";
	
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	hr = D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice
		, NULL, &m_pSubset, NULL, &m_dwSubsetCnt, &m_pMesh);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, szFullPath);
	
	D3DXMATERIAL*	pSubset = ((D3DXMATERIAL*)m_pSubset->GetBufferPointer());

	m_pMtrl		= new D3DMATERIAL9[m_dwSubsetCnt];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pMtrl[i] = pSubset[i].MatD3D;

		/*TCHAR		szFileName[256] = L"";
		lstrcpy(szFullPath, pPath);
		MultiByteToWideChar(CP_ACP
			, 0, pSubset[i].pTextureFilename
			, strlen(pSubset[i].pTextureFilename)
			, szFileName, 256);
		lstrcat(szFullPath, szFileName);*/

		TCHAR	szFileName[256] = L"";
		MultiByteToWideChar(CP_ACP
			, 0, pSubset[i].pTextureFilename
			, strlen(pSubset[i].pTextureFilename)
			, szFileName, 256);

		CTexture* pMeshTexture = (CTexture*)(CResourceMgr::GetInstance()->FindResource(3, szFileName));

		m_ppTexture[i] = *pMeshTexture->GetTexture(0);

		//hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i]);
		FAILED_CHECK(hr);
	}	

	MakeBoundingBox();
	return S_OK;
}

void Engine::CStaticMesh::RenderMesh_ForSDK(const D3DXMATRIX* pWorld)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pWorld);

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pDevice->SetTexture(0, m_ppTexture[i]);
		m_pDevice->SetMaterial(&m_pMtrl[i]);
		m_pMesh->DrawSubset(i);
	}

	//BoundingBox
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoundingBox->Render(pWorld);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Engine::CStaticMesh::RenderMesh_ForShader(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorld)
{
	m_pEffect = pEffect;
	m_pEffect->AddRef();

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);			
		m_pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);
	}

	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBoundingBox->Render(pWorld);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	

	Safe_Release(m_pEffect);
}


void Engine::CStaticMesh::Free(void)
{
	Engine::CResources::Free();

	if(m_dwRefCnt == 0)
	{
		Safe_Delete_Array(m_pMtrl);
		/*for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
		{
			Safe_Release(m_ppTexture[i]);
		}*/
		Safe_Delete_Array(m_ppTexture);

		Safe_Release(m_pMesh);
		Safe_Release(m_pSubset);

		m_pBoundingBox->Release();		
	}
	else
		--m_dwRefCnt;
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName)
{
	CStaticMesh*		pMesh = new CStaticMesh(pDevice);
	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
		Safe_Release(pMesh);

	return pMesh;
}

void Engine::CStaticMesh::MakeBoundingBox(void)
{
	DWORD		dwFVF = m_pMesh->GetFVF();

	if(dwFVF & D3DFVF_XYZ)
	{
		D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];
		m_pMesh->GetDeclaration(VtxFVF);

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

		m_pMesh->LockVertexBuffer(0, &pVtxInfo);

		BYTE*		pByteVtxInfo = (BYTE*)pVtxInfo;
		pVtxInfo = &pByteVtxInfo[dwOffSet];

		DWORD		dwNumVertices = m_pMesh->GetNumVertices();
		DWORD		dwStride = D3DXGetFVFVertexSize(dwFVF);

		D3DXComputeBoundingBox((D3DXVECTOR3*)pVtxInfo, dwNumVertices, dwStride, &m_vMin, &m_vMax);

		m_pMesh->UnlockVertexBuffer();
	}

	m_pBoundingBox = CCubeColor::Create(m_pDevice, m_vMin, m_vMax, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
}

