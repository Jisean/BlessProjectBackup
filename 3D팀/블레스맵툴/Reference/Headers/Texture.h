/*!
 * \file Texture.h
 * \date 2015/04/06 21:48
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
#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture
	: public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture(void);

public:
	virtual CResources* CloneResource(void);
	void Set_ConstantTable(LPD3DXEFFECT pEffect, const char* pConstantName, const _ushort& wCnt = 0);

public:
	HRESULT LoadTexture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
	void Render(const DWORD& dwStage, const DWORD& iIndex);
	HRESULT LockRect(const DWORD& dwStage, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD dwFlag);
	HRESULT UnLockRect(const DWORD& dwStage);
	LPDIRECT3DTEXTURE9*	GetTexture(const DWORD& iIndex);

private:
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;
	VECTEXTURE		m_vecTexture;

private:
	DWORD		m_dwContainerSize;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice
		, TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
	virtual void Free(void);
};

END

#endif // Texture_h__