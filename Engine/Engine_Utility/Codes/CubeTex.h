#pragma once

#ifndef __CUBETEX_H__
#define __CUBETEX_H__

#include "ViBuffer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CCubeTex :
	public CViBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CCubeTex();

public:
	virtual HRESULT Ready_Buffer();
	virtual void Render_Buffer();

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResourceses* Clone(void);
	void Free(void);

};

NS_END

#endif // !__CUBETEX_H__
