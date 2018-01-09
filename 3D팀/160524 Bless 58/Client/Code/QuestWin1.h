/*!
 * \file QuestWin1.h
 * \date 2016/06/30 11:19
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

#ifndef QuestWin1_h__
#define QuestWin1_h__

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

class CQuestWin1
	: public CUI
{
private:
	enum	eQuest{	QUESTWIN_MAINTXTOUTLINE, QUESTWIN_MAINTXT, QUESTWIN_MAINMARK, 
					QUESTWIN_MAINQUEST1, QUESTWIN_MAINQUEST2, QUESTWIN_MAINQUEST3,
					QUESTWIN_MAINQUEST1S, QUESTWIN_MAINQUEST2S, QUESTWIN_MAINQUEST3S,
					QUESTWIN_MAINQUEST1T, QUESTWIN_MAINQUEST2T, QUESTWIN_MAINQUEST3T,
					QUESTWIN_SUBQUEST, QUESTWIN_SUBQUESTS, QUESTWIN_SUBQESTT,
					QUESTWIN_END };

	enum	eQuestCheck{	QUESTCK_MAINQ1, QUESTCK_MAINQ2, QUESTCK_MAINQ3,
							QUESTCK_SUB,	QUESTCK_END };
private:
	explicit CQuestWin1(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CQuestWin1(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static CQuestWin1* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	void	Set_MainQuestCheck(CPlayers::PLAYERJOB eJob, bool bMonsterHunt = false);
	bool	Get_MainQuestCheck(CPlayers::PLAYERJOB eJob, bool bMonsterHunt = false);

private:
	void	Set_TextureInit(eQuest eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);
	void	MakeRect(RECT& rc, eQuest eTexNum, int iAdd = 0);

	void	QuestCheck(void);
	int		QuestPosCnt(void);
	void	CompleteAlpha(float fTimeDelta);

private:
	float	Get_CompleteQuestAlpha(eQuest eQuestName);
private:
	void	Cheat_QeustComplete(void);
protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;

private:
	Engine::CTexture*			m_pTexture[QUESTWIN_END];

private:
	float			m_fX[QUESTWIN_END], m_fY[QUESTWIN_END];
	float			m_fSizeX[QUESTWIN_END], m_fSizeY[QUESTWIN_END];
	D3DXMATRIX		m_matView[QUESTWIN_END], m_matProj;

private:
	float			m_fBarOriSizeX;
	float			m_fBarOrifX;
	float			m_fMaxMonster;
	float			m_fCurMonster;


private:
	float			m_fCompleteQuestAlpha[QUESTCK_END];

private:
	bool			m_bQuestGroup[QUESTCK_END];
	bool			m_bQuestCK[QUESTWIN_END];


// Quest 이펙트
private:
	_myvec			m_vQuestEffect;

	// 클라 툴
private:
	void	Init_MoveTex(eQuest eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eQuest	m_eTexNum;
};


#endif // QuestWin1_h__