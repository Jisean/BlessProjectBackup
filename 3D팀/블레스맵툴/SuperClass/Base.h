#ifndef Base_h__
#define Base_h__

namespace Engine
{
	class _declspec(dllexport) CBase
	{
	protected:
		inline explicit CBase(void);
		inline virtual ~CBase(void);
	protected:
		unsigned long		  m_dwRefCnt;
	public:
		inline virtual void Free(void) = 0;
		inline virtual unsigned long Release(void);

	};
}

#include "Base.inl"

#endif // Base_h__
