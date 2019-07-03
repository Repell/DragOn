#ifndef MyFunction_h__
#define MyFunction_h__

template <typename T>
DWORD Safe_Release(T& pointer)
{
	DWORD	dwRefCnt = 0;
	if (nullptr != pointer)
	{
		dwRefCnt = pointer->Release();
		if (dwRefCnt == 0)
			pointer = nullptr;
	}
	return dwRefCnt;
}

template <typename T>
DWORD Safe_Release_Delete(T& pointer)
{
	DWORD	dwRefCnt = 0;
	if (nullptr != pointer)
	{
		dwRefCnt = pointer->Release();
		if (dwRefCnt == 0)
		{
			delete pointer;
			pointer = nullptr;
		}
	}
	return dwRefCnt;
}

template <typename T>
void Safe_Single_Destory(T& pointer)
{
	if (nullptr != pointer)
	{
		pointer->DestroyInstance();
		pointer = nullptr;
	}
}

template <typename T>
void Safe_Delete(T& pointer)
{
	if (nullptr != pointer)
	{
		delete pointer;
		pointer = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& pointer)
{
	if (nullptr != pointer)
	{
		delete[] pointer;
		pointer = nullptr;
	}
}

#endif // MyFunction_h__
