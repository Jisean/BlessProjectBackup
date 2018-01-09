/*!
 * \file GuiscardObject.h
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

#ifndef GuiscardObject_h__
#define GuiscardObject_h__

#include "LandObject.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
};

class CGuiscardObject
	: public CLandObject
{
private:
	explicit CGuiscardObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGuiscardObject(void);

public:
	virtual HRESULT Initialize(const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos);
	virtual int Update(const float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(const TCHAR* pMeshKey);

public:
	static CGuiscardObject* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey, D3DXVECTOR3 vMousePos);

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


#endif // GuiscardObject_h__