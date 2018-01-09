/*!
 * \file Renderer.h
 * \date 2015/04/02 23:05
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 씬의 렌더링을 수행하는 클래스
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CScene;
class CGameObject;

class ENGINE_DLL CRenderer : public CBase
{
public:
	enum RENDERGROUP {TYPE_PRIORITY, TYPE_NONEALPHA, TYPE_ALPHA, TYPE_UI, TYPE_END};
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CRenderer(void);

public:
	void SetCurrentScene(CScene* pScene);
	void AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject);

public:
	HRESULT Ready_Renderer(void);
	void Render(void);
	void Clear_RenderGroup(void);

private:
	void Render_Priority(void);
	void Render_NoneAlpha(void);
	void Render_LightAcc(void);
	void Render_Blend(void);
	void Render_Alpha(void);
	void Render_UI(void);

private:
	CScene*		m_pScene;

private:
	LPDIRECT3DDEVICE9		m_pDevice;	
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	float		m_fTime;
	TCHAR		m_szFps[128];
	DWORD		m_dwCount;

private:
	typedef list<CGameObject*>		RENDERLIST;
	RENDERLIST			m_RenderGroup[TYPE_END];
private:	

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};

END

#endif // Renderer_h__