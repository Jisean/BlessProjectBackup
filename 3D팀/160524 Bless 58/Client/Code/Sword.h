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

#ifndef Sword_h__
#define Sword_h__

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
class CSword
	: public CEquipment
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSword(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void		 CreateSpherePos();
private:
	virtual HRESULT AddComponent(void);

public:
	static CSword* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	CGuardian*					m_pGuardian;
};


#endif // Sword_h__