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

#ifndef DungeonObject_h__
#define DungeonObject_h__

#include "MapObjects.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
	class CMaterial;
};

class CCalculator;
class CDungeonObject
	: public CMapObjects
{
private:
	explicit CDungeonObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CDungeonObject(void);

public:
	virtual HRESULT Initialize(const TCHAR* pMeshKey, int iIndex, int iInstanceIndex);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(const TCHAR* pMeshKey);
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);

public:
	static CDungeonObject* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey = NULL, int iIndex = 0, int iInstanceIndex = 0);
private:
	virtual void Free(void);
};


#endif // DungeonObject_h__