/*!
 * \file Transform.h
 * \date 2015/04/05 8:37
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
#ifndef ComSphere_h__
#define ComSphere_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CComSphere
	: public CComponent
{
private:
	explicit CComSphere(void);
public:
	virtual ~CComSphere(void);

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXMESH			m_pSphereMesh;
	D3DXMATRIX			m_matCombinedMatrix;
	D3DXVECTOR3			m_vPos;
	float				m_fRadius;

public:
	virtual void Update(void);
	HRESULT	Initialize( const D3DXMATRIX& matCombinedMatrix, float fRadius, const D3DXVECTOR3& vPos );
	void	Render(const D3DXMATRIX* matWorld);

public:
	static CComSphere* Create( const D3DXMATRIX& matCombinedMatrix, float fRadius, const D3DXVECTOR3& vPos = D3DXVECTOR3(0.f, 0.f, 0.f) );
	virtual void Free(void);
};

END

#endif // ComSphere_h__