/*!
 * \file KeyMgr.h
 * \date 2016/03/23 3:02
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

#ifndef KeyMgr_h__
#define KeyMgr_h__

#include "Engine_Include.h"
#include "Base.h"

const int   KEYMAX  =   256;

class CKeyMgr : public Engine::CBase
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	bool	m_bKeyDown[KEYMAX];	// Ű�� ���ȴ��� üũ�� �迭
	bool	m_bKeyUp[KEYMAX];	// Ű�� �������� üũ�� �迭
public:
	bool        GetStateKey( INT nKey );// Ű�� ������ �ִ��� üũ
	bool        GetKeyDown( INT nKey );// Ű�� �ѹ� ���ȴ��� üũ
	bool        GetKeyUp( INT nKey );  // Ű�� �ѹ� ���ȴ� �������� üũ
	bool        GetToggleKey( INT nKey );// �ѹ� ���������� on off�� �ٲ�
public:
	void		Free(void);
private:
	CKeyMgr(void);
	~CKeyMgr(void);
};

#endif // KeyMgr_h__