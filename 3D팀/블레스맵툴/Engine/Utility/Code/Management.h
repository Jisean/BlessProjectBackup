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

public:
	void AddRenderGroup(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject);
public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pDevice);
	_int  Update(const _float& fTimeDelta);
	void Render(void);
	void Clear_RenderGroup(void);
	Engine::CScene* Get_Scene(void);

public:
	template <typename T>
	HRESULT SceneChange(T& Functor);

private:
	CScene*					m_pScene;
	CRenderer*				m_pRenderer;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
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
	return S_OK;
}

END

#endif // Management_h__