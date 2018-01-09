/*!
 * \file ATObject.h
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

#ifndef ATObject_h__
#define ATObject_h__

#include "LandObject.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
};

class CATObject
	: public CLandObject
{
private:
	explicit CATObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CATObject(void);

public:
	virtual HRESULT Initialize(const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos);
	virtual int Update(const float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(const TCHAR* pMeshKey);

public:
	static CATObject* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos);

private:
	void Set_ContantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CStaticMesh*		m_pMesh;
	Engine::CShader*			m_pShader;
	D3DMATERIAL9				m_tMtrl;

private:
	bool		m_bCollision;

private:
	virtual void Free(void);
};


#endif // ATObject_h__