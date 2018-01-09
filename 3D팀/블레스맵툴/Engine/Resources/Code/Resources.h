/*!
 * \file Resources.h
 * \date 2015/04/03 18:18
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
#ifndef Resources_h__
#define Resources_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources
	: public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pDevice);
	explicit CResources(const CResources& rhs);
	virtual ~CResources(void);
public:
	virtual CResources* CloneResource(void)PURE;
protected:
	LPDIRECT3DDEVICE9		m_pDevice;
public:
	virtual void Free(void);
};

END

#endif // Resources_h__