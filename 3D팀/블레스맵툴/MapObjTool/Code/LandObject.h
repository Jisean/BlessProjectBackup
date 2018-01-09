/*!
 * \file LandObject.h
 * \date 2016/02/26 14:34
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

#ifndef LandObject_h__
#define LandObject_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CTransform;
}

class CLandObject
	: public Engine::CGameObject
{
protected:
	explicit CLandObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLandObject(void);

public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual int Update(const float& fTimeDelta);
	virtual void Render(void) {};
public:
	void SetCollision(bool bCollision)
	{
		m_bCollision = bCollision;
	}
	void SetOnMouse(bool bOnMouse)
	{
		m_bOnMouse = bOnMouse;
	}
	void SetPos(D3DXVECTOR3 vPos);
	void SetScale(D3DXVECTOR3 vScale);
	void SetAngle(D3DXVECTOR3 vAngle);
	
	TCHAR* GetKey(void)
	{
		return m_tcKey;
	}

protected:
	Engine::CTransform*			m_pInfo;
	bool						m_bCollision;
	bool						m_bOnMouse;
public:
	TCHAR						m_tcKey[256];

};

#endif // LandObject_h__