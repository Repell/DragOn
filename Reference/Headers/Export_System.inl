#include "Export_System.h"

CGraphicDev* Get_GraphicDev()
{
	return CGraphicDev::Get_Instance();
}

HRESULT Ready_GraphicDev(HWND hWnd, CGraphicDev::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, ENGINE::CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::Get_Instance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

inline const float& Get_DeltaTime()
{
	return CTimeMgr::Get_Instance()->Get_DeltaTime();
}

inline void Update_Time()
{
	CTimeMgr::Get_Instance()->Update_Time();
}

inline void Init_Time()
{
	CTimeMgr::Get_Instance()->Init_Time();
}

inline void KeyInput()
{
	CKeyMgr::Get_Instance()->KeyInput();
}

inline bool Key_Down(DWORD dwKey)
{
	return CKeyMgr::Get_Instance()->Key_Down(dwKey);
}

inline bool Key_Up(DWORD dwKey)
{
	return CKeyMgr::Get_Instance()->Key_Up(dwKey);
}

inline bool Key_Press(DWORD dwKey)
{
	return CKeyMgr::Get_Instance()->Key_Press(dwKey);
}

inline bool Key_Combined(DWORD dwFirst, DWORD dwSecond)
{
	return CKeyMgr::Get_Instance()->Key_Combined(dwFirst, dwSecond);
}

inline DWORD Key_Press_Comb(DWORD dwFirst, DWORD dwSecond)
{
	return CKeyMgr::Get_Instance()->Key_Press_Comb(dwFirst, dwSecond);
}

inline _byte Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::Get_Instance()->Get_DIKeyState(byKeyID);
}

inline _byte Get_DIMouseState(CInputDev::MOUSE_KEYSTATE eMouse)
{
	return CInputDev::Get_Instance()->Get_DIMouseState(eMouse);
}

inline _long Get_DIMouseMove(CInputDev::MOUSE_MOVESTATE eMouseState)
{
	return CInputDev::Get_Instance()->Get_DIMouseMove(eMouseState);
}

inline void Set_InputDev(void)
{
	CInputDev::Get_Instance()->Set_InputDev();
}

inline HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::Get_Instance()->Ready_InputDev(hInst, hWnd);
}

inline HRESULT Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFontTag, const _tchar * pFontType, const _uint & iWidth, const _uint & iHeight, const _uint & iWeight)
{
	return CFontMgr::Get_Instance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

inline void Render_Font(const _tchar * pFontTag, const _tchar * pString, const _vec2 * pPos, D3DXCOLOR Color)
{
	CFontMgr::Get_Instance()->Render_Font(pFontTag, pString, pPos, Color);
}

inline void Release_System()
{
	CFontMgr::Get_Instance()->Destroy_Instance();
	CInputDev::Get_Instance()->Destroy_Instance();
	CKeyMgr::Get_Instance()->Destroy_Instance();
	CTimeMgr::Get_Instance()->Destroy_Instance();
	CGraphicDev::Get_Instance()->Destroy_Instance();
}



