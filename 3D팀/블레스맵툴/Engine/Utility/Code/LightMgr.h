/*!
 * \file LightMgr.h
 * \date 2016/02/22 14:09
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

#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CLight;

class ENGINE_DLL CLightMgr : public CBase
{
public:
	DECLARE_SINGLETON(CLightMgr)

private:
	CLightMgr(void);
	~CLightMgr(void);
public:
	const D3DLIGHT9* Get_LightInfo(const _ulong& dwIndex = 0) const;

public:
	HRESULT AddLight(LPDIRECT3DDEVICE9 pDevice
		, const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx);
	void Render_Light(LPD3DXEFFECT pEffect);

private:
	typedef list<CLight*>		LIGHTLIST;
	LIGHTLIST		m_LightList;

private:
	virtual void Free(void);
};

END

#endif // LightMgr_h__