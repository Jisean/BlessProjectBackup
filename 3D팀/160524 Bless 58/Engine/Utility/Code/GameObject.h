/*!
 * \file GameObject.h
 * \date 2015/04/03 18:11
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
#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)
class CComponent;
class ENGINE_DLL CGameObject : public CBase
{
public:
	enum SHADOWTYPE {SHADOW_NONE_D, SHADOW_NONE_S, SHADOW_SHADOW, SHADOW_OBJECT, SHADOW_END};
	enum TRAIL_STATE {STATE_READY, STATE_START, STATE_IDLE};

protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGameObject(void);
public:
	const CComponent* GetComponent(const TCHAR* pComponentKey);
	const float& Get_ViewZ(void) const {return m_fViewZ;}

public:
	virtual HRESULT	Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void){}
	void	Set_ShadowRender(SHADOWTYPE eShadowRender){ m_eShadowRender = eShadowRender; }

protected:
	virtual HRESULT AddComponent(void) {return S_OK;}

protected:
	void Compute_ViewZ(const D3DXVECTOR3* pPos);
public:
	void SetDead(bool bDead = true);
	bool GetDead(void);
	void SetTrailState(bool bCheck);
	int	GetTrailState();
	void	SetNumber(int iNumber);
	int		GetNumber();


protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	float		m_fViewZ;
	bool		m_bDead;
	bool		m_bStart;
	SHADOWTYPE	m_eShadowRender;
	TRAIL_STATE	m_eTrailState;
	int			m_iNumber;

protected:
	typedef HTable		HTCOMPONENT;
	HTCOMPONENT			m_htComponent;
public:
	virtual void Free(void);
};

END

#endif // GameObject_h__