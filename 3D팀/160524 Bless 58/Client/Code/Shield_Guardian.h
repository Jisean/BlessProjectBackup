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

#ifndef Shield_Guardian_h__
#define Shield_Guardian_h__

#include "Equipment.h"
#include "Include.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CShader;
	class CMaterial;
};
class CGuardian;
class CShield_Guardian
	: public CEquipment
{
private:
	explicit CShield_Guardian(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CShield_Guardian(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void		 CreateSpherePos();
private:
	virtual HRESULT AddComponent(void);

public:
	static CShield_Guardian* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
private:
	_vec3						m_vLook;
	CGuardian*					m_pGuardian;
};


#endif // Shield_Guardian_h__