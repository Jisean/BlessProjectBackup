#ifndef Bullet_h__
#define Bullet_h__

#include "LandObject.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
	class CMaterial;
	class CTransform;
}

class CBullet : public CLandObject
{
protected:
	Engine::CStaticMesh* m_pMesh;
	Engine::CShader*	 m_pShader;
	Engine::CMaterial*	 m_pMtrlCom;
	_vec3				 m_vLook;
	WORD				 m_wNumber;
	_float				 m_fTimeDelta;
public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta){return 0;}
	virtual void Render(void) {};
	virtual void Free(void);
public:
	void Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix = NULL);
	void Set_ConstantTable(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix, const D3DLIGHT9* pLightInfo, const D3DMATERIAL9* pMtrlInfo);
public:
	WORD	Get_wNumber();
protected:
	_bool Clear_Render(void);
public:
	Engine::CTransform*	m_pUnitInfo;
	const _matrix*		m_pTarget_Matrix;
	float				m_fSpeed;
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CBullet();

};
#endif // Bullet_h__
