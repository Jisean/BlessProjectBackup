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

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene(void);
public:
	const CComponent* GetComponent(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey);

public:
	virtual HRESULT InitScene(void) PURE;
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

	_tchar					m_szFPS[128];
	_float					m_fTimeAcc;
	_ulong					m_dwFPSCnt;

public:
	typedef map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;
protected:
	void Render_FPS(void);

protected:
	virtual void Free(void);

public:
	const CLayer*	GetLayer(WORD wLayerKey)
	{
		MAPLAYER::iterator iter = m_mapLayer.find(wLayerKey);
		if(iter != m_mapLayer.end())
			return iter->second;
		else
			return NULL;
	}
};

END

#endif // Scene_h__