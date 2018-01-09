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

#ifndef HighIcon_h__
#define HighIcon_h__

#include "GameObject.h"
#include "Include.h"
#include "UI.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CHighIcon : public CUI
{
private:
	explicit CHighIcon(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHighIcon(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
private:
	virtual HRESULT AddComponent(void);
	virtual HRESULT SetComponent(WORD wContainerID, const TCHAR* pResourcekey);
public:
	static CHighIcon* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
private:
	Engine::CTransform*			m_pInfo;
	Engine::CTexture*			m_pTexture;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;
	D3DMATERIAL9				m_tMtrl;
private:
	_float			m_fX, m_fY;
	_float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
	bool			m_bActivate;
	RECT			m_Rect;

private:
	void			SetUp();
	void			CollisionUI();
	void			LoadData(const wstring& wstrPath);
};

#endif // HighIcon_h__