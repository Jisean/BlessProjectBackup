/*!
 * \file Stage.h
 * \date 2015/04/03 0:13
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 스테이지
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Stage2_h__
#define Stage2_h__

#include "Scene.h"
#include "Include.h"
#include "Units.h"

namespace Engine
{
	class CGameObject;
}

class CPlayers;
class CMonsters;
class CNPCs;
class CStage2
	: public Engine::CScene
{
//public:
//	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI, LAYER_EFFECT};

private:
	typedef HTable		HTREANIINFO;
	HTREANIINFO			m_htAniInfo;
	HTREANIINFO			m_htBoxInfo;
	vector<CMonsters*>  m_vecMonsters;
	vector<CUnits*>		m_vecPlayer_Npc_Info;
	vector<CPlayers*>   m_vecPlayerInfo;
	vector<CNPCs*>      m_vecNpc_Info;
	bool				m_bCheck;
	_float				m_fTimeDelta;
	bool				m_bBossBgm;
	bool				m_bClearBgm;
	bool				m_bBossCamReady;

private:
	CStage2(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage2(void);
public:
	virtual HRESULT InitScene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void	KeyCheck(const _float& fTimeDelta);
public:// 애니 박스
	void	LoadingAni(const wstring& wstrPath);
	void	LoadingBox(const wstring& wstrPath);
	void	SetAni_Box_Info(const TCHAR* pName, Engine::CGameObject* pObj);
	void	Print_KeyFont(void);
private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Effect_Layer(void);
	HRESULT Add_Light(void);
	void	LoadObject(Engine::CLayer*	pLayer);
	void	LoadOcclusionBox(void);
	HRESULT	LoadPlayerSet(Engine::CLayer* pLayer);
	HRESULT	LoadMonsterSet(Engine::CLayer* pLayer);
	HRESULT LoadBossSet(Engine::CLayer* pLayer);
	Engine::CGameObject* LoadEffectCreate(const wstring& wstrPath);
	HRESULT Init_NaviMesh(void);
public:
	static CStage2* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

// UI
private:
	void	Check_UI(void);

// Light
private:
	HRESULT	StartPoint(void);
	HRESULT	StartStreet(void);
	HRESULT	NormalField(void);
	HRESULT	NormalStreet(void);
	HRESULT	MidBoss(void);
	HRESULT	BossStreet(void);
	HRESULT	Boss(void);
	void	Occ_Trigger(void);

private:
	HRESULT LightAdd(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, TCHAR* pKeyName, int iRepeat = 1);
};

#endif // Stage2_h__