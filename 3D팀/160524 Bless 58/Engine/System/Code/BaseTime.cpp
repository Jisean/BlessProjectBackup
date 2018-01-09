#include "BaseTime.h"

USING(Engine)

CBaseTime::CBaseTime(void)
{

}

CBaseTime::~CBaseTime(void)
{

}

void CBaseTime::Set_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	// ���ʰ� ������
	if(m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		// Cpu�� ���ʿ� ���� �ִ� ���ڰ� �޶��� �� �ִ�.
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_LastTime.QuadPart) / _float(m_CpuTick.QuadPart);
	m_LastTime = m_FrameTime;
}

HRESULT CBaseTime::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

CBaseTime* CBaseTime::Create(void)
{
	CBaseTime*		pTimer = new CBaseTime;

	if(FAILED(pTimer->Ready_Timer()))
	{
		MSG_BOX(L"pTimer Ready Failed");
		Safe_Release(pTimer);
	}

	return pTimer;
}

void CBaseTime::Free(void)
{
	
}

