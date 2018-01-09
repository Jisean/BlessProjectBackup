/*!
 * \file Font.h
 * \date 2016/02/23 13:34
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

#ifndef Font_h__
#define Font_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont : public CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CFont(void);
public:
	HRESULT InitFont(const UINT& iHeight, const UINT& iWidth, const UINT& iWeight
		, const TCHAR* pFaceName);
	void Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos);
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXFONT				m_pFont;
	UINT					m_iHeight;
	UINT					m_iWidth;
public:
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice, const UINT& iHeight, const UINT& iWidth, const UINT& iWeight, const TCHAR* pFaceName);
	virtual void Free(void);
};

END

#endif // Font_h__