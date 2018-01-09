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
	virtual ~CTerrainTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pDevice
		, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);
	virtual void Free(void);
};

END

#endif // TerrainTex_h__