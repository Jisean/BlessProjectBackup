/*!
 * \file QuestMark.h
 * \date 2016/07/03 10:31
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

#ifndef QuestMark_h__
#define QuestMark_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CQuestMark : public CUI
{
public:
	enum	eQuest {QUEST_1, QUEST_2, QUEST_3, QUEST_END};

private:
	explicit CQuestMark(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CQuestMark(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);

public:
	void	Set_Pos(D3DXVECTOR3 vPos, eQuest eNum) { m_vPos[eNum] = vPos; }
	void	Set_Alpha(eQuest eQuestNum, float fAlpha) { m_fAlpha[eQuestNum] = fAlpha; }
private:
	virtual HRESULT AddComponent(void);
public:
	static CQuestMark* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	_myvec					m_vPos[QUEST_END];
	_float					m_fAlpha[QUEST_END];
	_float					m_fSpeed;

private:
	float					m_fMoveY[QUEST_END];
	bool					m_bDown[QUEST_END];

protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;
	Engine::CTexture*			m_pTexture;

};



#endif // QuestMark_h__