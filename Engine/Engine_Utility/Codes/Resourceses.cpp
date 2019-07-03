#include "Resourceses.h"

USING(ENGINE)

CResourceses::CResourceses(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CResourceses::CResourceses(const CResourceses & rhs)
	:m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CResourceses::~CResourceses(void)
{

}

void CResourceses::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
