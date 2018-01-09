
#include "Frame.h"

USING(Engine)

CFrame::CFrame(void)
: m_fLimitCall(0.f)
, m_fAccTimes(0.f)
{

}

CFrame::~CFrame(void)
{

}

HRESULT CFrame::Ready_Frame(const _float& fLimitCall)
{
	m_fLimitCall = 1.f / fLimitCall;

	return S_OK;	
}

bool CFrame::Pass_Limit(const _float& fTimeDelta)
{
	m_fAccTimes += fTimeDelta;

	if(m_fAccTimes >= m_fLimitCall)
	{
		m_fAccTimes = 0.f;
		return true;
	}	

	return false;
}

CFrame* CFrame::Create(const _float& fLimitCall)
{
	CFrame*		pFrame = new CFrame;

	if(FAILED(pFrame->Ready_Frame(fLimitCall)))
	{
		MSG_BOX(L"pFrame Ready Failed");
		Safe_Release(pFrame);
	}

	return pFrame;
}

void CFrame::Free(void)
{

}
