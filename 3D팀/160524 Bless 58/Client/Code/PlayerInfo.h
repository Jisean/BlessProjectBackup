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

#ifndef PlayerInfo_h__
#define PlayerInfo_h__

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

class CPlayerInfo : public CUI
{
private:
	explicit CPlayerInfo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayerInfo(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
private:
	virtual HRESULT AddComponent(void);
	virtual HRESULT SetComponent(WORD wContainerID, const TCHAR* pResourcekey);
public:
	static CPlayerInfo* Create(LPDIRECT3DDEVICE9 pDevice);
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
	_float			m_fSaveX[8];
	_float			m_fSaveY[8];
	D3DXMATRIX		m_matView, m_matProj;
	bool			m_bActivate , m_bPush;
	bool			m_bPressed;
	RECT			m_Rect;

private:
	void			SetUp();
	void			CollisionUI();
	void			LoadData(const wstring& wstrPath);
};

#endif // PlayerInfo_h__