/*!
 * \file QuestComplete1.h
 * \date 2016/07/02 17:25
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

#ifndef QuestComplete1_h__
#define QuestComplete1_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CQuestComplete1
	: public CUI
{
private:
	enum	eComplete{COMPLETE_BACK, COMPLETE_MARK, COMPLETE_TXT, COMPLETE_END };
private:
	explicit CQuestComplete1(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CQuestComplete1(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static CQuestComplete1* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	bool	Get_NewQuest(void) { return m_bNewQuest; }

private:
	void	Set_TextureInit(eComplete eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);

protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;

private:
	Engine::CTexture*			m_pTexture[COMPLETE_END];

private:
	float			m_fX[COMPLETE_END], m_fY[COMPLETE_END];
	float			m_fSizeX[COMPLETE_END], m_fSizeY[COMPLETE_END];
	D3DXMATRIX		m_matView[COMPLETE_END], m_matProj;

private:
	float			m_fMaxTime;
	float			m_fCurTime;

private:
	bool			m_bNewQuest;
	bool			m_bEndQuest;

	// Å¬¶ó Åø
private:
	void	Init_MoveTex(eComplete eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eComplete	m_eTexNum;
};



#endif // QuestComplete1_h__