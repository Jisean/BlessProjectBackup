/*!
 * \file SecondThread.h
 * \date 2016/05/30 19:44
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

#ifndef SecondThread_h__
#define SecondThread_h__

#include "Base.h"

class CSecondThread : public Engine::CBase
{
private:
	explicit CSecondThread(void);

public:
	~CSecondThread(void);

public:
	bool GetComplete(void);
	const TCHAR* GetLoadMessage(void);
	const TCHAR* GetLoadPath(void);

public:
	HRESULT InitLoading(void);
	HRESULT PlayerLoad(void);

public:
	static CSecondThread* Create(void);
	static UINT WINAPI LoadingFunction(void* pArg);

private:
	virtual void Free(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;

private:
	TCHAR		m_szLoadMessage[MAX_PATH];
	TCHAR		m_szLoadPath[MAX_PATH];
	bool		m_bComplete;
};

#endif // SecondThread_h__