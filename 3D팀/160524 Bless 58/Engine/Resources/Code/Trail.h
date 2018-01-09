/*!
 * \file Trail.h
 * \date 2015/04/04 23:00
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
#ifndef Trail_h__
#define Trail_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrail
	: public CVIBuffer
{
	#define			LINECOUNT	20
	#define			POINTCOUNT	40
	#define			LINESIZE	LINECOUNT * POINTCOUNT
private:
	explicit CTrail(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTrail(void);	

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer();

public:
	static CTrail* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		void Free(void);

	
public:
	//D3DXVECTOR3	m_vRomLine[LINESIZE * 2];
	D3DXVECTOR3	m_vPoint[LINECOUNT + 1];
	D3DXVECTOR3	m_vSubPoint[LINECOUNT + 1];	
};

END

#endif // Trail_h__



