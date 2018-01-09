#include "stdafx.h"
#include "DynamicCamera.h"

#include "Include.h"
#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_fCamSpeed(0.f)
, m_bClick(false)
, m_bMouseFix(true)
{

}

CDynamicCamera::~CDynamicCamera(void)
{
	
}

HRESULT CDynamicCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed = 10.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();
	return S_OK;
}

void CDynamicCamera::KeyCheck(void)
{
	float		fTime = Engine::Get_Time();

	if(Engine::GetDIKeyState(DIK_Q) & 0x80)
	{
		if(m_bClick == true)
			return;

		if(m_bMouseFix == true)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;

		m_bClick = true;
	}
	else
		m_bClick = false;

	if(Engine::GetDIKeyState(DIK_A) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fCamSpeed * fTime;
		m_vAt -= vRight * m_fCamSpeed * fTime;
	}

	if(Engine::GetDIKeyState(DIK_D) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fCamSpeed * fTime;
		m_vAt += vRight * m_fCamSpeed * fTime;
	}

	if(Engine::GetDIKeyState(DIK_W) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fCamSpeed * fTime;
		m_vAt += vLook * m_fCamSpeed * fTime;
	}

	if(Engine::GetDIKeyState(DIK_S) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fCamSpeed * fTime;
		m_vAt -= vLook * m_fCamSpeed * fTime;
	}
}

_int CDynamicCamera::Update(const _float& fTimeDelta)
{
	KeyCheck();
	if(m_bMouseFix)
	{
		FixMouse();
		MouseMove();
	}
	return Engine::CCamera::Update(fTimeDelta);
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pDevice);
	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CDynamicCamera::FixMouse(void)
{
	POINT		ptMouse = {WINCX >> 1, WINCY >> 1};
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::MouseMove(void)
{
	int		iDistance = 0;
	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}