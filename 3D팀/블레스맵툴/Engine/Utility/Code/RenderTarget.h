#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderTarget(void);
public:
	void SetUp_TargetOnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
public:
	HRESULT Ready_RenderTarget(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
	void SetUp_OnGraphicDev(const _uint& iIndex);
	void Release_OnGraphicDev(void);
	void Clear_RenderTarget(void);
public:
	HRESULT  Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void Render_DebugBuffer(void);
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPDIRECT3DTEXTURE9				m_pTargetTexture;
	LPDIRECT3DSURFACE9				m_pSurface;
	LPDIRECT3DSURFACE9				m_pOldSurface;
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
private:	
	D3DXCOLOR						m_ClearColor;
	_uint							m_iIndex;

public:
	static CRenderTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
	virtual void Free(void);
};


END

#endif // RenderTarget_h__
