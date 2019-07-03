#pragma once

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "VIBuffer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CSkyBox :
	public CViBuffer
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBox(const CSkyBox& rhs);

public:
	virtual ~CSkyBox();

public:
	virtual HRESULT Ready_Buffer();
	virtual void Render_Buffer();

private:

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResourceses* Clone(void);
	void Free();

};

NS_END

#endif // !__SKYBOX_H__
