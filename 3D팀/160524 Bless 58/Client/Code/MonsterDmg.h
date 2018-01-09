/*!
 * \file MonsterDmg.h
 * \date 2016/07/04 13:48
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


#ifndef MonsterDmg_h__
#define MonsterDmg_h__

#include "UI.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CMonsterDmg : public CUI
{
private:
	explicit CMonsterDmg(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterDmg(void);
public:
	virtual HRESULT Initialize(const _vec3* pPos, const _float& fDmg);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CMonsterDmg* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _float& fDmg);
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

#endif // MonsterDmg_h__