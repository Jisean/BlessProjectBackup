#include "TimeMgr.h"
#include "BaseTime.h"

USING(Engine)

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr(void)
{
	m_htTimes.Ready_Table(19);
}

CTimeMgr::~CTimeMgr(void)
{

}

float CTimeMgr::Get_TimeDelta(const TCHAR* pTimerTag)
{
	CBaseTime* pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return 0.0f;

	return pTimer->Get_TimeDelta();
}

void CTimeMgr::Set_TimeDelta(const TCHAR* pTimerTag)
{
	CBaseTime* pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return;

	pTimer->Set_TimeDelta();
}

HRESULT CTimeMgr::Ready_Timer(const TCHAR* pTimerTag)
{
	CBaseTime* pTimer = Find_Timer(pTimerTag);

	if(NULL != pTimer)
		return E_FAIL;

	pTimer = CBaseTime::Create();
	if(NULL == pTimer)
		return E_FAIL;

	m_htTimes.Insert_Table(pTimerTag, pTimer);

	return S_OK;	
}

CBaseTime* CTimeMgr::Find_Timer(const TCHAR* pTimerTag)
{
	CBaseTime* pTime = (CBaseTime*)m_htTimes.Search_TableData(pTimerTag);

	return pTime;	
}

void CTimeMgr::Free(void)
{
	size_t iTableSize =	m_htTimes.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		CBaseTime*		pTime = (CBaseTime*)m_htTimes.Erase_Slot(i);
		if(NULL != pTime)
			Safe_Release(pTime);
	}
	m_htTimes.Erase_Table();
}
