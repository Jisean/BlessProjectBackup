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
#include "Include.h"

class CDynamicCamera
	: public Engine::CCamera
{
private:
	CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CDynamicCamera(void);

public:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

public:
	virtual _int Update(const _float& fTimeDelta);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	void KeyCheck(void);
	void FixMouse(void);
	void MouseMove(void);

private:
	float		m_fCamSpeed;
	bool		m_bClick;
	bool		m_bMouseFix;
};

#endif // DynamicCamera_h__