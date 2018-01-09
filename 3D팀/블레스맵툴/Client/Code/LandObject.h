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
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};

protected:
	Engine::CTransform*			m_pInfo;
};

#endif // LandObject_h__