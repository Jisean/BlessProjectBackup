/*!
 * \file BaseTime.h
 * \date 2016/05/09 17:02
 *
 * \author ±«ø¿»∆
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef BaseTime_h__
#define BaseTime_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CBaseTime : public CBase
{
private:
	explicit CBaseTime(void);
	virtual ~CBaseTime(void);
public:
	float Get_TimeDelta(void) {	return m_fTimeDelta; }
	void Set_TimeDelta(void);
	HRESULT Ready_Timer(void);

private:
	LARGE_INTEGER					m_FrameTime;
	LARGE_INTEGER					m_LastTime;
	LARGE_INTEGER					m_FixTime;
	LARGE_INTEGER					m_CpuTick;
private:
	float							m_fTimeDelta;
public:
	static CBaseTime* Create(void);
	virtual void Free(void);
};

END

#endif // BaseTime_h__
