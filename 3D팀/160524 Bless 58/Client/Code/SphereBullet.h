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

#ifndef SphereBullet_h__
#define SphereBullet_h__

#include "Bullet.h"
#include "Include.h"
#include "Monsters.h"

class CMagician;
class CSphereBullet
	: public CBullet
{
private:
	explicit CSphereBullet(LPDIRECT3DDEVICE9 pDevice, WORD wNumber);
	virtual ~CSphereBullet(void);
public:
	virtual HRESULT Initialize();
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	void	SetUp();
	void	SphereTrans(_vec3 vPos , CMonsters* pTargetMon, float fAngle = 0.f);
public:
	void Collision_Monster();
	void Set_MonContainer(vector<CMonsters*>* pvecMon);
private:
	virtual HRESULT AddComponent(void);
public:
	static CSphereBullet* Create(LPDIRECT3DDEVICE9 pDevice , WORD wNumber);
	virtual void Free(void);
private:
	CMonsters*			m_pTargetMonster;
	CMagician*			m_pMagician;
	vector<CMonsters*>* m_pvecMonster;
	_float				m_fRadian;
	_vec3				m_vTargetPos;
	_float				m_fShotAcc;
	_float				m_fRotY_Angle;
	_float				m_fAxis_Angle;
	bool				m_bTemp;
	_float				m_fSkillAngle;
	_vec3				m_vEffectPos;
	bool				m_bFirst;
	_float				m_fTimeSkill;
};


#endif // SphereBullet_h__