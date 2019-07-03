#ifndef Resourceses_h__
#define Resourceses_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CResourceses :
	public CComponent
{
protected:
	explicit CResourceses(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResourceses(const CResourceses& rhs);
	virtual ~CResourceses(void);

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

public:
	virtual CResourceses* Clone(void)PURE;
	virtual void Free(void);


};

NS_END

#endif // Resourceses_h__
