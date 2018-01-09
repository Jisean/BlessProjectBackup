#include "stdafx.h"
#include "Collision.h"
#include "Export_Function.h"
#include "Stage.h"
#include "TerrainTex.h"
#include "Transform.h"
#include "Players.h"
#include "MainPlayer.h"
#include "DynamicMesh.h"
#include "StaticMesh.h"
#include "DynamicCamera.h"
#include "KeyMgr.h"

CCollision::CCollision(void)
{

}

CCollision::~CCollision(void)
{

}

CCollision* CCollision::Create(void)
{
	CCollision* pInstance = new CCollision;

	return pInstance;	
}

void CCollision::Free(void)
{
	
}

_bool CCollision::Collision_Distance(Engine::CTransform* pInfo1, Engine::CTransform* pInfo2, float fCollRadius)
{
	_bool bColl = false;

	_myvec vDist = pInfo1->m_vPos - pInfo2->m_vPos;
	_float fDist = vDist.Length();

	bColl = fDist < fCollRadius ? true : false;

	return bColl;
}

_bool CCollision::Collision_Distance(Engine::CTransform* pInfo1, D3DXVECTOR3 vPos, float fCollRadius)
{
	_bool bColl = false;

	_myvec vDist = pInfo1->m_vPos - vPos;
	_float fDist = vDist.Length();

	bColl = fDist < fCollRadius ? true : false;

	return bColl;
}

_bool CCollision::Collision_Distance(D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, float fCollRadius)
{
	_bool bColl = false;

	_myvec vDist = vPos1 - vPos2;
	_float fDist = vDist.Length();

	bColl = fDist < fCollRadius ? true : false;

	return bColl;
}

void CCollision::Collision_Mesh_Player(Engine::CTransform* pInfo, Engine::CStaticMesh* pMesh)
{
	// 안쓸거임
	CPlayers* pPlayer[4];
	for(_uint i = 0; i < 4; ++i)
	{
		pPlayer[i] = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front())->Get_PlayerSet(i);
	}
	Engine::CTransform* pPlayerInfo[4];
	Engine::CDynamicMesh* pPlayerMesh[4];
	for(_uint i = 0; i < 4; ++i)
	{
		pPlayerInfo[i] = dynamic_cast<Engine::CTransform*>(const_cast<Engine::CComponent*>(pPlayer[i]->GetComponent(L"Transform")));
		pPlayerMesh[i] = dynamic_cast<Engine::CDynamicMesh*>(const_cast<Engine::CComponent*>(pPlayer[i]->GetComponent(L"Mesh")));
	}
	
	BOOL	bInterSect[4] = {FALSE,};
	DWORD	dwIndex[4];
	float	fU[4], fV[4], Dist[4];
	_myvec vOriPos[4] = {pPlayerInfo[0]->m_vPos, pPlayerInfo[1]->m_vPos, pPlayerInfo[2]->m_vPos, pPlayerInfo[3]->m_vPos};
	D3DXVECTOR3 vDownDir = D3DXVECTOR3(0.f, -1.f, 0.f); 
	_myvec vDown[4] = {vDownDir, vDownDir, vDownDir, vDownDir};

	//cout << vOriPos[3].x << " " <<  vOriPos[3].y << " " <<  vOriPos[3].z << endl;
	_mymat		matWorld[4];
	for(_uint i = 0; i < 4; ++i)
	{
		//matWorld[i].Identity();
		matWorld[i].Inverse(&pInfo->m_matWorld);
		vOriPos[i].TransformCoord(&vOriPos[i], &matWorld[i]);
		vDown[i].TransformNormal(&vDown[i], &matWorld[i]);
		D3DXIntersect(*(pMesh->Get_Mesh()), &vOriPos[i], &vDown[i], &bInterSect[i], &dwIndex[i], &fU[i], &fV[i], &Dist[i], NULL, NULL);
	}
	cout<< "법사 위치 로컬"  << vOriPos[3].x << " " <<  vOriPos[3].y << " " <<  vOriPos[3].z << endl;
	cout<< "다운 위치 로컬"  << vDown[3].x << " " <<  vDown[3].y << " " <<  vDown[3].z << endl;
	cout<< "거리"  << Dist[3] << endl;

	//cout << vOriPos[1].x << " " <<  vOriPos[1].y << " " <<  vOriPos[1].z << endl;
	//cout << vOriPos[2].x << " " <<  vOriPos[2].y << " " <<  vOriPos[2].z << endl;
	//cout << vOriPos[3].x << " " <<  vOriPos[3].y << " " <<  vOriPos[3].z << endl;

	/*for(_uint i = 0; i < 4; ++i)
	{
		if(bInterSect[i] && Dist[i] <= 1.f)
		{
			pPlayer[i]->Set_RideOnMesh(true);
			_myvec vPos = vOriPos[i] - vDown[i] * (1.f - Dist[i]);
			vPos.TransformCoord(&vPos, &pInfo->m_matWorld);
			pPlayerInfo[i]->m_vPos = vPos;
		}
		else
			pPlayer[i]->Set_RideOnMesh(false);
	}*/
}


bool CCollision::CollisionSphere(Engine::CTransform* Info1, Engine::CTransform* Info2)
{
	float fWidth = Info1->m_vPos.x - Info2->m_vPos.x;
	float fHeight = Info1->m_vPos.z - Info2->m_vPos.z;
	float fDistance = (fWidth * fWidth) + (fHeight * fHeight);

	fDistance = sqrt(fDistance);

	float OriginDis = 0.15f / 2.f + 0.15f / 2.f;

	if(OriginDis > fDistance)
	{
		float Temp = OriginDis - fDistance;
		D3DXVECTOR3 vDir = Info1->m_vPos - Info2->m_vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		Info1->m_vPos += vDir * Temp * 0.15f;

		return true;
	}

	return false;
}

void CCollision::Collision_Mesh_Cam(CDynamicCamera* pCam, Engine::CTransform* pInfo, Engine::CStaticMesh* pMesh)
{
	_myvec vEye		= pCam->GetEye();
	_myvec vAt		= pCam->GetAt();
	_mymat matInverse = pInfo->m_matWorld;
	matInverse.InverseThis(&matInverse);

	vEye.TransformCoord(&vEye, &matInverse);
	vAt.TransformCoord(&vAt, &matInverse);

	_myvec vDir(vEye - vAt);
	
	vDir.Normalize();

	BOOL	bInterSect = FALSE;
	DWORD	dwIndex = 0;
	float	fU = 0.f;
	float	fV = 0.f;
	float	fDist = 0.f;

	D3DXIntersect(*(pMesh->Get_Mesh()), &vAt, &vDir, &bInterSect, &dwIndex, &fU, &fV, &fDist, NULL, NULL);

	if(TRUE == bInterSect)
	{
		_myvec vLen(vEye - vAt);
		_float fLen = vLen.Length();

		fDist -= fLen;
	
		if(pCam->Get_Distance() > fDist && fDist >= 0.5f)
		{
			pCam->Set_Distance(fDist);
			pCam->RenewalCamera();
		}
	}


	//D3DXVECTOR3 vEye	= pCam->GetEye();
	//D3DXVECTOR3 vAt		= pCam->GetAt();

	//_myvec vDir = vEye - vAt;
	//_float fCameraDis = pCam->Get_Distance();

 //  D3DXVECTOR3 vRayPos, vRayDir;

 //  vRayPos = vAt;

 //  vRayDir   = vEye - vAt;
 //  //D3DXVec3Normalize(&vRayDir,&vRayDir);
 //  vRayDir.y = 0.f;
 //  BOOL   bHit;
 //  float   fu, fv, fDis;
 //  DWORD  dwIndex;

 // D3DXIntersect(*(pMesh->Get_Mesh()),&vRayPos,&vRayDir,
 //    &bHit,&dwIndex,&fu,&fv,&fDis,NULL,NULL);

 // if(bHit)
 // {
 //    if(fDis < fCameraDis)
 //    {

 //          D3DXVECTOR3 vTemp = vAt - vEye;
 //          //D3DXVec3Normalize(&vTemp,&vTemp);

 //          fDis = fCameraDis - fDis ;

	//	   vEye += vTemp * (fDis);
	//	   pCam->Set_Eye(vEye);
	//	  
	//	//   __debugbreak();   

 //    }
 // }

  /*if(bHit)
  {
     if(fDis < 10.f)
        m_vEye += vRayDir * (fDis);
  }*/
}

