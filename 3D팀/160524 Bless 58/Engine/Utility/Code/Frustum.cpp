#include "Frustum.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrustum)

Engine::CFrustum::CFrustum(void)
{
	ZeroMemory(m_vPoint, sizeof(_vec3) * 8);
	ZeroMemory(m_Plane, sizeof(D3DXPLANE) * 6);

	Init_FrustumInProj();
}

Engine::CFrustum::~CFrustum(void)
{

}

_bool Engine::CFrustum::Decision_In(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition)
{
	Transform_ToView(pGraphicDev);
	Transform_ToWorld(pGraphicDev);

	// 평면을 구성하자.
	// +x, -x
	//D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);	
	//D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[0], &m_vPoint[3], &m_vPoint[7]);	

	//// +y, -y
	//D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);	
	//D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	//// +z, -z
	//D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);	
	//D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	// 평면 세개로 줄임
	// +x, -x
	//D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);	
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[0], &m_vPoint[3], &m_vPoint[7]);	

	// +y, -y
	//D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);	
	//D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// +z, -z
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);	
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	_float		fDistance = 0.f;

	for (_int i = 0; i < 3; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPosition);

		if(fDistance > 0.f)
			return false;
	}

	/*for (_int i = 0; i < 6; ++i)
	{
		_float fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPosition);

		if(fDistance > 0.f)
			return false;
	}*/
	return true;	
}

_bool Engine::CFrustum::Decision_In(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius)
{
	Transform_ToView(pGraphicDev);
	Transform_ToWorld(pGraphicDev);

	_float		fDistance = 0.f;

	// 평면 세개로 줄임
	// +x, -x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);	
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[0], &m_vPoint[3], &m_vPoint[7]);	

	// +y, -y
	//D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);	
	//D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// +z, -z
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);	
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	for (_int i = 0; i < 4; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPosition);

		if(fDistance > fRadius)
			return false;
	}

	return true;
}

_bool Engine::CFrustum::Decision_In_Object(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius)
{
	Transform_ToView(pGraphicDev);
	Transform_ToWorld(pGraphicDev);

	_float		fDistance = 0.f;

	// 평면을 구성하자.
	// +x, -x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);
	fDistance = D3DXPlaneDotCoord(&m_Plane[0], pPosition);
	if(fDistance > fRadius)
		return false;

	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[3], &m_vPoint[7], &m_vPoint[4]);
	fDistance = D3DXPlaneDotCoord(&m_Plane[1], pPosition);
	if(fDistance > fRadius)
		return false;

	// +y, -y
	//D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);	
	//D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// +z, -z
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);
	fDistance = D3DXPlaneDotCoord(&m_Plane[2], pPosition);
	if(fDistance > fRadius)
		return false;

	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);
	fDistance = D3DXPlaneDotCoord(&m_Plane[3], pPosition);
	if(fDistance > fRadius)
		return false;

	return true;
}

void Engine::CFrustum::Init_FrustumInProj(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.0f, -1.f, 0.0f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.0f, -1.f, 1.f);
}


void Engine::CFrustum::Transform_ToView(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Init_FrustumInProj();

	_matrix			matProj;
	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matProj, NULL, &matProj);

	for (_int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
	}
}

void Engine::CFrustum::Transform_ToWorld(LPDIRECT3DDEVICE9 pGraphicDev)
{	
	_matrix			matView;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}
}

void Engine::CFrustum::Free(void)
{
	delete this;
}

