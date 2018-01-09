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
#include "CWY_Struct.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CTexture;
};

class CUI
	: public Engine::CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI(void);

protected:
	virtual HRESULT Initialize(void) { return S_OK;}
	virtual _int Update(const _float& fTimeDelta) { return 0; }
	virtual void Render(void) {}

protected:
	virtual HRESULT AddComponent(void) {return S_OK; }
	virtual HRESULT SetComponent(void) {return S_OK; }

protected:
	static CUI* Create(LPDIRECT3DDEVICE9 pDevice) {return NULL; }
	virtual void Free(void);

protected:
	Engine::CTransform*			m_pInfo;
	Engine::CTexture*			m_pTexture;
	Engine::CVIBuffer*			m_pBuffer;
protected:
	typedef	vector<UI_INFO>		vec_UI;
	typedef vector<Engine::CTexture*> vec_Texture;
	vec_UI			m_vecUI_Info;
	vec_Texture	m_vecTexture;
protected:
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
protected:
	UINT			m_iCnt;
};

#endif // UI_h__