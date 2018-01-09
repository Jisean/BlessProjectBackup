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
class CShader_Manager;
class CRenderTarget_Manager;

class ENGINE_DLL CRenderer : public CBase
{
public:
	enum RENDERGROUP {TYPE_PRIORITY, TYPE_NONEALPHA, TYPE_STATIC, TYPE_ALPHA, TYPE_UI, TYPE_VOLUMETRIC, TYPE_END};
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CRenderer(void);

public:
	void SetCurrentScene(CScene* pScene);
	void Set_ShadowDraw(bool bStart) { m_bStart = bStart;}
	void AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject);


public:
	HRESULT Ready_Renderer(void);
	void Render(void);
	void Clear_RenderGroup(void);
	void OnOffDebugBuffer(_bool bOnOff){ m_bOnOffDebugBuffer = bOnOff; }

private: 
	HRESULT Load_Shader(void);
	HRESULT Create_Buffer(void);
	HRESULT Create_RenderTarget(void);
	HRESULT Create_DebugBuffer(void);
	HRESULT Create_MRT(void);

private:
	void Render_Priority(void);
	void Render_Shadow_Static(void); // 기림자
	void Render_Shadow_Dynamic(void);
	void Render_NoneAlpha(void);
	void Render_Static(void);
	void Render_LightAcc(void);
	void Render_DeferredBlend(void);
	void Render_Bloom(void);
	void Render_DOF(void);
	void Render_Radial(void);
	void Render_Volumetric(void);
	void Render_Blend(void);
	void Render_Alpha(void);
	void Render_UI(void);
	void Render_DebugBuffer(void);

private:
	void DrawBuffer(LPD3DXEFFECT pEffect, _uint iPassNum = 0);

private:
	float	RadialPower(void);

public:
	float	m_fLightPower;		// HDR 파워 조절용
	float	m_fBloomPower;		// Bloom 파워 조절용
	float	m_fBloomRange;		// Bloom 범위
	bool	m_bDOF;
	bool	m_bRadial;
	bool	m_bShadow;
	bool	m_bVolumetric;

	float	m_fRadialPower;

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
	typedef vector<CGameObject*>		RENDERVEC;
	RENDERVEC			m_RenderGroup[TYPE_END];
	CRenderTarget_Manager*	m_pRenderTagetMgr;
	CShader_Manager*		m_pShaderMgr;
private:	
	bool	m_bHDR;
	bool	m_bBloom;
	bool	m_bOnOffDebugBuffer;

// 그림자 한번 그리는 용도
private:
	bool	m_bStart;
	int		m_iStartCnt;

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);



};

END

#endif // Renderer_h__