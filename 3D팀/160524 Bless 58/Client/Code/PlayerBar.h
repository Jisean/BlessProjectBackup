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

#ifndef PlayerBar_h__
#define PlayerBar_h__

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

class CPlayerBar : public CUI
{
private:
	explicit CPlayerBar(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayerBar(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
private:
	virtual HRESULT AddComponent(void);
	virtual HRESULT SetComponent(WORD wContainerID, const TCHAR* pResourcekey);
public:
	static CPlayerBar* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
private:
	Engine::CTransform*			m_pInfo;
	Engine::CTexture*			m_pTexture;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;
	D3DMATERIAL9				m_tMtrl;
private:
	TCHAR			m_pRight[128];
	TCHAR			m_pLeft[128];
	TCHAR			m_pTop[128];
	TCHAR			m_pBottom[128];
private:
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
	bool			m_bClick;
	bool			m_bPressed;
	float			m_fSaveX[5];
	float			m_fSaveY[5];
	float			m_fReferX;
	float			m_fReferY;
	RECT			m_Rect;
	float			m_fTimeDelta;
	float			m_fWaveTime;
	float			m_fTime;

private:
	float			m_fOriHpSize;
	float			m_fOriHpfX;
	float			m_fOriMpSize;
	float			m_fOriMpfX;

private:
	void			PlayerBar();
	void			SetUp();
	void			CollisionUI();
	void			LoadData(const wstring& wstrPath);
private:
	void	Move_Tex(void);
	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

};

#endif // PlayerBar_h__