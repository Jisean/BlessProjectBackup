#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CFrame;
class ENGINE_DLL CFrameMgr : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)
private:
	explicit CFrameMgr(void);
	virtual ~CFrameMgr(void);
public:
	bool Pass_Limit(const TCHAR* pFrameTag, const float& fTimeDelta);
public:
	HRESULT Ready_Frame(const TCHAR* pFrameTag, const float& fLimitTime);
private:
	typedef HTable		HTFRAMES;
	HTFRAMES			m_htFrames;
private:
	CFrame* Find_Frame(const TCHAR* pFrameTag);
public:
	virtual void Free(void);
};

END


#endif // CFrameMgr
