/*!
 * \file Trail_Effect.h
 * \date 2015/04/05 8:25
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
#ifndef Trail_Effect_h__
#define Trail_Effect_h__

#include "GameObject.h"
#include "Include.h"
#include "Equipment.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};

class CTrail_Effect
	: public Engine::CGameObject
{
#define			LINECOUNT	20
#define			POINTCOUNT	40
#define			LINESIZE	LINECOUNT * POINTCOUNT
public:
	enum	TRAIL_TYPE { TRAIL_SWORD, TRAIL_MACE, TRAIL_DASH, TRAIL_NONE};	

private:
	CTrail_Effect(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTrail_Effect(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	void	SetTarget(CEquipment* pTarget);
	void	SetType(TRAIL_TYPE eType);

private:
	virtual HRESULT AddComponent(void);
	void	SetCatmullRom();

public:
	static CTrail_Effect* Create(LPDIRECT3DDEVICE9 pDevice, TRAIL_TYPE eType);
	virtual void	Free(void);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CShader*		m_pShader;

	CEquipment*				m_pTarget;

	TRAIL_TYPE				m_eType;	
	bool					m_bFree;
	_float					m_fFreeTime;
	_float					m_fTimeDelta;

private:
	_vec3				m_vTopPoint[LINECOUNT + 1];
	_vec3				m_vBotPoint[LINECOUNT + 1];
	_vec3				m_vCatPoint[LINESIZE * 2];
	_mymat				m_matWorld[LINESIZE * 2];
	D3DMATERIAL9		m_tMtrl;
	_float				m_fFrame;
	_float				m_fTime;
	_float				m_fTimeLast;
};

#endif // Trail_Effect_h__