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

#ifndef Staff_h__
#define Staff_h__

#include "Equipment.h"
#include "Include.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CShader;
	class CMaterial;
};
class CMagician;
class CStaff
	: public CEquipment
{
private:
	explicit CStaff(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStaff(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void		 CreateSpherePos();
private:
	virtual HRESULT AddComponent(void);

public:
	static CStaff* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	CMagician*					m_pMagician;
};


#endif // Staff_h__