#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CBaseTime;
class ENGINE_DLL CTimeMgr : public CBase
{
	DECLARE_SINGLETON(CTimeMgr)
private:
	explicit CTimeMgr(void);
	virtual ~CTimeMgr(void);
public:
	float Get_TimeDelta(const TCHAR* pTimerTag);
	void Set_TimeDelta(const TCHAR* pTimerTag);
public:
	HRESULT Ready_Timer(const TCHAR* pTimerTag);
private:
	typedef HTable					HTTIMER;
	HTTIMER							m_htTimes;
private:
	CBaseTime* Find_Timer(const TCHAR* pTimerTag);
public:
	virtual void Free(void);
};

END

#endif // TimeMgr_h__
