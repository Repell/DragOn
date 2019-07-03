#ifndef Engine_Macro_h__
#define Engine_Macro_h__


namespace ENGINE
{

#define  WINCX 1280
#define  WINCY 720

#define TERRAIN_VTX_X 129
#define TERRAIN_VTX_Z 129
#define TERRAIN_INTRV 1

#define NO_EVENT 0
#define END_EVENT -1

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#define			ERR_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#define			TAGMSG_BOX(_tag,_message)	MessageBoxW(NULL, _message, _tag, MB_OK)

#define			NS_BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			NS_END						}

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif // ENGINE_EXPORTS

#define NULL_CHECK(_ptr)	\
{if(_ptr == nullptr) { __asm {int 3}; return; }}

#define NULL_CHECK_RETURN(_ptr, _return)	\
{if(_ptr == nullptr){__asm{int 3};return _return;}}

#define NULL_CHECK_MSG(_ptr, _message)	\
{if(_ptr == nullptr){MessageBox(NULL, _message, L"System Message", MB_OK);__asm {int 3};}}

#define NULL_CHECK_RETURN_MSG(_ptr, _return, _message)	\
{if(_ptr == nullptr){MessageBox(NULL, _message, L"System Message", MB_OK);__asm{int 3}; return _return;}}

#define  FAILED_CHECK(_hr)if(((HRESULT)(_hr)) != S_OK)	\
{MessageBoxW(NULL, L"Failed!", L"System Error!", MB_OK); __asm {int 3}; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return) if((HRESULT)(_hr) != S_OK)	\
{MessageBoxW(NULL, L"Failed!", L"System Error!", MB_OK); __asm {int 3}; return _return;}

#define  FAILED_CHECK_MSG(_hr, _message)if(((HRESULT)(_hr)) != S_OK)	\
{MessageBoxW(NULL, _message, L"System Error!", MB_OK); __asm {int 3}; return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG(_hr, _return, _message) if((HRESULT)(_hr) != S_OK)	\
{MessageBoxW(NULL, _message, L"System Error!", MB_OK); __asm {int 3}; return _return;}

#define NO_COPY(ClassName)												\
private:																							\
	ClassName(const ClassName& obj);								\
	ClassName& operator=(const ClassName& obj);

#define DECLARE_SINGLETON(ClassName)							\
		NO_COPY(ClassName)														\
public:																							\
	static ClassName* Get_Instance(void)							\
	{																									\
		if(nullptr == m_pInstance)												\
			m_pInstance = new ClassName;									\
																										\
		return m_pInstance;															\
	}																									\
																										\
	void Destroy_Instance(void)												\
	{																									\
		if(nullptr != m_pInstance)												\
		{																								\
			delete m_pInstance;														\
			m_pInstance = nullptr;													\
		}																								\
	}																									\
																										\
private:																							\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)						\
ClassName* ClassName::m_pInstance = nullptr;

	//Constant
#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256

}
#endif // Engine_Macro_h__
