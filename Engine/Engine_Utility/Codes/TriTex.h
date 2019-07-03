#ifndef TriTex_h__
#define TriTex_h__

#include "ViBuffer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CTriTex : 
	public CViBuffer
{
private:
	explicit CTriTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriTex(const CTriTex& rhs);
	virtual ~CTriTex(void);

public:
	void Set_Line(LINE_3D pLine) { m_Line = pLine; }
	LINE_3D& Get_3DLine() { return m_Line; }
	void Set_World(_matrix* pWorld);
	void Set_TriTex(LINE_3D pLine);

public:
	HRESULT Ready_Buffer();
	void		Render_Buffer();

public:
	static CTriTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResourceses* Clone(void);
	virtual void Free(void);

private:
	bool bIsClone;
	LINE_3D m_Line;
	_matrix m_matWorld;

	LPD3DXMESH Sphere[3];

};

NS_END
#endif // TriTex_h__
