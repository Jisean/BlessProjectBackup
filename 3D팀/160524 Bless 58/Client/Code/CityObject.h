/*!
 * \file CityObject.h
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

#ifndef CityObject_h__
#define CityObject_h__

#include "MapObjects.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
	class CMaterial;
};

class CCalculator;
class CCityObject
	: public CMapObjects
{
private:
	explicit CCityObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCityObject(void);

public:
	virtual HRESULT Initialize(const TCHAR* pMeshKey, int iIndex, int iInstanceIndex, bool bSelectCollision);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(const TCHAR* pMeshKey);

public:
	static CCityObject* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey = NULL, int iIndex = 0, int iInstanceIndex = 0, bool bSelectCollision = FALSE);

private:
	void Collision_AABB(void);
	void Collision_OBB(void);
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);

	// ------ Ãß°¡ºÐ-------//
private:
	int						m_iIndex;
	//const TCHAR*			m_cKeyName;
	bool					m_bHtableDelete;
	BOOL					m_bSelectCollision;

private:
	_float	m_fSpecPower;
	_float	m_fSpecRange;

private:
	virtual void Free(void);
};


#endif // CityObject_h__