/*!
 * \file Scene.h
 * \date 2015/04/02 23:10
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
#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene(void);
public:
	const CComponent* GetComponent(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey);
	vector<CGameObject*>*	GetObjList(const WORD LayerID, const TCHAR* pObjKey);

public:
	virtual HRESULT InitScene(void) PURE;
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	HRESULT AddObject( const WORD LayerID, const TCHAR* pObjectKey, CGameObject* pGameObject);


protected:
	LPDIRECT3DDEVICE9		m_pDevice;
	_tchar					m_szFPS[128];
	_tchar					m_szMemoryLoad[128];
	_tchar					m_szTotal[128];
	_float					m_fTimeAcc;
	_ulong					m_dwFPSCnt;

protected:
	typedef map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;
protected:
	void Render_FPS(void);
	void Render_Memory(void);

protected:
	virtual void Free(void);
};

END

#endif // Scene_h__