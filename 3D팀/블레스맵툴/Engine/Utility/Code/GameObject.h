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
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGameObject(void);
public:
	const CComponent* GetComponent(const TCHAR* pComponentKey);
	const float& Get_ViewZ(void) const {return m_fViewZ;}
	virtual const Engine::VTXTEX*	GetTerrainVertex(void){	return NULL; }

public:
	virtual HRESULT	Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void){}

protected:
	virtual HRESULT AddComponent(void) {return S_OK;}

protected:
	void Compute_ViewZ(const D3DXVECTOR3* pPos);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	float		m_fViewZ;

public:
	typedef map<const TCHAR*, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;
public:
	virtual void Free(void);
};

END

#endif // GameObject_h__