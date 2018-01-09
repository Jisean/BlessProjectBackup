#ifndef Function_h__
#define Function_h__

class CStringCompare
{
public:
	explicit CStringCompare(const TCHAR* pKey)
		: m_pString(pKey) {}
	~CStringCompare() {}
public:
	template <typename T>
	bool operator () (T Data)
	{
		return !lstrcmp(Data.first, m_pString);
	}
private:
	const TCHAR*	m_pString;
};

#endif // Function_h__
