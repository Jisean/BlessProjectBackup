/*!
 * \file MonsterBar.h
 * \date 2016/06/23 22:04
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

#ifndef MonsterBar_h__
#define MonsterBar_h__


#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CMonsterBar
	: public CUI
{
private:
	enum	eMonsterHP {HP_OUTLINE, HP_BAR, HP_BLOOD, HP_END };
private:
	explicit CMonsterBar(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMonsterBar(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static CMonsterBar* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void	Set_TextureInit(eMonsterHP eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);

private:
	void	MonsterHP(void);
	void	Check_Blood(void);

protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;

private:
	Engine::CTexture*			m_pTexture[HP_END];

private:
	float			m_fX[HP_END], m_fY[HP_END];
	float			m_fSizeX[HP_END], m_fSizeY[HP_END];
	D3DXMATRIX		m_matView[HP_END], m_matProj;

	float			m_fOriHpSizeX;
	float			m_fOriHpfX;

	bool			m_bBlood;
	float			m_fBloodAlpha;
	D3DXVECTOR3		m_vTargetPos;

private:
	float			m_fTimeDelta;

// Å¬¶ó Åø
private:
	void	Init_MoveTex(eMonsterHP eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eMonsterHP	m_eTexNum;
};

#endif // MonsterBar_h__