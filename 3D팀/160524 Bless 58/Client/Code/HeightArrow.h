/*!
 * \file Bow.h
 * \date 2016/03/14 14:33
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

#ifndef HeightArrow_h__
#define HeightArrow_h__

#include "Bullet.h"
#include "Include.h"

class CRanger;
class CHeightArrow
	: public CBullet
{
private:
	explicit CHeightArrow(LPDIRECT3DDEVICE9 pDevice, WORD wNumber);
	virtual ~CHeightArrow(void);
public:
	virtual HRESULT Initialize();
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	void Collision_Monster();
	void Collision_Map();
private:
	virtual HRESULT AddComponent(void);
public:
	static CHeightArrow* Create(LPDIRECT3DDEVICE9 pDevice , WORD wNumber);
	virtual void Free(void);
public:
	void	ArrowTrans(_vec3 vDir,float fSpeed,float fTime , float fAngle = 0.f);
private:
	Engine::CTransform*	m_pTransform_Ranger;
	CRanger*			m_pRanger;
	const _matrix*		m_matHandle;
	float		m_fTimeCount , m_fTimeMax;
	_vec3		m_vTargetPos;
	_float		m_fDistance;
	_float		m_fShotAcc;
	_float		m_fRadian;
	bool		m_bTemp;
	_float		m_fTimeSecond;
	_vec3		m_vLastDir;
	_vec3		m_vPrevPos;
};


#endif // HeightArrow_h__