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

#ifndef Stone_h__
#define Stone_h__

#include "LandObject.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
};

class CStone
	: public CLandObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStone(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CStone* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void Collision_AABB(void);
	void Collision_OBB(void);
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


#endif // Stone_h__