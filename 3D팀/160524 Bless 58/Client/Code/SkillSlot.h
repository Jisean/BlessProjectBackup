/*!
 * \file UI.h
 * \date 2016/03/15 13:45
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

#ifndef SkillSlot_h__
#define SkillSlot_h__

#include "GameObject.h"
#include "Include.h"
#include "UI.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CPlayers;
class CMainPlayer;
class CSkillSlot : public CUI
{
public:
	enum eSKillImg{SK_GUARDIAN,SK_PALADIN, SK_MAGICIAN, SK_RANGER, SK_END};
private:
	explicit CSkillSlot(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSkillSlot(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
private:
	virtual HRESULT AddComponent(void);
	virtual HRESULT SetComponent(WORD wContainerID, const TCHAR* pResourcekey);
public:
	static CSkillSlot* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
private:
	Engine::CTransform*			m_pInfo;
	Engine::CTexture*			m_pTexture;
	Engine::CTexture*			m_pFilterTexture;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;
	D3DMATERIAL9				m_tMtrl;
	vector<UI_INFO>				m_vecSkill_ImgInfo[SK_END];
	vector<Engine::CTexture*>   m_vecSkill_Img[SK_END];
	CMainPlayer*					m_pMainPlayer;
private:
	_float			m_fX, m_fY;
	_float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
	bool			m_bActivate;
	bool			m_bPush;
	RECT			m_Rect;

private:
	float			SkillCoolTime(int iNum);
	void			SetUp();
	void			CollisionUI(void);
	void			LoadData(const wstring& wstrPath);
private:
	void	Move_Tex(void);
	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;
};

#endif // SkillSlot_h__