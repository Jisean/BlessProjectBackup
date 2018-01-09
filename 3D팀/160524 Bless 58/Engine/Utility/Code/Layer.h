/*!
 * \file Layer.h
 * \date 2015/04/03 16:06
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
#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLayer(void);

private:
	typedef vector<CGameObject*>		OBJECTLIST;
	typedef HTable						HTOBJLIST;
	HTOBJLIST		m_htObjlist;

public:
	const CComponent*	GetComponent(const TCHAR* pObjKey, const TCHAR* pComponentKey);
	OBJECTLIST*			GetObjList(const TCHAR* pObjKey);
public:
	HRESULT AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject);

public:
	_int Update(const _float& fTimeDelta);
	void Render(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};

END

#endif // Layer_h__