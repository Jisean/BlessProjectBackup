#include "GameObject.h"
#include "Component.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_bDead(false)
, m_bStart(true)
, m_eShadowRender(SHADOW_END)
, m_eTrailState(STATE_READY)
, m_iNumber(0)
{
	m_pDevice->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{
	
}

const Engine::CComponent* Engine::CGameObject::GetComponent(const TCHAR* pComponentKey)
{
	Engine::CComponent* pComponent = (Engine::CComponent*)(m_htComponent.Search_TableData(pComponentKey));
	NULL_CHECK_RETURN(pComponent, NULL);

	return pComponent;
}

_int Engine::CGameObject::Update(const _float& fTimeDelta)
{
	size_t iTableSize =	m_htComponent.Get_TableSize();

	_int		iResult = 0;

	for(size_t i = 0; i < iTableSize; ++i)
	{
		Engine::CComponent* pComponent = (Engine::CComponent*)m_htComponent.Search_TableData(i);
		if(NULL != pComponent)
			pComponent->Update();
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
	size_t iTableSize =	m_htComponent.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		Engine::CComponent*		pComponent = (Engine::CComponent*)m_htComponent.Erase_Slot(i);
		if(NULL != pComponent)
			Safe_Release(pComponent);
	}
	m_htComponent.Erase_Table();

	Engine::Safe_Release(m_pDevice);
}

void Engine::CGameObject::SetDead(bool bDead/* = true*/)
{
	m_bDead = bDead;
}

bool Engine::CGameObject::GetDead(void)
{
	return m_bDead;
}

void Engine::CGameObject::SetTrailState(bool bCheck)
{
	if(bCheck == true)
		m_eTrailState = STATE_START;
	else
		m_eTrailState = STATE_READY;
}

int Engine::CGameObject::GetTrailState()
{
	return m_eTrailState;
}

void Engine::CGameObject::SetNumber(int iNumber)
{
	m_iNumber = iNumber;
}

int Engine::CGameObject::GetNumber()
{
	return m_iNumber;
}

