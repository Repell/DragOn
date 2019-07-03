#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Define.h"
#include "GraphicDev.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "DirectInput.h"
#include "FontMgr.h"

NS_BEGIN(ENGINE)

//GraphicDev
//Get
inline CGraphicDev* Get_GraphicDev();
//Set
//Public
inline HRESULT Ready_GraphicDev(HWND hWnd, CGraphicDev::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, ENGINE::CGraphicDev** ppGraphicDev);

//TimeMgr
//Get
inline const float& Get_DeltaTime();
//Set
//Public
inline void Init_Time();
inline void Update_Time();

//KeyMgr
//Get
//Set
//Public
inline void KeyInput();
inline bool Key_Down(DWORD dwKey);
inline bool Key_Up(DWORD dwKey);
inline bool Key_Press(DWORD dwKey);
inline bool Key_Combined(DWORD dwFirst, DWORD dwSecond);
inline DWORD Key_Press_Comb(DWORD dwFirst, DWORD dwSecond);

//DirectInput
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(CInputDev::MOUSE_KEYSTATE eMouse);
inline _long	Get_DIMouseMove(CInputDev::MOUSE_MOVESTATE eMouseState);
// Set
inline void	Set_InputDev(void);
// Public
inline HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);

// FontMgr
// Get
// Set
// Public
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color);


//Release
inline void Release_System();

#include "Export_System.inl"

NS_END

#endif // Export_System_h__
