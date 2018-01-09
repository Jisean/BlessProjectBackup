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

#ifndef Aim_h__
#define Aim_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CTexture;
	class CVIBuffer;
};

class CAim
	: public CUI
{
private:
	explicit CAim(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CAim(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static CAim* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CTexture*			m_pTexture;
	Engine::CVIBuffer*			m_pBuffer;
protected:
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
	bool			m_bAct_Ranger;
//////////////////////////////////////////////////////////////////////////GET
public:
	bool			Get_ActRanger();
//////////////////////////////////////////////////////////////////////////SET
public:
	void			Set_ActRanger(bool bCheck);
};

#endif // Aim_h__