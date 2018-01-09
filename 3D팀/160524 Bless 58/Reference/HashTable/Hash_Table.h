/*!
 * \file Hash_Table.h
 * \date 2016/05/20 12:18
 *
 * \author �ǿ���
 * Contact: user@company.com
 *
 * \brief �ؽ� ���̺�
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
	// ���̺� �ʱ�ȭ
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
	// ���̺� Ŭ���� ���Ի��� ����
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
	// ���̺� �̷����� ���Ի��� �Ѵ�
	void	Erase_Table(void)
	{
		delete [] ((HASH_SLOT_SINGLE*)m_pHashSlot);
		m_pHashSlot = NULL;
	}
	// ������ �ޱ�
	unsigned int	Get_TableSize(void)
	{
		return m_iTablesize;
	}
	// ���̺� Ű�� �� ����
	void	Insert_Table(const TCHAR* pKey, void* data)
	{
		int iKey = 0;
		iKey = Make_HashKey(pKey, HT_INSERT);

		// Ű�� �ߺ��� ���
		if(-1 == iKey)
			return;
		
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey = new TCHAR[MAX_PATH];
		wcscpy_s((TCHAR*)(((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey), MAX_PATH, pKey);
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_eState = INUSE;
		((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_pData = data;
	}
	// ������ ����(���⼭ ���� ���ϰ� ��ȯ�ϸ� �޾Ƽ� �����ϵ簡)
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
	// ������ Ž��
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

	// ������ ����
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
	// Ű ��ȯ
	unsigned int Make_HashKey(const TCHAR* pKey, DWORD hashstate = HT_INSERT)
	{
		// ȣ���� ���(�Ҽ� 19)
		int iKey = 0;
		//wcscpy_s()
		const TCHAR* pTempKey = pKey;

		while('\0' != *pKey)
		{
			iKey = (iKey * 19) + (*pKey);
			*pKey++;
		}

		iKey = iKey % m_iTablesize;

		// ���ϴ� ���ۿ� �°� �����ؽ�
		const TCHAR* pNowKey = ((HASH_SLOT_SINGLE*)m_pHashSlot)[iKey].m_hashKey;

		switch(hashstate)
		{
		case HT_INSERT:
			if(NULL != pNowKey)
			{
				if(!wcscmp(pNowKey, pTempKey))
				{
					MSG_BOX(L"Ű �� �ߺ�");
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
				//MSG_BOX(L"ã�� ������ ����");
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
				MSG_BOX(L"ã�� ������ ����");
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
		// ȣ���� ���(�Ҽ� 37)
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

		// �ؽ����̺� ���� ���
		if(iKey == Make_HashKey(pTempKey, HT_DEFAULT))
		{
			TAGMSG_BOX(pTempKey, L"�ؽ� ���̺� �� ��");
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
					MSG_BOX(L"Ű �� �ߺ�");
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
				//MSG_BOX(L"ã�� ������ ����");
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
				MSG_BOX(L"ã�� ������ ����");
				iKey = -1;
				return iKey;
			}
			break;
		}

		return iKey;
	}
}HTable;



#endif // Hash_Table_h__
