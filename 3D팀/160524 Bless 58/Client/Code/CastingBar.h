/*!
 * \file CastingBar.h
 * \date 2016/06/24 20:31
 *
 * \author gpdms
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CastingBar_h__
#define CastingBar_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CCastingBar
	: public CUI
{
private:
	enum	eCasting {CAST_OUTLINE, CAST_BAR, CAST_Slider, CAST_END };
private:
	explicit CCastingBar(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCastingBar(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static CCastingBar* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void	Set_TextureInit(eCasting eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);

protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;

private:
	Engine::CTexture*			m_pTexture[CAST_END];

private:
	float			m_fX[CAST_END], m_fY[CAST_END];
	float			m_fSizeX[CAST_END], m_fSizeY[CAST_END];
	D3DXMATRIX		m_matView[CAST_END], m_matProj;

private:
	float			m_fOrifX;
	float			m_fOriSizeX;

private:
	bool			m_bRender;
	float			m_fAlpha;
	float			m_fMaxTime;
	float			m_fCurTime;

public:
	void			Set_Render(bool bRender);
	void			Set_CastTime(float fMaxTime);


	// Å¬¶ó Åø
private:
	void	Init_MoveTex(eCasting eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eCasting	m_eTexNum;
};


#endif // CastingBar_h__