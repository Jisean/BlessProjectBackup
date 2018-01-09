/*!
 * \file Light.h
 * \date 2016/02/22 14:23
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

#ifndef Light_h__
#define Light_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLight(void);
public:
	const D3DLIGHT9* Get_LightInfo(void) const {return &m_LightInfo;}
	D3DLIGHT9* Set_LightInfo(void);
	
	bool*		Get_LightRender(void);
public:
	HRESULT Init_Light(const D3DLIGHT9* pLightInfo);
	void Render_Light(LPD3DXEFFECT pEffect);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	D3DLIGHT9				m_LightInfo;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

private:
	bool					m_bRender;

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DLIGHT9* pLightInfo);
	virtual void	Free(void);

};

END

#endif // Light_h__