/*!
 * \file LogoBack.h
 * \date 2015/04/05 8:25
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
#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};

class CEffect
	: public Engine::CGameObject
{
private:
	CEffect(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEffect(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void	Free(void);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CShader*		m_pShader;

private:
	D3DMATERIAL9		m_tMtrl;
	_float				m_fFrame;
};

#endif // LogoBack_h__