#ifndef Mesh_h__
#define Mesh_h__

#include "Resourceses.h"

NS_BEGIN(ENGINE)

class CMesh :
	public CResourceses
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	_bool m_bClone;

public:
	virtual CResourceses* Clone() PURE;

public:
	virtual void Free();



};

NS_END

#endif // Mesh_h__
