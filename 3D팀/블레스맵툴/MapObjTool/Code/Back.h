/*!
 * \file Back.h
 * \date 2016/05/09 16:06
 *
 * \author Nia
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Back_h__
#define Back_h__

#include "Scene.h"
#include "Include.h"
#include "DynamicCamera.h"
#include "Terrain.h"
#include "MouseCol.h"

namespace Engine
{
	class CManagement;
	class CResourceMgr;
	class CGameObject;
	class CLayer;
	class CNaviMgr;
	class CNaviCell;
}

class CMapObjToolView;
class CBack : public Engine::CScene
{
public:
	enum SCENE {SCENE_BACK, SCENE_END};
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI };
	SCENE m_eSceneType;
	
public:
	CMouseCol*					m_pMouseCol;
	//Engine::CLayer*				m_pGameLogicLayer;
	CTerrain*					m_pTerrain;
	typedef list<Engine::CGameObject*>	OBJECTLIST;
	OBJECTLIST					m_ObjectList;
	typedef map<const TCHAR*, OBJECTLIST*>	MAPOBJLIST;
	MAPOBJLIST					m_ObjectMap;
	bool						m_bPickFirst;
	bool						m_bPickSecond;
	D3DXVECTOR3					m_vPoint[3];
	bool						m_bGetMousePt;
	D3DXVECTOR3					m_vGetMousePt;
	CMapObjToolView*			m_pMainView;
	TCHAR						m_szMemoryLoad[128];
	TCHAR						m_szTotal[128];
	bool						m_bCamSpeedUp;
	bool						m_bCamSpeedDown;
	bool						m_bCameraFix;
	bool						m_bPickCheck;
	int							m_iNaviIndex;
	Engine::CNaviCell*			m_pSelectedCell;

	D3DXVECTOR3					m_vPickingPos;


private:
	Engine::VTXTEX*				m_pTerrainVtx;
	Engine::VTXCOL*				m_pBoundingVtx;
	Engine::INDEX16*			m_pBoundingIdx;
	Engine::VTXCOL*				m_pOccVtx;
	Engine::INDEX16*			m_pOccIdx;


public:
	void SetMainView(CMapObjToolView* pMainView);
	void ConstMapMode(void);
	void ConstObjectMode(void);
	void ConstSplatingMode(void);
	void NavimeshMode(void);
	void BuildNavimesh(void);
	void ConstOcclusionBox(void);
	HRESULT	MakeTerrainBuffer(WORD wCntX, WORD wCntZ, WORD wItv);
	HRESULT	MakeTerrainLayer(WORD wCntX, WORD wCntZ, float fUVCnt);
	static bool	Compare(Engine::CGameObject* pDest, Engine::CGameObject* pSource);


private:
	CBack(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBack(void);

private:
	virtual	HRESULT	InitScene(void); 
	virtual int Update(const float& fTimeDelta);
	virtual	void Render(void);

private:
	HRESULT	Add_TexBuffer(void);
	HRESULT	Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);	
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Light(void);

	void	ComparePoint(D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2, D3DXVECTOR3& vPoint3);

public:
	static CBack* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);



};

#endif // Back_h__