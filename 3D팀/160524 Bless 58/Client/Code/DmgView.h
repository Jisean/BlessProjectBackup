#ifndef DmgView_h__
#define DmgView_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CDmgView : public CUI
{
private:
	explicit CDmgView(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDmgView(void);
public:
	virtual HRESULT Initialize(const _vec3* pPos, const _float& fDmg);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CDmgView* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _float& fDmg);
private:
	_vec3					m_vPos;
	_int					m_iDmg;
	_uint					m_iCnt;
	_uint					m_iCnt2;
	_float					m_fAddY;
	_float					m_fTime;
	_float					m_fScaleRatio;
	_float					m_fUpSpeed;
	_float					m_fRightSpeed;
	_float					m_fDrawRatio;

private:
	_float					m_fPosRandX;
	_float					m_fPosRandY;

	_float					m_fAlpha;

	_float					m_fMoveSize;

protected:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;
	Engine::CTexture*			m_pTexture;

private:
	HRESULT Add_Component(void);
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);
	void Compute_Cnt(void);
public:
	virtual void Free(void);
};


#endif // DmgView_h__
