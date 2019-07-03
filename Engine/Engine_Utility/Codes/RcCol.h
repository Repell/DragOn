#ifndef RcCol_h__
#define RcCol_h__

#include "ViBuffer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CRcCol : 
	public CViBuffer
{
private:
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol(void);

public:
	HRESULT Ready_Buffer(void);

public:
	static CRcCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResourceses* Clone(void);
	virtual void Free(void);

};

NS_END
#endif // RcCol_h__
