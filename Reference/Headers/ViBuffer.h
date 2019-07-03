#ifndef ViBuffer_h__
#define ViBuffer_h__

#include "Resourceses.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CViBuffer :
	public CResourceses
{
protected:
	explicit CViBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CViBuffer(const CViBuffer& rhs);
	virtual ~CViBuffer(void);

public:
	HRESULT Ready_Buffer(void);
	void			Render_Buffer(void);	//¿”Ω√ ∑ª¥ı


public:
	void Set_OriginVtx(VTX_TEX* pVtx, DWORD dSize);
public:
	void Get_VtxInfo(void * pVtx);
	_ulong Get_TriCnt() { return m_dwTriCnt; }

protected:
	LPDIRECT3DVERTEXBUFFER9		 m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	_ulong			m_dwVtxSize;
	_ulong 		m_dwVtxCnt;
	_ulong 		m_dwTriCnt;
	_ulong 		m_dwVtxFVF;

	_ulong 		m_dwIdxSize;
	D3DFORMAT	m_IdxFmt;

private:
	VTX_TEX*		m_pOriginTex;

public:
	virtual void Free(void);


};

NS_END

#endif // ViBuffer_h__
