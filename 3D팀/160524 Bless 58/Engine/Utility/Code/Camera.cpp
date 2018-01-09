#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_vEye(0.f, 0.f, 0.f)
, m_vAt(0.f, 0.f, 0.f)
, m_vUp(0.f, 1.f, 0.f)
, m_fFovY(0.f)
, m_fAspect(0.f)
, m_fNear(0.f)
, m_fFar(0.f)
{
	m_htComponent.Ready_Table(19);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

Engine::CCamera::~CCamera(void)
{

}

_int Engine::CCamera::Update(const _float& fTimeDelta)
{
	Invalidate_ViewMatrix();

	return 0;
}

void Engine::CCamera::Invalidate_ViewMatrix(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void Engine::CCamera::Invalidate_ProjMatrix(void)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

