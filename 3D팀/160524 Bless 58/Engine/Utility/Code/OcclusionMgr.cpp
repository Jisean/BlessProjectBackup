#include "OcclusionMgr.h"
#include "OcclusionBox.h"
#include "Component.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Management.h"
#include "Scene.h"
#include "Camera.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(Engine::COcclusionMgr)

Engine::COcclusionMgr::COcclusionMgr(void)
: m_pDevice(NULL)
{

}

Engine::COcclusionMgr::~COcclusionMgr(void)
{

}

HRESULT Engine::COcclusionMgr::InitOcclusion(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	return S_OK;
}

void Engine::COcclusionMgr::Set_OccBox(const _uint iIndex, CTransform* pInfo)
{
	m_vecOccBox[iIndex]->SetOccBox(pInfo);
}

Engine::CTransform* Engine::COcclusionMgr::Get_OccBox(const _uint iIndex)
{
	return m_vecOccBox[iIndex]->GetOccBox();
}

Engine::CCubeColor* Engine::COcclusionMgr::Get_OccBoxVtx(const _uint iIndex)
{
	return m_vecOccBox[iIndex]->GetOccBoxVtx();
}

HRESULT Engine::COcclusionMgr::Add_OccBox(const D3DXMATRIX* pmatWorld)
{
	COcclusionBox* pOccBox = COcclusionBox::Create(m_pDevice, m_vecOccBox.size(), pmatWorld);
	NULL_CHECK_RETURN_MSG(pOccBox, E_FAIL, L"OccBox Make Failed");
	m_vecOccBox.push_back(pOccBox);
	OCCLUSION* pOcclusion = new OCCLUSION;
	m_vecOccluder.push_back(pOcclusion);
	
	return S_OK;
}

void Engine::COcclusionMgr::Render_OccBox(void)
{
	OCCBOXVEC::iterator	iter = m_vecOccBox.begin();
	OCCBOXVEC::iterator	iter_end = m_vecOccBox.end();

	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->RenderOccBox();
	}
}

void Engine::COcclusionMgr::Free(void)
{
	for_each(m_vecOccBox.begin(), m_vecOccBox.end(), CDeleteObj());
	m_vecOccBox.clear();
	for_each(m_vecOccluder.begin(), m_vecOccluder.end(), CDeleteVar());
	m_vecOccluder.clear();
	Safe_Release(m_pDevice);
}

void Engine::COcclusionMgr::Delete_OccBox(const _uint iIndex)
{
	OCCBOXVEC::iterator	iter = m_vecOccBox.begin();
	OCCBOXVEC::iterator	iter_end = m_vecOccBox.end();
	for(iter; iter != iter_end; ++iter)
	{
		if((*iter) == m_vecOccBox[iIndex])
		{
			Safe_Release(m_vecOccBox[iIndex]);
			m_vecOccBox.erase(iter);
			break;
		}
	}
}

bool Engine::COcclusionMgr::SetInOccBox(CGameObject* pObject)
{
	OCCBOXVEC::iterator	iter = m_vecOccBox.begin();
	OCCBOXVEC::iterator	iter_end = m_vecOccBox.end();

	CComponent* pObjCom = const_cast<CComponent*>(pObject->GetComponent(L"Mesh"));
	CMesh* pObjMesh = dynamic_cast<CMesh*>(pObjCom);
	D3DXVECTOR3 vObjMin, vObjMax, vObjCenter;
	pObjMesh->GetMinMax(&vObjMin, &vObjMax);
	vObjCenter = (vObjMin + vObjMax) * 0.5f;
	CComponent* pObjInfoCom = const_cast<CComponent*>(pObject->GetComponent(L"Transform"));
	CTransform* pObjInfo = dynamic_cast<CTransform*>(pObjInfoCom);
	D3DXVec3TransformCoord(&vObjCenter, &vObjCenter, &pObjInfo->m_matWorld);

	D3DXPLANE Plane[6];
	D3DXVECTOR3* pVtxPos = new D3DXVECTOR3[8];
	
	bool bIn = false;
	for(iter; iter != iter_end; ++iter)
	{
		/*D3DXVECTOR3 vOccMin, vOccMax;
		bool bIn = false;
		(*iter)->GetMinMax(&vOccMin, &vOccMax);

		bIn = vObjCenter.x > vOccMin.x ? true : false;
		if(bIn)
			bIn = vObjCenter.y > vOccMin.y ? true : false;
		if(bIn)
			bIn = vObjCenter.z > vOccMin.z ? true : false;
		if(bIn)
			bIn = vObjCenter.x <= vOccMax.x ? true : false;
		if(bIn)
			bIn = vObjCenter.y <= vOccMax.y ? true : false;
		if(bIn)
			bIn = vObjCenter.z <= vOccMax.z ? true : false;
		
		if(bIn)
		{
			(*iter)->SetInOccBox(pObject);
			break;
		}*/
		(*iter)->GetVtxPos(pVtxPos);

		// +x, -x
		D3DXPlaneFromPoints(&Plane[0], &pVtxPos[1], &pVtxPos[5], &pVtxPos[6]);	
		D3DXPlaneFromPoints(&Plane[1], &pVtxPos[3], &pVtxPos[7], &pVtxPos[4]);	

		// +y, -y
		D3DXPlaneFromPoints(&Plane[2], &pVtxPos[4], &pVtxPos[5], &pVtxPos[1]);	
		D3DXPlaneFromPoints(&Plane[3], &pVtxPos[3], &pVtxPos[2], &pVtxPos[6]);

		// +z, -z
		D3DXPlaneFromPoints(&Plane[4], &pVtxPos[7], &pVtxPos[6], &pVtxPos[5]);	
		D3DXPlaneFromPoints(&Plane[5], &pVtxPos[0], &pVtxPos[1], &pVtxPos[2]);

		
		for(_uint i = 0; i < 6; ++i)
		{
			if(D3DXPlaneDotCoord(&Plane[i], &vObjCenter) > 0.f)
			{
				bIn = false;
				break;
			}
			bIn = true;
		}
		if(bIn)
			(*iter)->SetInOccBox(pObject);
	}
	Safe_Delete_Array(pVtxPos);

	return bIn;
}

void Engine::COcclusionMgr::Set_VtxPos(const _uint iIndex)
{
	m_vecOccBox[iIndex]->SetVtxPos();
}

void Engine::COcclusionMgr::Update(void)
{
	SetupOcclusion();
	OcclusionCulling();
}

void Engine::COcclusionMgr::SetupOcclusion()
{
	OCCBOXVEC::iterator iterbox		= m_vecOccBox.begin();
	OCCBOXVEC::iterator iterbox_end = m_vecOccBox.end();
	

	D3DXMATRIX matView, matViewInv;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	D3DXVECTOR3 vCamEye(matViewInv._41, matViewInv._42, matViewInv._43);

	D3DXVECTOR3 vMin, vMax, vCenter;
	for(iterbox; iterbox != iterbox_end; ++iterbox)
	{
		OCCVEC::iterator	iterocc		= m_vecOccluder.begin();

		(*iterbox)->GetMinMax(&vMin, &vMax);
		vCenter = (vMin + vMax) * 0.5f;
		CTransform* pInfo = (*iterbox)->GetOccBox();

		_myvec vOccDir, vCamDir;
		vCamDir = vCenter - vCamEye;
		vCamDir.y = 0.f;
		vOccDir = -pInfo->m_vDir;
		vCamDir.Normalize();
		vOccDir.Normalize();

		float fAngleWithCam = D3DXToDegree(acosf(vCamDir.Dot(&vOccDir)));
		if(matViewInv._41 < vCenter.x)
			fAngleWithCam = -fAngleWithCam;

		OCCANGLE eOccAngle = OCCANGLE_END;
		if(fAngleWithCam < 0.f && fAngleWithCam >= -90.f)
			eOccAngle = OCCANGLE_1;
		else if(fAngleWithCam < -90.f && fAngleWithCam >= -180.f)
			eOccAngle = OCCANGLE_2;
		else if(fAngleWithCam < 90.f && fAngleWithCam >= 0.f)
			eOccAngle = OCCANGLE_3;
		else if(fAngleWithCam < 180.f && fAngleWithCam >= 90.f)
			eOccAngle = OCCANGLE_4;

		(*iterbox)->GetOccVtxPos(eOccAngle, &(*iterocc)->vPlane[0], &(*iterocc)->vPlane[1], &(*iterocc)->vPlane[2], &(*iterocc)->vPlane[3]);

		D3DXVECTOR3 vPlane[4];
		D3DXVECTOR3 vCamera(0.f, 0.f, 0.f);

		D3DXVec3TransformCoord(&vPlane[0], &(*iterocc)->vPlane[0], &matView);
		D3DXVec3TransformCoord(&vPlane[1], &(*iterocc)->vPlane[1], &matView);
		D3DXVec3TransformCoord(&vPlane[2], &(*iterocc)->vPlane[2], &matView);
		D3DXVec3TransformCoord(&vPlane[3], &(*iterocc)->vPlane[3], &matView);

		(*iterocc)->MinZ = min(vPlane[0].z, min(vPlane[1].z, min(vPlane[2].z, vPlane[3].z)));

		D3DXPlaneFromPoints(&(*iterocc)->FrontPlane, &vPlane[0], &vPlane[1], &vPlane[2]);
		D3DXPlaneFromPoints(&(*iterocc)->FirstPlane, &vCamera, &vPlane[0], &vPlane[1]);
		D3DXPlaneFromPoints(&(*iterocc)->SecondPlane, &vCamera, &vPlane[1], &vPlane[2]);
		D3DXPlaneFromPoints(&(*iterocc)->ThirdPlane, &vCamera, &vPlane[2], &vPlane[3]);
		D3DXPlaneFromPoints(&(*iterocc)->FourthPlane, &vCamera, &vPlane[3], &vPlane[0]);

		//OCCBOXVEC::iterator iterboxCull		= m_vecOccBox.begin();
		//OCCBOXVEC::iterator iterboxCull_end = m_vecOccBox.end();

		//for(iterboxCull; iterboxCull != iterboxCull_end; ++iterboxCull)
		//{
		//	if(iterboxCull == iterbox)
		//	{
		//		++iterocc;
		//		continue;
		//	}
		//	
		//	D3DXVECTOR3 vMinChild, vMaxChild, vCenterChild;
		//	(*iterboxCull)->GetMinMax(&vMinChild, &vMaxChild);
		//	vCenterChild = (vMinChild + vMaxChild) * 0.5f;

		//	bool bInOcc = true;
		//	
		//	if(!InOcclusion((*iterocc), vMinChild, 0.f))
		//	{
		//		bInOcc = false;
		//		(*iterboxCull)->SetRender(true);
		//	}
		//	else if(!InOcclusion((*iterocc), vMaxChild, 0.f))
		//	{
		//		bInOcc = false;
		//		(*iterboxCull)->SetRender(true);
		//	}
		//	else if(!InOcclusion((*iterocc), vCenterChild, 0.f))
		//	{
		//		bInOcc = false;
		//		(*iterboxCull)->SetRender(true);
		//	}
		//	else
		//		(*iterboxCull)->SetRender(false);
		//
		//}
		++iterocc;
	}
}

bool Engine::COcclusionMgr::InOcclusion(OCCLUSION* pOcc, D3DXVECTOR3 vPosition, float fRadius)
{
	float	MinZ;
	MinZ = vPosition.z - fRadius;
	if( pOcc->MinZ < MinZ )
		return false;

	if(D3DXPlaneDotCoord(&pOcc->FrontPlane, &vPosition) > fRadius)
		return false;

	if(D3DXPlaneDotCoord(&pOcc->FirstPlane, &vPosition) > fRadius)
		return false;

	if(D3DXPlaneDotCoord(&pOcc->SecondPlane, &vPosition) > fRadius)
		return false;

	if(D3DXPlaneDotCoord(&pOcc->ThirdPlane, &vPosition) > fRadius)
		return false;

	if(D3DXPlaneDotCoord(&pOcc->FourthPlane, &vPosition) > fRadius)
		return false;

	return true;
}

void Engine::COcclusionMgr::OcclusionCulling(void)
{
	OCCBOXVEC::iterator iterbox		= m_vecOccBox.begin();
	OCCBOXVEC::iterator iterbox_end = m_vecOccBox.end();
	OCCVEC::iterator	iterocc		= m_vecOccluder.begin();

	for(iterbox; iterbox != iterbox_end; ++iterbox)
	{
		D3DXVECTOR3 vMin, vMax, vCenter;
		(*iterbox)->GetMinMax(&vMin, &vMax);
		vCenter = (vMin + vMax) * 0.5f;
		
		bool bRender = false;
		if(!InOcclusion((*iterocc), vMin, 3.f))
		{
			if(!InOcclusion((*iterocc), vMax, 3.f))
			{
				bRender = InOcclusion((*iterocc), vCenter, 3.f);
			}
			else
				bRender = true;
		}
		else
			bRender = true;

		(*iterbox)->SetRender(bRender);

		++iterocc;
	}
}

int Engine::COcclusionMgr::TestInOccBox(D3DXVECTOR3 vPos)
{
	OCCBOXVEC::iterator iterbox		= m_vecOccBox.begin();
	OCCBOXVEC::iterator iterbox_end = m_vecOccBox.end();
	int iIndex = 0;
	for(iterbox; iterbox != iterbox_end; ++iterbox)
	{
		D3DXVECTOR3 vMin, vMax;
		(*iterbox)->GetMinMax(&vMin, &vMax);

		bool bIn = false;
		bIn = vMin.x < vPos.x ? (vMin.z < vPos.z ? (vMax.x >= vPos.x ? (vMax.z >= vPos.z ? true : false) : false) : false) : false;

		if(bIn)
			break;

		++iIndex;
	}
	if(iIndex == m_vecOccBox.size())
		return -1;

	return iIndex;
}
