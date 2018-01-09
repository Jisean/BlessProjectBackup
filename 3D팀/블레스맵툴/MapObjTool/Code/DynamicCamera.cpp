#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_fCamSpeed(0.f)
, m_bCameraFix(false)
, m_bSpeedUp(false)
, m_bSpeedDown(false)
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

	if(GetAsyncKeyState('A'))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fCamSpeed * fTime;
		m_vAt -= vRight * m_fCamSpeed * fTime;
	}

	if(GetAsyncKeyState('D'))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fCamSpeed * fTime;
		m_vAt += vRight * m_fCamSpeed * fTime;
	}

	if(GetAsyncKeyState('W'))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fCamSpeed * fTime;
		m_vAt += vLook * m_fCamSpeed * fTime;
	}

	if(GetAsyncKeyState('S'))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fCamSpeed * fTime;
		m_vAt -= vLook * m_fCamSpeed * fTime;
	}

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

int CDynamicCamera::Update(const float& fTimeDelta)
{
	if(GetAsyncKeyState('P'))
	{
		if(m_bCameraFix == true)
			m_bCameraFix = false;
	}
	if(GetAsyncKeyState('O'))
	{
		if(m_bCameraFix == false)
			m_bCameraFix = true;
	}

	if(GetAsyncKeyState('9'))
	{
		if(m_bSpeedUp == true || m_bSpeedDown == true)
		{
			m_bSpeedUp = false;
			m_bSpeedDown = false;
			m_fCamSpeed = 10.f;
		}
	}
	if(GetAsyncKeyState('0'))
	{
		if(m_bSpeedUp == false)
		{
			m_bSpeedUp = true;
			m_bSpeedDown = false;
			m_fCamSpeed = 30.f;
		}
	}
	if(GetAsyncKeyState('8'))
	{
		if(m_bSpeedDown == false)
		{
			m_bSpeedDown = true;
			m_bSpeedUp = false;
			m_fCamSpeed = 1.f;
		}
	}

	if(m_bCameraFix == false)
	{
		KeyCheck();
		FixMouse();
	}


	//cout << "cam at :" << m_vAt.x << "," << m_vAt.y << "," << m_vAt.z << endl;
	//cout << "cam eye :" << m_vEye.x << "," << m_vEye.y << "," << m_vEye.z << endl;

	return Engine::CCamera::Update(fTimeDelta);
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pDevice);
	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Delete(pCamera);

	return pCamera;
}

void CDynamicCamera::FixMouse(void)
{
	POINT		ptMouse = {WINCX >> 1, WINCY >> 1};
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

