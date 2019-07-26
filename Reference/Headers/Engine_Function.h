#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace ENGINE
{
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

	//////////////////// FUNCTOR ///////////////////

	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag)
			: m_pTargetTag(pTag) {		}
		~CTag_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (NULL == lstrcmpW(m_pTargetTag, pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public:
		template<typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;
			dwRefCnt = pInstance->Release();

			if (NULL == dwRefCnt)
				pInstance = nullptr;
		}
	};

	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public:
		template<typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;
			dwRefCnt = Pair.second->Release();

			if (NULL == dwRefCnt)
				Pair.second = nullptr;
		}
	};

	template <typename T>
	T LimitNumber(T& Value, T Min, T Max)
	{
		return(Value < Min) ? (Value = Min) : ((Value > Max) ? (Value = Max) : Value);
	}

}


#endif // Engine_Function_h__
