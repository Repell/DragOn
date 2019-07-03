#pragma once

#ifndef TERRAINTEX_H__
#define TERRAINTEX_H__

#include "ViBuffer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CTerrainTex :
	public CViBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);

public:
	virtual ~CTerrainTex();

public:
	void Get_VtxPos(_vec3* pVec) 
	{		memcpy(pVec, m_vPos, sizeof(_vec3) * m_dwVtxCnt);	}
	const _vec3* Get_vVtxPos() const { return m_vPos; }

public:
	virtual HRESULT Ready_Buffer(WORD wCntX, WORD wCntY, WORD wItv, WORD wDetail);
	virtual void Render_Buffer();
	void Copy_Index(INDEX32* pIndex, const _ulong& dwTriCnt);

private:
	DWORD* LoadHeightMap();

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const WORD & wCntX,
		const WORD & wCntY,
		const WORD & wItv,
		const WORD wDetail);
	virtual CResourceses* Clone(void);
	void Free();

private:
	_vec3* m_vPos;
	_bool m_bClone;

};

NS_END

#endif // !TERRAINTEX_H__

