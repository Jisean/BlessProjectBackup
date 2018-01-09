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
	bool	m_bKeyDown[KEYMAX];	// 키가 눌렸는지 체크할 배열
	bool	m_bKeyUp[KEYMAX];	// 키가 떼졌는지 체크할 배열
public:
	bool        GetStateKey( INT nKey );// 키가 눌리고 있는지 체크
	bool        GetKeyDown( INT nKey );// 키가 한번 눌렸는지 체크
	bool        GetKeyUp( INT nKey );  // 키가 한번 눌렸다 떼졌는지 체크
	bool        GetToggleKey( INT nKey );// 한번 눌릴때마다 on off로 바뀜
public:
	void		Free(void);
private:
	CKeyMgr(void);
	~CKeyMgr(void);
};

#endif // KeyMgr_h__