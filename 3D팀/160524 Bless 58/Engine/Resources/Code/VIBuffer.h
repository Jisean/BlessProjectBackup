/*!
 * \file VIBuffer.h
 * \date 2015/04/04 22:28
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
#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer
	: public CResources
{
public:
	enum BUFFERTYPE {BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_TERRAIN, BUFFER_CUBETEX, BUFFER_CUBECOL, BUFFER_TRAIL};

public:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);

public:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	DWORD		m_dwVtxSize;
	DWORD		m_dwVtxCnt;
	DWORD		m_dwVtxFVF;

	LPDIRECT3DINDEXBUFFER9		m_pIB;
	DWORD		m_dwIdxSize;
	DWORD		m_dwTriCnt;
	D3DFORMAT	m_IdxFmt;

	_vec3*					m_pVtxPos; // 정점들의 위치만 모아놓은 배열의 주소

public:
	virtual CResources* CloneResource(void) PURE;
	void GetVtxInfo(void* pVtxInfo);
	void GetIdxInfo(void* pIdxInfo, DWORD& dwTriCnt);
	inline const _vec3* Get_VtxPosArrayPointer(void) const {
		return m_pVtxPos;}
	inline const _ulong Get_VtxCnt(void) const {
		return m_dwVtxCnt;}
	inline _ulong Get_NumTriCnt(void) const {
		return m_dwTriCnt;}

public:
	void SetVtxInfo(void* pVtxInfo);
	void SetIdxInfo(void* pIdxInfo, const _uint& pTriCnt);
	void Change_BoxColor(DWORD dwColor);
	void Set_VertexPositionArray(bool bReset = false);
	LPDIRECT3DVERTEXBUFFER9*	GetVBInfo();

public:
	virtual HRESULT CreateBuffer(void);

public:
	void Render(const D3DXMATRIX* pmatWorld);

public:
	virtual void Free(void);
};

END

#endif // VIBuffer_h__