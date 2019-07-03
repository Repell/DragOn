#include "Mesh.h"

USING(ENGINE)

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResourceses(pGraphicDev),
	m_bClone(false)
{
}

CMesh::CMesh(const CMesh & rhs)
	: CResourceses(rhs),
	m_bClone(true)
{
}

CMesh::~CMesh()
{
}

void CMesh::Free()
{
	CResourceses::Free();
}
