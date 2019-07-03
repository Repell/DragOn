#pragma once

#ifndef __RCTEX_H__
#define __RCTEX_H__

#include "ViBuffer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CRcTex :
	public CViBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);

public:
	virtual ~CRcTex();

public:
	virtual HRESULT Ready_Buffer();
	virtual void Render_Buffer();

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResourceses* Clone(void);
	void Free(void);
};

NS_END

#endif // !__RCTEX_H__

