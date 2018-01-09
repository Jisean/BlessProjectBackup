/*!
 * \file Hash_Table.h
 * \date 2016/05/20 12:18
 *
 * \author 권오훈
 * Contact: user@company.com
 *
 * \brief 해쉬 테이블
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Hash_Table_h__
#define Hash_Table_h__

#include "Hash_Slot.h"
#include "string.h"

#define MSG_BOX(_message)	MessageBox(NULL, _message, L"System Message", MB_OK)
#define TAGMSG_BOX(_tag,_message)	MessageBox(NULL, _message, _tag, MB_OK)

typedef struct	_table
{
private:
	enum	HASHSTATE { HT_INSERT, HT_SEARCH, HT_DELETE, HT_DEFAULT };
public:
	_table(void){;}
	_table( const _table& rhs) 
	{
		m_pHashSlot = rhs.m_pHashSlot; 
		m_iTablesize = rhs.m_iTablesize;
	}
public:
	HASH_SLOT*		m_pHashSlot;
	unsigned int	m_iTablesize;
public:
	// 테이블 초기화
	void	Ready_Table(unsigned int iTableSize)
	{
		m_iTablesize = iTableSize;
		m_pHashSlot = new HASH_SLOT_SINGLE[iTableSize];

		for(unsigned int i = 0; i < iTableSize; ++i)
		{
			((HASH_SLOT_SINGLE*)(m_pHashSlot))[i].m_eState = EMPTY;
			((HASH_SLOT_SINGLE*)(m_pHashSlot))[i].m_hashKey = NULL;
			((HASH_SLOT_SINGLE*)(m_pHashSlot))[i].m_pData = NULL;
		}
	}
	// 테이블 클리어 슬롯삭제 안함
	void	Clear_Table(void)
	{
		for(unsigned int iKey = 0; iKey < m_iTablesize; ++iKey)
		{
			if(EMPTY != ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState)
			{
				((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey = NULL;
				((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState = EMPTY;
				((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData = NULL;
			}
		}
	}
	// 테이블 이레이즈 슬롯삭제 한다
	void	Erase_Table(void)
	{
		delete [] ((HASH_SLOT_SINGLE*)m_pHashSlot);
		m_pHashSlot = NULL;
	}
	// 사이즈 받기
	unsigned int	Get_TableSize(void)
	{
		return m_iTablesize;
	}
	// 테이블에 키와 값 저장
	void	Insert_Table(const TCHAR* pKey, void* data)
	{
		int iKey = 0;
		iKey = Make_HashKey(pKey, HT_INSERT);

		// 키값 중복된 경우
		if(-1 == iKey)
			return;
		
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey = new TCHAR[MAX_PATH];
		wcscpy_s((TCHAR*)(((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey), MAX_PATH, pKey);
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState = INUSE;
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData = data;
	}
	// 데이터 삭제(여기서 삭제 안하고 반환하면 받아서 삭제하든가)
	void*	Erase_Slot(const TCHAR*  pKey)
	{
		unsigned int iKey = 0;
		iKey = Make_HashKey(pKey, HT_DELETE);
		if(-1 == iKey)
			return NULL;

		void* data = NULL;

		data = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData;
		delete [] (((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey);
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey = NULL;
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState = DELETED;
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData = NULL;

		return data;
	}
	void*	Erase_Slot(unsigned int  iKey)
	{
		void* data = NULL;

		if(NULL != ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData)
		{
			data = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData;
			delete [] (((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey);
			((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey = NULL;
			((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState = DELETED;
			((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData = NULL;			
		}
		return data;
	}
	// 데이터 탐색
	void*	Search_TableData(const TCHAR*  pKey)
	{
		unsigned int iKey = 0;
		iKey = Make_HashKey(pKey, HT_SEARCH);
		if(-1 == iKey)
			return NULL;

		void* data = NULL;

		data = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData;

		return data;
	}

	void*	Search_TableData(unsigned int	iKey)
	{
		void* data = NULL;

		data = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData;

		return data;
	}

	// 데이터 변경
	void*	Change_TableData(const TCHAR* pKey, void* pData)
	{
		unsigned int iKey = 0;
		iKey = Make_HashKey(pKey, HT_SEARCH);
		if(-1 == iKey)
			return NULL;

		void* data = NULL;

		data = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData = pData;

		return data;
	}
private:
	// 키 변환
	unsigned int Make_HashKey(const TCHAR* pKey, DWORD hashstate = HT_INSERT)
	{
		// 호너의 방법(소수 19)
		int iKey = 0;
		//wcscpy_s()
		const TCHAR* pTempKey = pKey;

		while('\0' != *pKey)
		{
			iKey = (iKey * 19) + (*pKey);
			*pKey++;
		}

		iKey = iKey % m_iTablesize;

		// 원하는 동작에 맞게 이중해싱
		const TCHAR* pNowKey = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey;

		switch(hashstate)
		{
		case HT_INSERT:
			if(NULL != pNowKey)
			{
				if(!wcscmp(pNowKey, pTempKey))
				{
					MSG_BOX(L"키 값 중복");
					return -1;
				}
			}
			if(INUSE == ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState)
			{
				iKey = Make_HashKey_Second(pTempKey, 1, HT_INSERT);
			}
			break;

		case HT_SEARCH:
			if(EMPTY != ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState)
			{
				if(NULL != pNowKey)
				{
					if(wcscmp(pNowKey, pTempKey))
						iKey = Make_HashKey_Second(pTempKey, 1, HT_SEARCH);
				}
			}
			else
			{
				//MSG_BOX(L"찾는 데이터 없음");
				//iKey = -1;
				return iKey;
			}
			break;

		case HT_DELETE:
			if(EMPTY != ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState)
			{
				if(NULL != pNowKey)
				{
					if(wcscmp(pNowKey, pTempKey))
						iKey = Make_HashKey_Second(pTempKey, 1, HT_DELETE);
				}
			}
			else
			{
				MSG_BOX(L"찾는 데이터 없음");
				iKey = -1;
				return iKey;
			}
			break;

		case HT_DEFAULT:
			break;
		}

		return iKey;
	}
	unsigned int Make_HashKey_Second(const TCHAR* pKey, unsigned int iCnt, DWORD hashstate)
	{
		// 호너의 방법(소수 37)
		int iKey = 0;
		unsigned int iCount = iCnt;
		const TCHAR* pTempKey = pKey;

		while('\0' != *pKey)
		{
			iKey = (iKey * 19) + (*pKey);
			*pKey++;
		}
		iKey += (iCount * iCount);

		iKey = iKey % m_iTablesize;

		// 해시테이블 꽉찬 경우
		if(iKey == Make_HashKey(pTempKey, HT_DEFAULT))
		{
			TAGMSG_BOX(pTempKey, L"해시 테이블 꽉 참");
			iKey = -1;
			return iKey;
		}

		const TCHAR* pNowKey = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey;

		switch(hashstate)
		{
		case HT_INSERT:
			if(NULL != pNowKey)
			{
				if(!wcscmp(pNowKey, pTempKey))
				{
					MSG_BOX(L"키 값 중복");
					return -1;
				}
			}
			if(INUSE == ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState)
			{
				iKey = Make_HashKey_Second(pTempKey, ++iCount, HT_INSERT);
			}
			break;

		case HT_SEARCH:
			if(EMPTY != ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState)
			{
				if(NULL != pNowKey)
				{
					if(wcscmp(pNowKey, pTempKey))
						iKey = Make_HashKey_Second(pTempKey, ++iCount, HT_SEARCH);
				}
			}
			else
			{
				//MSG_BOX(L"찾는 데이터 없음");
				//iKey = 0;
				return iKey;
			}
			break;

		case HT_DELETE:
			if(EMPTY != ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState)
			{
				if(NULL != pNowKey)
				{
					if(wcscmp(pNowKey, pTempKey))
						iKey = Make_HashKey_Second(pTempKey, ++iCount, HT_DELETE);
				}
			}
			else
			{
				MSG_BOX(L"찾는 데이터 없음");
				iKey = -1;
				return iKey;
			}
			break;
		}

		return iKey;
	}
}HTable;



#endif // Hash_Table_h__
