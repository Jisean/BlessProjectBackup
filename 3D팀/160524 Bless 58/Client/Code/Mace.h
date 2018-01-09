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

#ifndef Mace_h__
#define Mace_h__

#include "Equipment.h"
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
class CMace
	: public CEquipment
{
private:
	explicit CMace(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMace(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void		 CreateSpherePos();
private:
	virtual HRESULT AddComponent(void);

public:
	static CMace* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
private:
	CPaladin*					m_pPaladin;

};


#endif // Mace_h__