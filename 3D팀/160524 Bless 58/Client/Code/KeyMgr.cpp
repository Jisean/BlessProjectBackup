#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
{
	ZeroMemory( m_bKeyDown, KEYMAX );
	ZeroMemory( m_bKeyUp, KEYMAX );
	// Ű ���۵��� �ʱ�ȭ ��Ų��.
}

CKeyMgr::~CKeyMgr(void)
{

}

//  ��� ��������
bool CKeyMgr::GetStateKey(INT nKey)
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{
		return TRUE;
	}

	return FALSE;
}

//  �ѹ��� �������� üũ
bool CKeyMgr::GetKeyDown(INT nKey)
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{
		//  ������ ���� true
		if( m_bKeyDown[nKey] == false )
		{
			m_bKeyDown[nKey]    =   true;
			return TRUE;
		}
	}

	else
	{
		//  ���� ���� false
		m_bKeyDown[nKey]    =   false;
	}   

	return FALSE;
}

//  �ѹ� ���ȴٰ� ��������� üũ
bool CKeyMgr::GetKeyUp(INT nKey)
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{   
		m_bKeyUp[nKey]  =   true;
	}

	else
	{
		// ������ ���� true�� �ǹǷ� true�϶��� ����
		if( m_bKeyUp[nKey] == true )
		{
			//  Ű���¸� false�� ����� ������ �Ұ�
			m_bKeyUp[nKey]  =   false;

			return TRUE;
		}
	}   

	return FALSE;
}

//  ���Ű
bool CKeyMgr::GetToggleKey(INT nKey)
{
	if( GetKeyState(nKey) & 0x0001 ) // GetKeyState�� 0x0001 �� ������ ���ȴ°� �� üũ�Ѵ�.
	{
		return true;
	}

	return false;
}

void CKeyMgr::Free(void)
{

}
