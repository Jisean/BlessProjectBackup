/*!
 * \file Option.h
 * \date 2016/06/21 20:17
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

#ifndef Option_h__
#define Option_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class COption
	: public CUI
{
public:
	enum eTextureName {OPTION_BORDER, /*OPTION_CHECK, OPTION_BAR, OPTION_SLIDE,*/ OPTION_BOTTOM, 
						CHECK_LIGHT, CHECK_HDR, CHECK_DOF, CHECK_SHADOW, CHECK_VOLUMETRIC,
						BAR_BRIGHTNESS, BAR_RANGE, BAR_POWER,
						SLIDE_BRIGHTNESS, SLIDE_RANGE, SLIDE_POWER,
						TXT_OPTION, TXT_LIGHT, TXT_HDR, TXT_BRIGHTNESS, TXT_BLOOM, TXT_RANGE, TXT_POWER,
						TXT_DOF, TXT_SHADOW, TXT_VOLUMETRIC, TXT_CANCEL, TXT_ACCEPT, OPTION_END};

	enum eGroup {GROUP_LIGHT, GROUP_HDR, GROUP_DOF, GROUP_SHADOW, GROUP_VOLUMETRIC, GROUP_END};
	enum eSlider {S_BRIGHTNESS, S_RANGE, S_POWER, S_END};
private:
	explicit COption(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~COption(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Free(void);
private:
	virtual HRESULT AddComponent(void);
public:
	static COption* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	void	Set_Option(eGroup eGroup, bool bRender);
	bool	Get_LightOption(void);
	void	Adjust_Light(void);

public:
	float	Get_OldLightPower(void) { return m_fOldLightPower; }
	void	Set_OldLightPower(float fPower) { m_fOldLightPower = fPower; }

private:
	void	Set_TextureInit(eTextureName eTexNum, float fX, float fY, float fSizeX, float fSizeY);
	void	Set_TexView(void);
	void	MakeRect(RECT& rc, eTextureName eTexNum, int iAdd = 0);

private:
	void	Check_Key(void);
	void	Check_Group(void);
	void	Check_Slider(void);

private:
	void	Update_Option(void);

protected:
	Engine::CResourceMgr*	m_pResourceMgr;
protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;
private:
	Engine::CTexture*			m_pTexture[OPTION_END];
	Engine::CTexture*			m_pCheckXTex;
private:
	float			m_fX[OPTION_END], m_fY[OPTION_END];
	float			m_fSizeX[OPTION_END], m_fSizeY[OPTION_END];
	D3DXMATRIX		m_matView[OPTION_END], m_matProj;
	bool			m_bCheck[OPTION_END];
private:
	bool			m_bCheckGroup[GROUP_END];
	RECT			m_rcCheckBox[GROUP_END];

	bool			m_bCheckSlider[S_END];
	RECT			m_rcSliderBox[S_END];

	RECT			m_rcCancelBox;
	RECT			m_rcAcceptBox;

private:
	float			m_fOldBloomPower;
	float			m_fOldBloomRange;
	float			m_fOldLightPower;

	float			m_fMinBloomPower;
	float			m_fMinBloomRange;
	float			m_fMinLightPower;

	float			m_fMaxBloomPower;
	float			m_fMaxBloomRange;
	float			m_fMaxLightPower;

// Å¬¶ó Åø
private:
	void	Init_MoveTex(eTextureName eTexNum);
	void	Move_Tex(void);

	float	m_MoveX, m_MoveY;
	float	m_MoveSizeX, m_MoveSizeY;

	eTextureName	m_eTexNum;
};


#endif // Option_h__