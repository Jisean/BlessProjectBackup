#include "FrameMgr.h"
#include "Frame.h"

USING(Engine)

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr(void)
{
	m_htFrames.Ready_Table(11);
}

CFrameMgr::~CFrameMgr(void)
{

}

bool CFrameMgr::Pass_Limit(const TCHAR* pFrameTag, const float& fTimeDelta)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if(NULL == pFrame)
		return false;

	return pFrame->Pass_Limit(fTimeDelta);	
}

HRESULT CFrameMgr::Ready_Frame(const TCHAR* pFrameTag, const float& fLimitTime)
{
	CFrame*	pFrame = (CFrame*)m_htFrames.Search_TableData(pFrameTag);

	if(NULL != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fLimitTime);
	if(NULL == pFrame)
		return E_FAIL;

	m_htFrames.Insert_Table(pFrameTag, pFrame);

	return S_OK;	
}

CFrame* CFrameMgr::Find_Frame(const TCHAR* pFrameTag)
{
	CFrame*	pFrame = (CFrame*)m_htFrames.Search_TableData(pFrameTag);

	return pFrame;
}

void CFrameMgr::Free(void)
{
	size_t iTableSize =	m_htFrames.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		CFrame*		pFrame = (CFrame*)m_htFrames.Erase_Slot(i);
		if(NULL != pFrame)
			Safe_Release(pFrame);
	}
	m_htFrames.Erase_Table();
}

