/*!
 * \file Bow.h
 * \date 2016/03/14 14:33
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

#ifndef Bow_h__
#define Bow_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
};

class CBow
	: public Engine::CGameObject
{
private:
	explicit CBow(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBow(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CBow* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	Engine::CStaticMesh*		m_pMesh;
	Engine::CTransform*			m_pInfo;

private:
	const D3DXMATRIX*		m_pmatPlayerInfo;
	const D3DXMATRIX*		m_pmatWeaponRef;
};


#endif // Bow_h__