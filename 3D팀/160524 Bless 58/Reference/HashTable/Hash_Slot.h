/*!
 * \file Hash_Slot.h
 * \date 2016/05/20 12:18
 *
 * \author ±«ø¿»∆
 * Contact: user@company.com
 *
 * \brief «ÿΩ¨ ΩΩ∑‘
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Hash_Slot_h__
#define Hash_Slot_h__

typedef	const TCHAR*	Key;

enum	SLOTSTATE	{ EMPTY, DELETED, INUSE };

typedef struct _slot
{
	Key					m_hashKey;
}HASH_SLOT;

typedef struct _slotsingle : public HASH_SLOT
{
	void*				m_pData;	
	enum	SLOTSTATE	m_eState;
}HASH_SLOT_SINGLE;

typedef struct _slotmulti : public HASH_SLOT
{
	std::vector<void*>			m_pDataVec;
}HASH_SLOT_MULTI;

#endif // Hash_Slot_h__
