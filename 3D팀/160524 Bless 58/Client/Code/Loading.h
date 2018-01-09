/*!
 * \file Loading.h
 * \date 2016/02/23 14:20
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

#ifndef Loading_h__
#define Loading_h__

#include "Base.h"

class CLoading : public Engine::CBase
{
public:
	enum LOADINGID {LOADING_STAGE1, LOADING_STAGE2};

private:
	explicit CLoading(LOADINGID eLoadID);
	~CLoading(void);

public:
	bool GetComplete(void);
	const TCHAR* GetLoadMessage(void);
	const TCHAR* GetLoadPath(void);

public:
	HRESULT InitLoading(void);
	void StageLoading(LOADINGID eLoadID);
private:
	void	UI_Setting(void);
	HRESULT MonsterLoad(LOADINGID eLoadID);
	HRESULT StageMeshTexLoad(LOADINGID eLoadID);
	HRESULT StageMeshLoad(LOADINGID eLoadID);
	HRESULT EffectResourceLoad(void);
	HRESULT	EffectReadFile(const wstring& wstrPath);
	HRESULT PlayerLoad(void);
	HRESULT NPCLoad(void);

public:
	static CLoading* Create(LOADINGID eLoadID);
	static UINT WINAPI LoadingFunction(void* pArg);

private:
	virtual void Free(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LOADINGID				m_eLoadID;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;
	vector<wstring> vecMeshCheck;
	vector<wstring> vecTextureCheck;
	

private:
	TCHAR		m_szLoadMessage[MAX_PATH];
	TCHAR		m_szLoadPath[MAX_PATH];
	bool		m_bComplete;
	
};

#endif // Loading_h__