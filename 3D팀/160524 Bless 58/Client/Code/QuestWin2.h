/*!
 * \file QuestWin2.h
 * \date 2016/07/03 14:13
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

#ifndef QuestWin2_h__
#define QuestWin2_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CQuestWin2
	: public CUI
{
private:
	enum	eQuest{	QUESTWIN_MAINTXTOUTLINE, QUESTWIN_MAINTXT, QUESTWIN_MAINMARK, 
		QUESTWIN_SAVEQUESTOUTLINE, QUESTWIN_SAVEQUESTNUM, QUESTWIN_SAVEQCOMPLETE, QUESTWIN_SAVET,
		QUESTWIN_MAINQUEST1, QUESTWIN_MAINQUEST2, QUESTWIN_END };

private:
	explicit CQuestWin2(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CQuestWin2(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static CQuestWin2* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void	Set_TextureInit(eQuest eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);
	float	CompleteAlpha(float fTimeDelta, int iComplete);

	void	Set_QuestNpc(void);

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
	float			m_fMaxTime;
	float			m_fCurTime;
	bool			m_bStart;
	float			m_fTimeDelta;


	_ushort			m_sNum;
	float			m_fSaveQAlpha;


	// Å¬¶ó Åø
private:
	void	Init_MoveTex(eQuest eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eQuest	m_eTexNum;
};

#endif // QuestWin2_h__