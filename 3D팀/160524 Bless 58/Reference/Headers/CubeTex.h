/*!
 * \file CubeTex.h
 * \date 2015/12/21 15:43
 *
 * \author Jusin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex
	: public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCubeTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	HRESULT CreateBuffer(void);

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};

END

#endif // CubeTex_h__