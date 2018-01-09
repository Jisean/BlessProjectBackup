/*!
 * \file MainApp.h
 * \date 2015/04/01 15:08
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

#ifndef MainApp_h__
#define MainApp_h__

#include "Include.h"
#include "Base.h"


class CMainApp : public Engine::CBase
{
private:
	explicit CMainApp(void);
	virtual ~CMainApp(void);

	// 생성자
	// 소멸자
	// public 변수
	// public 함수
	// private 변수
	// private 함수
	// Create()
	// Free()
public:
	HRESULT InitApp(void);
	_int Update(const _float& fTimeDelta);
	void Render(void);
private:
	LPDIRECT3DDEVICE9		m_pDevice;
public:
	static CMainApp* Create(void);
	void Free(void);
};

#endif // MainApp_h__