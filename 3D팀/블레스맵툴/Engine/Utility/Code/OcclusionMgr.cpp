#include "OcclusionMgr.h"
#include "OcclusionBox.h"
#include "Component.h"
#include "GameObject.h"
#include "Mesh.h"

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

void Engine::COcclusionMgr::SetInOccBox(CGameObject* pObject)
{
	OCCBOXVEC::iterator	iter = m_vecOccBox.begin();
	OCCBOXVEC::iterator	iter_end = m_vecOccBox.end();

	CComponent* pObjCom = const_cast<CComponent*>(pObject->GetComponent(L"Mesh"));
	CMesh* pObjMesh = dynamic_cast<CMesh*>(pObjCom);
	D3DXVECTOR3 vObjMin, vObjMax, vObjCenter;
	pObjMesh->GetMinMax(&vObjMin, &vObjMax);
	vObjCenter = (vObjMin + vObjMax) * 0.5f;

	for(iter; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vOccMin, vOccMax;
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
		}
	}
}

void Engine::COcclusionMgr::Update(void)
{

}

void Engine::COcclusionMgr::Get_MinMax(const _uint iIndex, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax, D3DXVECTOR3* pCenter /*= NULL*/)
{
	m_vecOccBox[iIndex]->GetMinMax(pMin, pMax, pCenter);
}
