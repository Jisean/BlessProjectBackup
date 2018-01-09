#include "VIBuffer.h"

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
, m_pVB(NULL)
, m_dwVtxSize(0)
, m_dwVtxCnt(0)
, m_dwVtxFVF(0)
, m_pIB(NULL)
, m_dwIdxSize(0)
, m_dwTriCnt(0)
, m_IdxFmt(D3DFMT_UNKNOWN)
, m_pVtxPos(NULL)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
: CResources(rhs.m_pDevice)
, m_pVB(rhs.m_pVB)
, m_dwVtxSize(rhs.m_dwVtxSize)
, m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwVtxFVF(rhs.m_dwVtxFVF)
, m_pIB(rhs.m_pIB)
, m_dwIdxSize(rhs.m_dwIdxSize)
, m_dwTriCnt(rhs.m_dwTriCnt)
, m_IdxFmt(rhs.m_IdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
	Set_VertexPositionArray();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void Engine::CVIBuffer::GetVtxInfo(void* pVtxInfo)
{
	void* pOriVtx = NULL;

	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pVtxInfo, pOriVtx, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::GetIdxInfo(void* pIdxInfo, DWORD& dwTriCnt)
{
	void* pOriIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pOriIndex, 0);
	memcpy(pIdxInfo, pOriIndex, m_dwIdxSize * m_dwTriCnt);
	m_pIB->Unlock();

	dwTriCnt = m_dwTriCnt;
}

void Engine::CVIBuffer::SetVtxInfo(void* pVtxInfo)
{
	void* pOriVtx = NULL;
	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pOriVtx, pVtxInfo, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::SetIdxInfo(void* pIdxInfo,  const _uint& pTriCnt)
{
	void* pOriIndex = NULL;

	m_pIB->Lock(0, 0, &pOriIndex , 0);
	memcpy(pOriIndex, pIdxInfo, m_dwIdxSize * pTriCnt);
	m_dwTriCnt = pTriCnt;
	m_pIB->Unlock();
}

HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	HRESULT		hr = NULL;
	hr = m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"VertexBuffer Create Failed");

	hr = m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"IndexBuffer Create Failed");
	return S_OK;
}

void Engine::CVIBuffer::Set_VertexPositionArray(bool bReset/* = false*/)
{
	if(bReset)
		Engine::Safe_Delete_Array(m_pVtxPos);

	m_pVtxPos = new _vec3[m_dwVtxCnt];

	_byte*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_uint i = 0; i < m_dwVtxCnt; ++i)	
		memcpy(m_pVtxPos + i, pVertex + m_dwVtxSize * i, sizeof(_vec3));	

	m_pVB->Unlock();
}	

void Engine::CVIBuffer::Render(const D3DXMATRIX* pmatWorld)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pmatWorld);

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwVtxFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void Engine::CVIBuffer::Change_BoxColor(DWORD dwColor)
{
	VTXCOL*		pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	for(int i = 0; i < 8; ++i)
	{
		pVertex[i].dwColor = dwColor;
	}
	m_pVB->Unlock();
}

void Engine::CVIBuffer::Free(void)
{
	Engine::CResources::Free();

	Engine::Safe_Release(m_pVB);
	_ulong RefCnt = Engine::Safe_Release(m_pIB);

	/*if(0 == RefCnt)*/
		Engine::Safe_Delete_Array(m_pVtxPos);
}

LPDIRECT3DVERTEXBUFFER9* Engine::CVIBuffer::GetVBInfo()
{
	return &m_pVB;
}