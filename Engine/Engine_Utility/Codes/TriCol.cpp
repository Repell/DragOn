#include "TriCol.h"

USING(ENGINE)

CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphicDev)
	:CViBuffer(pGraphicDev)
{
}

CTriCol::CTriCol(const CTriCol & rhs)
	:CViBuffer(rhs)
{
}

CTriCol::~CTriCol(void)
{
}

HRESULT CTriCol::Ready_Buffer(void)
{
	m_dwVtxCnt = 3;
	m_dwVtxFVF = FVF_COL;
	m_dwTriCnt = 1;
	m_dwVtxSize = sizeof(VTX_COL);

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwIdxSize = sizeof(INDEX32);

	FAILED_CHECK_RETURN(CViBuffer::Ready_Buffer(), E_FAIL);

	VTX_COL* pVtx = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vPos = _vec3(0.f, 0.5f, 0.f);
	pVtx[0].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVtx[1].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVtx[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVtx[2].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVtx[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIdx = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0]._0 = 0;
	pIdx[0]._1 = 1;
	pIdx[0]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
}

CTriCol * CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriCol* pInstance = new CTriCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		ERR_BOX("RcCol Create Failed!");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CResourceses * CTriCol::Clone(void)
{
	return new CTriCol(*this);
}

void CTriCol::Free(void)
{
	CViBuffer::Free();
}
