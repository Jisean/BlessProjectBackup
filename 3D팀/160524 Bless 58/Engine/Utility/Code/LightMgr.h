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
	const D3DLIGHT9* Get_LightInfo(const _ulong& dwIndex = 0, TCHAR*	pKeyName = L"Directional");
public:
	CLight*	Get_Light(TCHAR*	pKeyName, const DWORD& dwLightIdx);
	size_t	Get_LightKeyCnt(void);
	size_t	Get_DirectionLight(void);
	int		Get_LightCnt(TCHAR*	pKeyName);
	void	Set_LightRender(TCHAR*	pKeyName, bool bRender);
	void	Set_LightRender(UINT	iLightKey, bool bRender);

public:
	HRESULT AddLight(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, TCHAR*	pKeyName);
	void Render_Light(LPD3DXEFFECT pEffect);

private:
	HTable						m_htLightMap;
	typedef vector<CLight*>		LIGHTVEC;

private:
	virtual void Free(void);
};

END

#endif // LightMgr_h__