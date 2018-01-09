/*!
 * \file UI.h
 * \date 2016/03/15 13:45
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

#ifndef UI_h__
#define UI_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
};

class CUI
	: public Engine::CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	Engine::CTransform*			m_pInfo;
	Engine::CTexture*			m_pTexture;
	Engine::CVIBuffer*			m_pBuffer;

private:
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
};

#endif // UI_h__