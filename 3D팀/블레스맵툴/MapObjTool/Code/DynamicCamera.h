/*!
 * \file DynamicCamera.h
 * \date 2016/02/24 13:32
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CDynamicCamera
	: public Engine::CCamera
{
private:
	CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDynamicCamera(void);

public:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

public:
	virtual int Update(const float& fTimeDelta);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	void KeyCheck(void);
	void FixMouse(void);

public:
	D3DXVECTOR3* GetEye(void)
	{
		return &m_vEye;
	}

private:
	float		m_fCamSpeed;
	bool		m_bCameraFix;
	bool		m_bSpeedUp;
	bool		m_bSpeedDown;
};

#endif // DynamicCamera_h__