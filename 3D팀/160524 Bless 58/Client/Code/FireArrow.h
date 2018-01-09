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

#ifndef FireArrow_h__
#define FireArrow_h__

#include "Bullet.h"
#include "Include.h"
#include "Monsters.h"
#include "Players.h"

class CRanger;
class CFireArrow
	: public CBullet
{
public:
	enum eActBullet {AB_STUN,AB_DOWN,AB_DMG,AB_NORMAL,AB_WIDTH,AB_DEFAULT};
private:
	explicit CFireArrow(LPDIRECT3DDEVICE9 pDevice, WORD wNumber);
	virtual ~CFireArrow(void);
public:
	virtual HRESULT Initialize();
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	void	StunShot();
	void	DownShot();
	void	DmgShot();
	void	NormalShot();
	void	WidthShot();
	_vec3	SharePos();
	void	SetUp();
public:
	void Collision_Monster();
	void Collision_Width_Monster();
	void Collision_Map();
	void Set_MonContainer(vector<CMonsters*>* pvecMon);
private:
	virtual HRESULT AddComponent(void);
public:
	static CFireArrow* Create(LPDIRECT3DDEVICE9 pDevice , WORD wNumber);
	virtual void Free(void);
public:
	void	ArrowTrans(_vec3 vPos,float fSpeed,float fTime,CMonsters* pTarGetMon ,eActBullet ActBullet = AB_DEFAULT, float fAngle = 0.f);
private:
	Engine::CTransform*	m_pTransform_Ranger;
	CRanger*			m_pRanger;
	const _matrix*		m_matHandle;
	float				m_fTimeCount , m_fTimeMax;
	_vec3				m_vTargetPos;
	_float				m_fDistance;
	_float				m_fShotAcc;
	_float				m_fRadian;
	bool				m_bTemp;
	_float				m_fTimeSecond;
	_vec3				m_vLastDir;
	eActBullet  		m_eActBullet;
	CMonsters*			m_pTargetMonster;
	bool				m_bWidthShot;
	vector<CMonsters*>* m_pvecMonster;
	_bool				m_bEffect_First;
	WORD				m_wEffectNumber;
	WORD				m_wRanger_TrackID;
	CPlayers::TRACKID	m_eRanger_TrackID; 
	_vec3				m_vPrevPos;

	_vec3				m_vTemp;
};


#endif // FireArrow_h__