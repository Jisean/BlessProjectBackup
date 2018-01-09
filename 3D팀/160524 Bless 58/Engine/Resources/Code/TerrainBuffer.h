/*!
 * \file TerrainBuffer.h
 * \date 2016/03/12 19:54
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

#ifndef TerrainBuffer_h__
#define TerrainBuffer_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainBuffer 
	: public CVIBuffer
{
private:
	explicit CTerrainBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrainBuffer(const CTerrainBuffer& rhs);
public:
	virtual ~CTerrainBuffer(void);

private:
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);


public:
	static Engine::CTerrainBuffer* Create(LPDIRECT3DDEVICE9 pDevice, const WORD& wCntX, 
		const WORD& wCntZ, const WORD& vItv);
	virtual Engine::CResources* CloneResource(void);
	_ulong Get_VtxCntX(void)  const { return m_dwCntX;}
	_ulong Get_VtxCntZ(void)  const { return m_dwCntZ;}
	virtual void Free(void);

private:
	_ulong					m_dwCntX, m_dwCntZ;
};

END

#endif // TerrainBuffer_h__