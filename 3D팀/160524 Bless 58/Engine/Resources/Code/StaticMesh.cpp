#include "StaticMesh.h"
#include "CubeColor.h"
#include "Texture.h"
#include "ResourceMgr.h"
#include <tchar.h>

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_ppTexture(NULL)
, m_pMtrl(NULL)
, m_pMesh(NULL)
, m_pSubset(NULL)
, m_dwSubsetCnt(0)
, m_ppNormTexture(NULL)
, m_ppSpecTexture(NULL)
, m_bBoxRender(false)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
: CMesh(rhs.m_pDevice)
, m_ppTexture(rhs.m_ppTexture)
, m_pMesh(rhs.m_pMesh)
, m_pMtrl(rhs.m_pMtrl)
, m_pSubset(rhs.m_pSubset)
, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
, m_ppNormTexture(rhs.m_ppNormTexture)
, m_ppSpecTexture(rhs.m_ppSpecTexture)
, m_bBoxRender(false)
{
	m_pBoundingBox = rhs.m_pBoundingBox;
	m_vMin = rhs.m_vMin;
	m_vMax = rhs.m_vMax;	

	++m_dwRefCnt;
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

Engine::CResources* Engine::CStaticMesh::CloneResource(void)
{
	return new CStaticMesh(*this);
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt)
{
	HRESULT		hr = NULL;
	m_bLoadByTxt = bLoadByTxt;

	TCHAR		szFullPath[128] = L"";
	
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	hr = D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice
		, NULL, &m_pSubset, NULL, &m_dwSubsetCnt, &m_pMesh);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, szFullPath);
	
	D3DXMATERIAL*	pSubset = ((D3DXMATERIAL*)m_pSubset->GetBufferPointer());

	m_pMtrl		= new D3DMATERIAL9[m_dwSubsetCnt];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppNormTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppSpecTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	//ZeroMemory(m_ppNormTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	//ZeroMemory(m_ppSpecTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pMtrl[i] = pSubset[i].MatD3D;

		if(m_bLoadByTxt == false)
		{
			TCHAR		szFileName[128] = L"";
			TCHAR		szNormName[128] = L"";
			TCHAR		szNormPath[128] = L"";
			lstrcpy(szFullPath, pPath);
			MultiByteToWideChar(CP_ACP
				, 0, pSubset[i].pTextureFilename
				, strlen(pSubset[i].pTextureFilename)
				, szFileName, 128);
			lstrcat(szFullPath, szFileName);

			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i]);
			
			_tcscpy_s(szNormName, 128, szFileName);

			TCHAR szFinalNormName[256] = L"";
			TCHAR changeStr[256] = L"_D_";
			TCHAR changedStr[256] =L"_N_";
			TCHAR* first;
			TCHAR* last;

			for(_uint k=0; k < _tcsclen(szNormName); ++k)
			{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
				first=&szNormName[k]; // �Ⱦ�� ������ first.
				if(_tcsnccmp(first,changeStr,_tcsclen(changeStr))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
					//�������� �ٲ�����ϴ�.
					last = &szNormName[k + _tcsclen(changeStr)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
					_tcsnccpy(szFinalNormName, changedStr, _tcsclen(changedStr)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
					_tcsnccat(szFinalNormName, last, _tcsclen(last)); // �����ĵ� �� �ֽ��ϴ�.
					_tcsnccpy(first,szFinalNormName, _tcsclen(szFinalNormName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
				}
			}

			lstrcpy(szNormPath, pPath);
			lstrcat(szNormPath, szNormName);

			if(_tcsclen(szFinalNormName) !=0)
			{
				hr = D3DXCreateTextureFromFile(m_pDevice, szNormPath, &m_ppNormTexture[i]);
				if(hr<0)
				{
					hr = D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Mesh/NullTexture/NullTexture.tga", &m_ppNormTexture[i]);
				}
			}
			else
				hr = D3DXCreateTextureFromFile(m_pDevice, L"../bin/Resources/Mesh/NullTexture/NullTexture.tga", &m_ppNormTexture[i]);


			m_ppSpecTexture[i] = NULL;

			memset(szFinalNormName,'\0',sizeof(szFinalNormName)); // �ӽù迭 �ʱ�ȭ
			memset(szNormName,'\0',sizeof(szNormName));
			//����ŧ�� ������ �˾Ƽ�����.
		}

		else if(m_bLoadByTxt == true)
		{
			TCHAR	szFileName[256] = L"";
			MultiByteToWideChar(CP_ACP
				, 0, pSubset[i].pTextureFilename
				, strlen(pSubset[i].pTextureFilename)
				, szFileName, 256);

			CTexture* pMeshTexture = (CTexture*)(CResourceMgr::GetInstance()->FindResource(2, szFileName));

			m_ppTexture[i] = *pMeshTexture->GetTexture(0);
			//������� ��ǻ�� �ؽ��� �ֱ�

			TCHAR szFinalNormName[256] = L"";
			TCHAR szNormName[256] = L"";
			_tcscpy(szNormName,szFileName);


			if(_tcsclen(szNormName) < 20) // 51�� ���� �Ž����
			{
				TCHAR changeStr[256] = L"_N_.tga";
				TCHAR changedStr[256] =L".tga";
				TCHAR* first;
				TCHAR* last;

				for(_uint k = 0; k < _tcsclen(szNormName); ++k)
				{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
					first=&szNormName[k]; // �Ⱦ�� ������ first.
					if(_tcsnccmp(first,changeStr ,_tcsclen(changeStr))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
						//�������� �ٲ�����ϴ�.
						last = &szNormName[k + _tcsclen(changeStr)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
						_tcsnccpy(szFinalNormName, changedStr, _tcsclen(changedStr)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
						_tcsnccat(szFinalNormName, last, _tcsclen(last)); // �����ĵ� �� �ֽ��ϴ�.
						_tcsnccpy(first,szFinalNormName, _tcsclen(szFinalNormName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
					}
				}


			}
			else//�׶� �Ž�
			{
				TCHAR changeStr[256] = L"_Diff";
				TCHAR changedStr[256] =L"_Norm";
				TCHAR* first;
				TCHAR* last;

				for(_uint k = 0; k < _tcsclen(szNormName); ++k)
				{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
					first = &szNormName[k]; // �Ⱦ�� ������ first.
					if(_tcsnccmp(first,changeStr, _tcsclen(changeStr))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
						//�������� �ٲ�����ϴ�.
						last = &szNormName[k + _tcsclen(changeStr)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
						_tcsnccpy(szFinalNormName, changedStr, _tcsclen(changedStr)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
						_tcsnccat(szFinalNormName, last, _tcsclen(last)); // �����ĵ� �� �ֽ��ϴ�.
						_tcsnccpy(first,szFinalNormName, _tcsclen(szFinalNormName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
					}
				}

			}


			if(_tcsclen(szFinalNormName) == 0 )//���� ������ �Ž�.
													 //���� if�� ����ϰ� else���� ���� szFinalNormName�� ������� ������� ���� ������ ���� �Ž��̴�.
			{
				TCHAR changeStr[256] = L"_D_";
				TCHAR changedStr[256] =L"_N_";
				TCHAR* first;
				TCHAR* last;

				for(_uint k = 0; k < _tcsclen(szNormName); ++k)
				{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
					first = &szNormName[k]; // �Ⱦ�� ������ first.
					if(_tcsnccmp(first, changeStr, _tcsclen(changeStr))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
						//�������� �ٲ�����ϴ�.
						last = &szNormName[k + _tcsclen(changeStr)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
						_tcsnccpy(szFinalNormName, changedStr, _tcsclen(changedStr)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
						_tcsnccat(szFinalNormName, last, _tcsclen(last)); // �����ĵ� �� �ֽ��ϴ�.
						_tcsnccpy(first, szFinalNormName, _tcsclen(szFinalNormName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
					}
				}

			}
			CTexture* pMeshNormTexture = (CTexture*)(CResourceMgr::GetInstance()->FindResource(2, szNormName));
			if(pMeshNormTexture != NULL)
			{
				m_ppNormTexture[i] = *pMeshNormTexture->GetTexture(0);
			}
			else
				m_ppNormTexture[i] = NULL;
			memset(szFinalNormName, '\0', sizeof(szFinalNormName)); // �ӽù迭 �ʱ�ȭ
			memset(szNormName, '\0', sizeof(szNormName));


			//������� �븻 �ؽ��� �ֱ�


			TCHAR szFinalSpecName[256] = L"";
			TCHAR szSpecName[256] = L"";
			CTexture* pMeshSpecTexture = NULL;
			_tcscpy(szSpecName, szFileName);


			if(_tcsclen(szSpecName) > 20)//�׶� �Ž��� ���� �����Ž� �ƴϸ� ����ŧ�� �����ϱ� Ż����
			{
				TCHAR changeStr[256] = L"_Diff";
				TCHAR changedStr[256] = L"_Spec";
				TCHAR* first;
				TCHAR* last;

				for(_uint k = 0; k < _tcsclen(szSpecName); ++k)
				{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
					first = &szSpecName[k]; // �Ⱦ�� ������ first.
					if(_tcsnccmp(first, changeStr, _tcsclen(changeStr))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
						//�������� �ٲ�����ϴ�.
						last = &szSpecName[k + _tcsclen(changeStr)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
						_tcsnccpy(szFinalSpecName, changedStr, _tcsclen(changedStr)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
						_tcsnccat(szFinalSpecName, last, _tcsclen(last)); // �����ĵ� �� �ֽ��ϴ�.
						_tcsnccpy(first, szFinalSpecName, _tcsclen(szFinalSpecName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
					}
				}

			}


			if(_tcsclen(szFinalSpecName) == 0 )//���� ������ �Ž�.
				//���� if�� ����ϰ� else���� ���� szFinalNormName�� ������� ������� ���� ������ ���� �Ž��̴�.
			{
				TCHAR changeStr[256] = L"_D_";
				TCHAR changedStr[256] =L"_SP_";
				TCHAR* first;
				TCHAR* last;

				for(_uint k = 0; k < _tcsclen(szSpecName); ++k)
				{ //szFileName �̶� �迭�� ��� �Ⱦ�ϴ�.
					first = &szSpecName[k]; // �Ⱦ�� ������ first.
					if(_tcsnccmp(first, changeStr, _tcsclen(changeStr))==0){ //first�������� ã�� �ܾ��� ũ����� �ܾ ���ٸ�
						//�������� �ٲ�����ϴ�.
						last = &szSpecName[k + _tcsclen(changeStr)]; // last�� �ٲ���� �ܾ� ������ ������ ����ϴ�.
						_tcsnccpy(szFinalSpecName, changedStr, _tcsclen(changedStr)); // last ���ķ� �ٲ� ���ڿ��� �ְ�,
						_tcsnccat(szFinalSpecName, last, _tcsclen(last)); // �����ĵ� �� �ֽ��ϴ�.
						_tcsnccpy(first, szFinalSpecName, _tcsclen(szFinalSpecName)); // �ٲ� ���ڿ��� ù ��ġ���� szFinalNormName�� ������ �ٲ�ϴ�.
					}
				}

			}
			if(_tcsclen(szFinalSpecName)  != 0)
			{
				pMeshSpecTexture = (CTexture*)(CResourceMgr::GetInstance()->FindResource(2, szSpecName));
				if(pMeshSpecTexture != NULL)
					m_ppSpecTexture[i] = *pMeshSpecTexture->GetTexture(0);
				else
					m_ppSpecTexture[i] = NULL;
			}
			else
				m_ppSpecTexture[i] = NULL;

			memset(szFinalSpecName, '\0', sizeof(szFinalSpecName)); // �ӽù迭 �ʱ�ȭ
			memset(szSpecName, '\0', sizeof(szSpecName));


		}
		
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

void Engine::CStaticMesh::RenderMesh_ForShader(LPD3DXEFFECT pEffect, const _matrix* pBoudingBoxMatrix)
{
	pEffect->AddRef();

	// �Ϲ� ������
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{	
		if(NULL != m_ppTexture[i])
		{
			pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);
			pEffect->SetTexture("g_NormalTexture", m_ppNormTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", m_ppSpecTexture[i]);
		}

		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);

		

	}	

	// ���̴� �ʱ�ȭ	- �����߰�
	pEffect->SetTexture("g_BaseTexture", NULL);
	pEffect->SetFloat("g_fSpecMapping", 0.f);
	pEffect->SetFloat("g_fWeaponCheck", 0.f);
	pEffect->SetFloat("g_fSpecRange", 5.f);
	pEffect->SetFloat("g_fSpecPow", 1.f);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	if(m_bBoxRender)
	{
#ifdef _DEBUG
		//BoundingBox
		m_pDevice->SetTexture(0, NULL);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pBoundingBox->Render(pBoudingBoxMatrix);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif
	}
}

void Engine::CStaticMesh::RenderMesh_ForShader(CTexture* pTexture, CTexture* pMaskTexture, CTexture* pNormalTexture, LPD3DXEFFECT pEffect, const _matrix* pBoudingBoxMatrix, _uint cPasses /*= 0*/, _uint iPassIndex /*= 0*/)
{
	pEffect->AddRef();

	if(cPasses)
	{	// �ν��Ͻ�
		pEffect->Begin(&cPasses, 0);
		pEffect->BeginPass(iPassIndex);

		for( iPassIndex = 0; iPassIndex < cPasses; iPassIndex++ )
		{
			pEffect->CommitChanges();
		}
	}
	else
	{	
		if(NULL != pTexture)
			pTexture->Set_ConstantTable(pEffect, "g_BaseTexture", 0);

		if(NULL != pNormalTexture)
			pNormalTexture->Set_ConstantTable(pEffect, "g_NormalTexture", 0);

		if(NULL != pMaskTexture)
			pMaskTexture->Set_ConstantTable(pEffect, "g_MaskTexture", 0);



		//	pEffect->SetTexture("g_BaseTexture", ppTexture[(int)m_fFrame]);

		// �Ϲ� ������
		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(iPassIndex);

		for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		{
			pEffect->CommitChanges();

			m_pMesh->DrawSubset(i);
			//??
			//pEffect->SetTexture("g_BaseTexture", NULL);
		}
	}


	pEffect->EndPass();
	pEffect->End();
	Engine::Safe_Release(pEffect);
}

void Engine::CStaticMesh::Free(void)
{
	Engine::CResources::Free();

	if(m_dwRefCnt == 0)
	{
		Safe_Delete_Array(m_pMtrl);

		if(m_bLoadByTxt == false)
		{
			if(m_bEffect == false)
			{
				for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
				{
					Safe_Release(m_ppTexture[i]);
				}
			}
		}
		Safe_Delete_Array(m_ppTexture);
		Safe_Delete_Array(m_ppNormTexture);
		Safe_Delete_Array(m_ppSpecTexture);

		Safe_Release(m_pMesh);
		Safe_Release(m_pSubset);

		m_pBoundingBox->Release();		
	}
	else
		--m_dwRefCnt;
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName, bool bLoadByTxt)
{
	CStaticMesh*		pMesh = new CStaticMesh(pDevice);
	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName, bLoadByTxt)))
		Safe_Release(pMesh);

	return pMesh;
}

void Engine::CStaticMesh::MakeBoundingBox(void)
{
	LPD3DXMESH* ppTempMesh = new LPD3DXMESH;
	m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(), Engine::VTXFVF_TEX, m_pDevice, ppTempMesh);
	DWORD		dwFVF = (*ppTempMesh)->GetFVF();

	//DWORD dwFVF = m_pMesh->GetFVF();

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
	Engine::Safe_Delete(ppTempMesh);
	m_pBoundingBox = CCubeColor::Create(m_pDevice, m_vMin, m_vMax, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
}

HRESULT Engine::CStaticMesh::LoadEffectMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT		hr = NULL;

	TCHAR		szFullPath[128] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	hr = D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice
		, NULL, &m_pSubset, NULL, &m_dwSubsetCnt, &m_pMesh);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, szFullPath);

	D3DXMATERIAL*	pSubset = ((D3DXMATERIAL*)m_pSubset->GetBufferPointer());

	m_pMtrl		= new D3DMATERIAL9[m_dwSubsetCnt];

	MakeBoundingBox();

	m_bEffect = true;

	return S_OK;
}

Engine::CStaticMesh* Engine::CStaticMesh::CreateEffect(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName)
{
	CStaticMesh*		pMesh = new CStaticMesh(pDevice);
	if(FAILED(pMesh->LoadEffectMeshFromFile(pPath, pFileName)))
		Safe_Release(pMesh);

	return pMesh;
}

