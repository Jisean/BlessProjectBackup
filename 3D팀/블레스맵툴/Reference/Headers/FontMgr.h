/*!
 * \file FontMgr.h
 * \date 2016/02/23 13:47
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

#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)
class CFont;
class ENGINE_DLL CFontMgr : public CBase
{
public:
	DECLARE_SINGLETON(CFontMgr)
private:
	explicit CFontMgr(void);
	virtual ~CFontMgr(void);
public:
	HRESULT AddFont(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey 
		, const UINT& iHeight, const UINT& iWidth, const UINT& iWeight);
	void Render(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString
		, D3DXCOLOR Color);

private:
	typedef map<const TCHAR*, CFont*>		MAPFONT;
	MAPFONT			m_mapFont;
public:
	virtual void Free(void);
};

END

#endif // FontMgr_h__