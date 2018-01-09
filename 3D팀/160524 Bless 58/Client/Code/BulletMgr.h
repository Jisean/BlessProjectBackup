#ifndef BulletMgr_h__
#define BulletMgr_h__

#include "Include.h"
#include "Base.h"
#include "Engine_Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTransform;
}

class CBullet;
class CMonsters;
class CBulletMgr : public Engine::CBase
{
	DECLARE_SINGLETON(CBulletMgr)
public:
	enum eBulletType {BTYPE_MAGIC,BTYPE_ARROW,BTYPE_H_ARROW,BTYPE_SPHERE,BTYPE_END};
	enum eActBullet {AB_STUN,AB_DOWN,AB_DMG,AB_NORMAL,AB_WIDTH,AB_DEFAULT};
public:
	CBullet* Get_Bullet(eBulletType BulletTypeID , WORD NumberKey);
	WORD	ReadyBullet_Count(eBulletType BulletTypeID);
	void	Arrow_Trans(eBulletType BulletType ,_vec3 vPos,/*갯수*/	WORD wCnt ,	/*속도*/float fSpeed, CMonsters* pTarGetMon ,eActBullet ActBullet = AB_DEFAULT ,/*몇초간격*/ float fTime = 1.f , float fAngle = 0.f);
	void	Sphere_Trans(eBulletType BulletType,WORD wCnt , _vec3 vPos, CMonsters* pTargetMon = NULL,  _float fAngle = 0.f);
	HRESULT Reserve_Bullet(LPDIRECT3DDEVICE9 pDevice, eBulletType BulletType , WORD wSize = 50);
	HRESULT Resize_Bullet(eBulletType BulletType , WORD wSize);
	HRESULT ReserveSize_Check(eBulletType BulletType);
	void	Set_MonContainer(vector<CMonsters*>* pvecMon , WORD Index);
private:
	typedef vector<CBullet*> vec_Bullet;
	vec_Bullet	m_vecBullet[BTYPE_END];
	WORD		m_wReservedSize;
	float		m_fTimeDelta;
private:
	CBulletMgr();
	~CBulletMgr();
public:
	virtual void Free(void);
};

#endif // BulletMgr_h__
