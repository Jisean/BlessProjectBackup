#pragma once

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimeMgr : public CBase
{
public:
	DECLARE_SINGLETON(CTimeMgr)
private:
	explicit CTimeMgr(void);
	virtual ~CTimeMgr(void);
public:
	float GetTime(void);
	void SetTime(void);
	void InitTime(void);
private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;
	float				m_fTime;

public:	
	virtual void Free(void);
};

END