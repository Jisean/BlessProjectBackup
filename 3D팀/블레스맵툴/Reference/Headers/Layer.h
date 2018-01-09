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

public:
	const CComponent* GetComponent(const TCHAR* pObjKey, const TCHAR* pComponentKey);
	const Engine::VTXTEX* Engine::CLayer::GetTerrainVertex(const TCHAR* wstrObjKey);

public:
	HRESULT AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject);
	void	DeleteByKey(const TCHAR* wstrObjKey);
	void	DeleteByCompare(const CGameObject* pGameObject);

public:
	_int Update(const _float& fTimeDelta);
	void Render(void);
public:
	list<CGameObject*>* GetObjectList(const TCHAR* pObjKey)
	{
		MAPOBJLIST::iterator Mapiter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjKey));

		if(Mapiter != m_mapObjlist.end())
			return &(Mapiter->second);
		else
			return NULL;
	}

private:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<const TCHAR*, OBJECTLIST>	MAPOBJLIST;
	MAPOBJLIST		m_mapObjlist;

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};

END

#endif // Layer_h__