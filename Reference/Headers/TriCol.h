#ifndef TriCol_h__
#define TriCol_h__

#include "ViBuffer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CTriCol : 
	public CViBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol(void);

public:
	HRESULT Ready_Buffer(void);

public:
	static CTriCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResourceses* Clone(void);
	virtual void Free(void);

};

NS_END
#endif // TriCol_h__
