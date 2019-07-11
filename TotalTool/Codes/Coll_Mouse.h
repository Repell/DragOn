#pragma once

#ifndef COLLMOUSE_H__
#define COLLMOUSE_H__

#include "Collision.h"

class CColl_Mouse :
	public ENGINE::CCollision
{
public:
	explicit CColl_Mouse();
	virtual ~CColl_Mouse();

public:
	bool PickTerrain(D3DXVECTOR3* pOut,
		const ENGINE::VTX_TEX* pTerrerainVtx);
	bool PickObject(D3DXVECTOR3* pOut,
		const ENGINE::VTX_TEX* pTerrerainVtx,
		const D3DXMATRIX* pMatWorld);

private:
	HRESULT initialize();
	void Free();

private:
	void Translation_ViewSpace();
	void Translation_WorldSpace();
	void Translation_LocalSapce(const D3DXMATRIX* pMatWorld);

public:
	static CColl_Mouse* Create();

private:
	D3DXVECTOR3		m_vPivotPos;	// ±§º± √‚πﬂ¡°
	D3DXVECTOR3		m_vRayDir;		// ±§º± πÊ«‚
	D3DXVECTOR3		m_vMouse;
};


#endif // !COLLMOUSE_H__
