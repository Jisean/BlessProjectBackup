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
#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform
	: public CComponent
{
private:
	explicit CTransform( const D3DXVECTOR3& vLook );
	virtual ~CTransform(void);

public:
	float				m_fAngle[ANGLE_END];
	D3DXVECTOR3			m_vScale;
	D3DXVECTOR3			m_vPos;
	D3DXVECTOR3			m_vDir;
	D3DXMATRIX			m_matWorld;

public:
	virtual void Update(void);

public:
	static CTransform* Create(const D3DXVECTOR3& vLook);
	virtual void Free(void);
};

END

#endif // Transform_h__