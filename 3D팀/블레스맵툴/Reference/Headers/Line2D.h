/*!
 * \file Line2D.h
 * \date 2016/03/09 13:17
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

#ifndef Line2D_h__
#define Line2D_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLine2D : public CBase
{
public:
	enum DIR {DIR_LEFT, DIR_RIGHT};

private:
	CLine2D(void);
	~CLine2D(void);

public:
	HRESULT Init_Line(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);
	DIR Check_Dir(const D3DXVECTOR2* pMoveEndPoint);

public:
	D3DXVECTOR2		m_vStartPoint;
	D3DXVECTOR2		m_vEndPoint;
	D3DXVECTOR2		m_vNormal;

public:
	static CLine2D* Create(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);
	virtual void Free(void);
};

END

#endif // Line2D_h__