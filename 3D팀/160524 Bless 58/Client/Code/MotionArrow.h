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

#ifndef MotionArrow_h__
#define MotionArrow_h__

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
class CRanger;
class CMotionArrow
	: public CEquipment
{
private:
	explicit CMotionArrow(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMotionArrow(void);
public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void		 CreateSpherePos();
private:
	virtual HRESULT AddComponent(void);

public:
	static CMotionArrow* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
private:
	CRanger*					m_pRanger;

};


#endif // FireArrow_h__