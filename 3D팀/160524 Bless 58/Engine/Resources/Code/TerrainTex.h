/*!
 * \file TerrainTex.h
 * \date 2015/09/08 18:12
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

#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex
	: public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	virtual CResources* CloneResource(void);
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);
	_ulong Get_VtxCntX(void)  const { return m_dwCntX;}
	_ulong Get_VtxCntZ(void)  const { return m_dwCntZ;}

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pDevice
		, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);
	virtual void Free(void);

private:
	_ulong					m_dwCntX, m_dwCntZ;
};

END

#endif // TerrainTex_h__