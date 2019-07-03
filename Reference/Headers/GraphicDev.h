#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Base.h"
#include "Engine_Define.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum	WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit	CGraphicDev(void);
	virtual		~CGraphicDev(void);

public:
	LPDIRECT3DDEVICE9		Get_Device(void) { return m_pGraphicDev;  }
	LPD3DXSPRITE					Get_Sprite(void) { return m_pSprite; }
	//LPD3DXFONT						Get_Font(void) { return m_pFont; }
	LPD3DXLINE						Get_Line(void) { return m_pLine; }


public:
	HRESULT		Ready_GraphicDev(HWND hWnd, 
								WINMODE eMode, 
								const _uint& iSizeX,
								const _uint& iSizeY,
								ENGINE::CGraphicDev** ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(void);

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	LPD3DXSPRITE						m_pSprite = nullptr;
	//LPD3DXFONT							m_pFont = nullptr;
	LPD3DXLINE							m_pLine = nullptr;

public:
	inline	virtual void Free(void);
};

NS_END
#endif // GraphicDev_h__
