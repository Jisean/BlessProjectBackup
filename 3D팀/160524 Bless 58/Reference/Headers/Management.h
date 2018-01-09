/*!
 * \file Management.h
 * \date 2015/04/02 22:59
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
#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;
class CComponent;
class CGameObject;

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement(void);
	virtual ~CManagement(void);

public:
	const CComponent* GetComponent(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey);
	vector<CGameObject*>*	GetObjList(const WORD LayerID, const TCHAR* pObjKey);

public:
	void AddRenderGroup(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject);
public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pDevice);
	_int  Update(const _float& fTimeDelta);
	void Render(void);
	void Clear_RenderGroup(void);
	CScene*	GetScene(void){ return m_pScene; }
	void OnOffDebugBuffer(_bool bOnOff){ m_pRenderer->OnOffDebugBuffer(bOnOff); }

public:
	template <typename T>
	HRESULT SceneChange(T& Functor);

private:
	CScene*					m_pScene;
	CRenderer*				m_pRenderer;

public:
	float* SetRendererLightPower(void)			{ return &m_pRenderer->m_fLightPower; }
	float* SetRendererBloomPower(void)			{ return &m_pRenderer->m_fBloomPower; }
	float* SetRendererBloomRange(void)			{ return &m_pRenderer->m_fBloomRange; }
	void SetDOFRender(bool bDOF)				{ m_pRenderer->m_bDOF = bDOF; }
	void SetShadowRender(bool bShadow)			{ m_pRenderer->m_bShadow = bShadow; }
	void SetVolumetricRender(bool bVolumetric)	{ m_pRenderer->m_bVolumetric = bVolumetric; }
	void SetRadialRender(bool bRadial, float fMaxTime = 2.f);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	float					m_fRadialTime;
	float					m_fMaxTime;
public:
	virtual void Free(void);
};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if(m_pScene != NULL)
		Engine::Safe_Release(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pDevice), E_FAIL);

	m_pRenderer->SetCurrentScene(m_pScene);
	m_pRenderer->Set_ShadowDraw(true);
	return S_OK;
}

END

#endif // Management_h__