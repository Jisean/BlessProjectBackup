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

	// ������
	// �Ҹ���
	// public ����
	// public �Լ�
	// private ����
	// private �Լ�
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