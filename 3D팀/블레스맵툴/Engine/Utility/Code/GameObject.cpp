#include "GameObject.h"
#include "Component.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{
	
}

const Engine::CComponent* Engine::CGameObject::GetComponent(const TCHAR* pComponentKey)
{
	MAPCOMPONENT::iterator iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTagFinder(pComponentKey));
	if(iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

_int Engine::CGameObject::Update(const _float& fTimeDelta)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	_int		iResult = 0;

	for(; iter != iter_end ; ++iter)
	{
		iter->second->Update();		
	}

	

	return iResult;
}


void Engine::CGameObject::Compute_ViewZ(const D3DXVECTOR3* pPos)
{
	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(D3DXVECTOR3));

	float		fDistance = D3DXVec3Length(&(vCamPos - *pPos));
	m_fViewZ = fDistance;
}

void Engine::CGameObject::Free(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	DWORD		dwRefCnt = 0;
	for( ; iter != iter_end; ++iter)
	{
		/*dwRefCnt = iter->second->Release();
		if(dwRefCnt == 0)*/
		Safe_Release(iter->second);
	}
	m_mapComponent.clear();

	Engine::Safe_Release(m_pDevice);

}

