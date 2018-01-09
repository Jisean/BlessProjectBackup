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

#ifndef ArrowBasket_h__
#define ArrowBasket_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CComSphere;
	class CShader;
	class CMaterial;
};
class CPaladin;
class CArrowBasket
	: public Engine::CGameObject
{
private:
	explicit CArrowBasket(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CArrowBasket(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void		 CreateSpherePos();
private:
	virtual HRESULT AddComponent(void);

public:
	static CArrowBasket* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
private:
	CPaladin*					m_pPaladin;
private:
	Engine::CStaticMesh*		m_pMesh;
	Engine::CTransform*			m_pInfo;
	Engine::CComSphere*			m_pComSphere;
	Engine::CShader*			m_pShader;
	Engine::CMaterial*			m_pMtrlCom;
	_myvec						m_vSwordPos;
	_vec3						m_vPoint[2];
private:
	const D3DXMATRIX*		m_pmatPlayerInfo;
	const D3DXMATRIX*		m_pmatWeaponRef;
	D3DXMATRIX*				m_matSphere;
};


#endif // ArrowBasket_h__