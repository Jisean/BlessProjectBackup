#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Include.h"
#include "Base.h"


BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	explicit CFrustum(void);
	~CFrustum(void);
public:
	_bool Decision_In(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius);
	_bool Decision_In(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition);
	_bool Decision_In_Object(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius);
private:
	_vec3				m_vPoint[8];
	D3DXPLANE			m_Plane[6];
private:
	void Init_FrustumInProj(void);
	void Transform_ToView(LPDIRECT3DDEVICE9 pGraphicDev);
	void Transform_ToWorld(LPDIRECT3DDEVICE9 pGraphicDev);
	
public:
	virtual void Free(void);

};

END

#endif // Frustum_h__
