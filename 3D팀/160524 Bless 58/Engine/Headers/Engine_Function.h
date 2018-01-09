#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T> void Safe_Delete (T& pointer)
	{
		if(NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}


	template <typename T> void Safe_Delete_Array (T& pointer)
	{
		if(NULL != pointer)
		{
			delete [] pointer;
			pointer = NULL;
		}
	}

	template <typename T> DWORD Safe_Release (T& pointer)
	{
		DWORD dwRefCnt = 0;
		if(NULL != pointer)
		{
			dwRefCnt = pointer->Release();
			if(0 == dwRefCnt)
				pointer = NULL;
		}
		return dwRefCnt;
	}

	template <typename T> void Safe_Single_Destory(T& pointer)
	{
		if(NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}
}

#endif // Engine_Function_h__
