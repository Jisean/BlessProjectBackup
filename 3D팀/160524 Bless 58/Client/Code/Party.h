/*!
 * \file Party.h
 * \date 2016/06/25 17:52
 *
 * \author gpdms
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/


#ifndef Party_h__
#define Party_h__

#include "UI.h"
#include "Players.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CMainPlayer;
class CParty
	: public CUI
{
private:
	enum	eParty{		OUT_MAINHP, BAR_MAINHP, OUT_MAINMP, BAR_MAINMP, OUT_MAINRING, 
						OUT_1HP, BAR_1HP, OUT_1MP, BAR_1MP, OUT_1RING, 
						OUT_2HP, BAR_2HP, OUT_2MP, BAR_2MP, OUT_2RING, 
						OUT_3HP, BAR_3HP, OUT_3MP, BAR_3MP, OUT_3RING, 	
						
						CREST_GUARDIAN, CREST_MAGICIAN, CREST_PALADIN, CREST_RANGER,
						NAME_GUARDIAN, NAME_MAGICIAN, NAME_PALADIN, NAME_RANGER,

						CENTER_GUARDIAN, CENTER_MAGICIAN, CENTER_PALADIN, CENTER_RANGER,

						PARTY_END};

	enum	eBar{		_BAR_MAINHP, _BAR_MAINMP,
						_BAR_1HP, _BAR_1MP,
						_BAR_2HP, _BAR_2MP,
						_BAR_3HP, _BAR_3MP,		_BAR_END	};



private:
	explicit CParty(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CParty(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
	HRESULT			AddOutLineComponent(void);
	HRESULT			AddBarComponent(void);
public:
	static CParty* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	void	Add_PlayerParty(CPlayers::PLAYERJOB ePlayerJob, int iPos);

private:
	void	Init_Outline(void);
	void	Init_Bar(void);
	void	Init_Players(void);
	void	Set_Crest(CPlayers::PLAYERJOB ePlayer, int iPos);

private:
	void	Set_TextureInit(eParty eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);
	void	Set_Player(void);
	void	Set_Bar(void);

private:
	void	calculate_Bar(Engine::CStatus* pStatus, eParty HpEnum, eBar HpBar, eParty MpEnum, eBar MpBar);
	void	CenterCrest(void);
	bool	Check_CenterCrest(eParty eJobName);

protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;

private:
	Engine::CTexture*			m_pTexture[PARTY_END];

private:
	float			m_fX[PARTY_END], m_fY[PARTY_END];
	float			m_fSizeX[PARTY_END], m_fSizeY[PARTY_END];
	D3DXMATRIX		m_matView[PARTY_END], m_matProj;

	float			m_fOriBarSizeX[_BAR_END];
	float			m_fOriBarfX[_BAR_END];

private:
	CPlayers::PLAYERJOB		m_eMainPlayer;
	CPlayers::PLAYERJOB		m_eOldMainPlayer;
	CPlayers::PLAYERJOB		m_eOtherPlayer[CPlayers::JOB_END - 1];

private:
	CMainPlayer*				m_pMainPlayer;
	CPlayers*					m_pPlayer[CPlayers::JOB_END];

private:
	bool					m_bCenterCrest[CPlayers::JOB_END];
	

	// Å¬¶ó Åø
private:
	void	Init_MoveTex(eParty eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eParty m_e1st;
	eParty m_e2nd;
	eParty	m_eTexNum;
};


#endif // Party_h__