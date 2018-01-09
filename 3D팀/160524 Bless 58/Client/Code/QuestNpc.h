/*!
 * \file CQuestNpc.h
 * \date 2016/06/29 15:30
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

#ifndef CQuestNpc_h__
#define CQuestNpc_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CQuestNpc
	: public CUI
{
private:
	enum	eQuest{QUEST_BACK, QUEST_TXT, QUEST_BUTTON, QUEST_ACCEPT, QUEST_END };
private:
	explicit CQuestNpc(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CQuestNpc(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static CQuestNpc* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	float	Get_LightPower(void) { return m_fLightPower; }

private:
	void	Set_TextureInit(eQuest eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);
	void	MakeRect(RECT& rc, eQuest eTexNum, int iAdd = 0);

protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;

private:
	Engine::CTexture*			m_pTexture[QUEST_END];
	Engine::CTexture*			m_pGradationTex;

private:
	float			m_fX[QUEST_END], m_fY[QUEST_END];
	float			m_fSizeX[QUEST_END], m_fSizeY[QUEST_END];
	D3DXMATRIX		m_matView[QUEST_END], m_matProj;

private:
	RECT			m_rcAccept;
	float			m_fLightPower;

private:
	float			m_fMaxTime;
	float			m_fCurTime;

// Å¬¶ó Åø
private:
	void	Init_MoveTex(eQuest eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eQuest	m_eTexNum;
};


#endif // CQuestNpc_h__