/*!
 * \file Stone.h
 * \date 2016/02/26 14:33
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

#ifndef Sphere_h__
#define Sphere_h__

#include "MapObjects.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
	class CMaterial;
};

class CCalculator;
class CSphere
	: public CMapObjects
{
private:
	explicit CSphere(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSphere(void);
public:
	virtual HRESULT Initialize(const TCHAR* pMeshKey, _vec3 vPos = _vec3(0,0,0));
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
private:
	virtual HRESULT AddComponent(const TCHAR* pMeshKey);
public:
	static CSphere* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey = NULL , _vec3 vPos = _vec3(0,0,0));
private:
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);
public:
	void SetPos(_vec3 vPos);
private:
	virtual void Free(void);
};


#endif // Sphere_h__