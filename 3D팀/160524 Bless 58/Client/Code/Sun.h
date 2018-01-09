/*!
 * \file Sun.cpp
 * \date 2016/06/01 13:13
 *
 * \author gpdms
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Sun_h__
#define Sun_h__

#include "MapObjects.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
	class CMaterial;
};

class CSun
	: public CMapObjects
{
private:
	explicit CSun(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSun(void);

public:
	virtual HRESULT Initialize(const TCHAR* pMeshKey);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(const TCHAR* pMeshKey);

public:
	static CSun* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey = NULL);

private:
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);

private:
	virtual void Free(void);
};

#endif // Sun_h__