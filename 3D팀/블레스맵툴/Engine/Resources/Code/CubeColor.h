/*!
 * \file CubeColor.h
 * \date 2016/02/04 11:20
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

#ifndef CubeColor_h__
#define CubeColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColor
	: public CVIBuffer
{
private:
	explicit CCubeColor(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCubeColor(void);

public:
	virtual CResources* CloneResource(void);

public:
	HRESULT CreateBuffer(const D3DXVECTOR3& vMin
		, const D3DXVECTOR3& vMax
		, const DWORD& dwColor);

public:
	static CCubeColor* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3& vMin = D3DXVECTOR3(-1.f, -1.f, -1.f)
		, const D3DXVECTOR3& vMax = D3DXVECTOR3(1.f, 1.f, 1.f)
		, const DWORD& dwColor = D3DXCOLOR(1.f, 0.f, 1.f, 1.f));


public:
	virtual void Free(void);
};

END

#endif // CubeColor_h__