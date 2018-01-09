#include "stdafx.h"
#include "BulletMgr.h"
#include "FireArrow.h"
#include "Stage.h"
#include "Export_Function.h"
#include "HeightArrow.h"
#include "Bullet.h"
#include "SphereBullet.h"

IMPLEMENT_SINGLETON(CBulletMgr)

CBulletMgr::CBulletMgr()
: m_wReservedSize(0)
{

}
CBulletMgr::~CBulletMgr()
{

}
CBullet* CBulletMgr::Get_Bullet(eBulletType BulletTypeID , WORD NumberKey)
{
	vec_Bullet::iterator iter = m_vecBullet[BulletTypeID].begin();
	vec_Bullet::iterator iter_end = m_vecBullet[BulletTypeID].end();

	for(; iter != iter_end; ++iter)
	{
		if(((CBullet*)(*iter))->Get_wNumber() == NumberKey)
			return (*iter);
	}
	return NULL;
}
void CBulletMgr::Free(void)
{
	for(int i = 0; i < BTYPE_END; ++ i)
	{
		if(m_vecBullet[i].size() == 0)
			continue;

		//for(_uint j = 0; j < m_vecBullet[i].size(); ++j)
		//{
		//	//Engine::Safe_Delete(((CBullet*)(m_vecBullet[i])[j]));
		//
		//}
		//(m_vecBullet[i])

		
			//m_vecBullet[i].erase(iter);

		m_vecBullet[i].clear();
	}
	
}
void CBulletMgr::Arrow_Trans(eBulletType BulletType ,_vec3 vPos,/*갯수*/ WORD wCnt , /*속도*/float fSpeed,CMonsters* pTarGetMon ,eActBullet ActBullet, /*몇초간격*/ float fTime /*= 1.f*/, float fAngle /*= 0.f*/)
{
	if(m_vecBullet[BulletType].size() == 0)
	{
		MSG_BOX(L"컨테이너 사이즈 0");
		return;
	}

	if(BulletType == BTYPE_ARROW)
		(*(CFireArrow*)(m_vecBullet[BulletType])[wCnt]).ArrowTrans(vPos,fSpeed,fTime,pTarGetMon,(CFireArrow::eActBullet)ActBullet);
	if(BulletType == BTYPE_H_ARROW)
		(*(CHeightArrow*)(m_vecBullet[BulletType])[wCnt]).ArrowTrans(vPos,fSpeed,fTime);
}

HRESULT CBulletMgr::Reserve_Bullet(LPDIRECT3DDEVICE9 pDevice , eBulletType BulletType , WORD wSize /*= 20*/)
{
	if(m_wReservedSize < wSize)
	{
		m_wReservedSize = wSize;
		m_vecBullet[BulletType].reserve(m_wReservedSize);
	}
	
	for(int i = 0; i < m_wReservedSize; ++i)
	{
		CBullet* pBullet = NULL;

		switch(BulletType)
		{
		case BTYPE_ARROW:
			pBullet = CFireArrow::Create(pDevice,wSize);
			if(NULL != pBullet)
				(Engine::Get_Scene())->AddObject(LAYER_GAMELOGIC,L"FireArrow",pBullet);
			break;
		case BTYPE_H_ARROW:
			pBullet = CHeightArrow::Create(pDevice,wSize);
			if(NULL != pBullet)
				(Engine::Get_Scene())->AddObject(LAYER_GAMELOGIC,L"HeightArrow",pBullet);
			break;
		case BTYPE_MAGIC:
			break;
		case BTYPE_SPHERE:
			pBullet = CSphereBullet::Create(pDevice,wSize);
			if(NULL != pBullet)
				(Engine::Get_Scene())->AddObject(LAYER_GAMELOGIC,L"SphereBullet",pBullet);
			break;
		}

		NULL_CHECK_RETURN(pBullet, E_FAIL);

		m_vecBullet[BulletType].push_back(pBullet);
	}


	return E_FAIL;
}

HRESULT CBulletMgr::Resize_Bullet(eBulletType BulletType , WORD wSize)
{
	if(m_vecBullet[BulletType].size() <= wSize)
	{
		m_vecBullet[BulletType].resize(wSize);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CBulletMgr::ReserveSize_Check(eBulletType BulletType)
{
	if(m_vecBullet[BulletType].size() == 0)
		return S_OK;

	return E_FAIL;
}

void CBulletMgr::Set_MonContainer(vector<CMonsters*>* pvecMon , WORD Index)
{
	(*(CFireArrow*)(m_vecBullet[BTYPE_ARROW])[Index]).Set_MonContainer(pvecMon);
}

void CBulletMgr::Sphere_Trans(eBulletType BulletType,WORD wCnt  , _vec3 vPos, CMonsters* pTargetMon /*= NULL*/, _float fAngle /*= 0.f*/)
{
	if(m_vecBullet[BulletType].size() == 0)
	{
		MSG_BOX(L"컨테이너 사이즈 0");
		return;
	}

	if(BulletType == BTYPE_SPHERE)
		(*(CSphereBullet*)(m_vecBullet[BulletType])[wCnt]).SphereTrans(vPos,pTargetMon,fAngle);
}

WORD CBulletMgr::ReadyBullet_Count(eBulletType BulletTypeID )
{
	WORD wCnt = 999;
	if(BulletTypeID == BTYPE_ARROW)
	{
		for(_uint i = 0; i < m_vecBullet[BulletTypeID].size(); ++i)
		{
			if((*(CFireArrow*)(m_vecBullet[BulletTypeID])[i]).GetDead())
			{
				wCnt = (WORD)i;
				break;
			}
		}
	}
	if(BulletTypeID == BTYPE_H_ARROW)
	{
		for(_uint i = 0; i < m_vecBullet[BulletTypeID].size(); ++i)
		{
			if((*(CHeightArrow*)(m_vecBullet[BulletTypeID])[i]).GetDead())
			{
				wCnt = (WORD)i;
				break;
			}
		}
	}

	if(BulletTypeID == BTYPE_SPHERE)
	{
		for(_uint i = 0; i < m_vecBullet[BulletTypeID].size(); ++i)
		{
			if((*(CSphereBullet*)(m_vecBullet[BulletTypeID])[i]).GetDead())
			{
				wCnt = (WORD)i;
				break;
			}
		}
	}

	if(wCnt == 999)
		ReadyBullet_Count(BulletTypeID);
	
	return wCnt;
}

