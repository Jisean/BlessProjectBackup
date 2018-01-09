#include "OcclusionBox.h"
#include "CubeColor.h"
#include "ResourceMgr.h"
#include "Transform.h"

Engine::COcclusionBox::COcclusionBox(LPDIRECT3DDEVICE9 pDevice, const _uint iIndex)
: m_pDevice(pDevice)
, m_iIndex(iIndex)
, m_pOccBox(NULL)
, m_vMax(1.f, 1.f, 1.f)
, m_vMin(-1.f, -1.f, -1.f)
, m_bRender(true)
, m_bFrustumCulled(false)
{
	m_pDevice->AddRef();
	//D3DXMatrixIdentity(&m_matWorld);
}

Engine::COcclusionBox::~COcclusionBox(void)
{

}

HRESULT Engine::COcclusionBox::InitOcclusionBox(const D3DXMATRIX* pmatWorld)
{
	m_pInfo = CTransform::Create(D3DXVECTOR3(0.f, 0.f, -1.f));
	//m_pOccBox = dynamic_cast<CCubeColor*>(CResourceMgr::GetInstance()->CloneResource(RESOURCE_TOOL, L"Buffer_OccBox"));
	m_pOccBox = CCubeColor::Create(m_pDevice);
	NULL_CHECK_RETURN(pmatWorld, E_FAIL);
	D3DXMatrixIdentity(&m_pInfo->m_matWorld);
	m_pInfo->m_matWorld = *pmatWorld;
	m_pInfo->m_vPos.x = m_pInfo->m_matWorld._41;
	m_pInfo->m_vPos.y = m_pInfo->m_matWorld._42;
	m_pInfo->m_vPos.z = m_pInfo->m_matWorld._43;

	return S_OK;
}

void Engine::COcclusionBox::SetOccBox(CTransform* pInfo)
{
	m_pInfo = pInfo;
	m_pInfo->m_vDir = D3DXVECTOR3(0.f, 0.f, -1.f);
}

void Engine::COcclusionBox::Update(void)
{

}

void Engine::COcclusionBox::RenderOccBox(void)
{
	if(m_bRender)
		m_pOccBox->Change_BoxColor(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	else	
		m_pOccBox->Change_BoxColor(D3DXCOLOR(1.f, 0.f, 1.f, 1.f));

	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pOccBox->Render(&m_pInfo->m_matWorld);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

Engine::COcclusionBox* Engine::COcclusionBox::Create(LPDIRECT3DDEVICE9 pDevice, const _uint iIndex, const D3DXMATRIX* pmatWorld)
{
	COcclusionBox* pOccBox = new Engine::COcclusionBox(pDevice, iIndex);
	if(FAILED(pOccBox->InitOcclusionBox(pmatWorld)))
		Engine::Safe_Release(pOccBox);

	return pOccBox;
}

void Engine::COcclusionBox::Free(void)
{
	Safe_Release(m_pInfo);
	Safe_Release(m_pOccBox);
	Safe_Release(m_pDevice);
}

Engine::CTransform* Engine::COcclusionBox::GetOccBox(void)
{
	return m_pInfo;
}

Engine::CCubeColor* Engine::COcclusionBox::GetOccBoxVtx(void)
{
	return m_pOccBox;
}

void Engine::COcclusionBox::SetInOccBox(CGameObject* pObject)
{
	m_vecObject.push_back(pObject);
}

void Engine::COcclusionBox::GetMinMax(D3DXVECTOR3* pMin, D3DXVECTOR3* pMax)
{
	D3DXVec3TransformCoord(pMax, &m_vMax, &m_pInfo->m_matWorld);
	D3DXVec3TransformCoord(pMin, &m_vMin, &m_pInfo->m_matWorld);
}

void Engine::COcclusionBox::SetVtxPos(void)
{
	D3DXVECTOR3 vMin, vMax;
	D3DXVec3TransformCoord(&vMax, &m_vMax, &m_pInfo->m_matWorld);
	D3DXVec3TransformCoord(&vMin, &m_vMin, &m_pInfo->m_matWorld);
	
	m_vVtxPos[0] = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
	m_vVtxPos[1] = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
	m_vVtxPos[2] = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
	m_vVtxPos[3] = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);

	m_vVtxPos[4] = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
	m_vVtxPos[5] = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
	m_vVtxPos[6] = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
	m_vVtxPos[7] = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
}

void Engine::COcclusionBox::GetVtxPos(D3DXVECTOR3* pVtxPos)
{
	memcpy_s(pVtxPos, sizeof(D3DXVECTOR3) * 8, m_vVtxPos, sizeof(D3DXVECTOR3) * 8);
}

void Engine::COcclusionBox::GetOccVtxPos(WORD wAngle, D3DXVECTOR3* vOccPos1, D3DXVECTOR3* vOccPos2, D3DXVECTOR3* vOccPos3, D3DXVECTOR3* vOccPos4)
{
	switch(wAngle)
	{
	case 0:
		*vOccPos1 = m_vVtxPos[4];
		*vOccPos2 = m_vVtxPos[1];
		*vOccPos3 = m_vVtxPos[2];
		*vOccPos4 = m_vVtxPos[7];
		break;

	case 1:
		*vOccPos1 = m_vVtxPos[5];
		*vOccPos2 = m_vVtxPos[0];
		*vOccPos3 = m_vVtxPos[3];
		*vOccPos4 = m_vVtxPos[6];
		break;

	case 2:
		*vOccPos1 = m_vVtxPos[0];
		*vOccPos2 = m_vVtxPos[5];
		*vOccPos3 = m_vVtxPos[6];
		*vOccPos4 = m_vVtxPos[3];
		break;

	case 3:
		*vOccPos1 = m_vVtxPos[1];
		*vOccPos2 = m_vVtxPos[4];
		*vOccPos3 = m_vVtxPos[7];
		*vOccPos4 = m_vVtxPos[2];
		break;
	case 4:
		break;
	}
}

