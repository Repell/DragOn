#include "RcCol.h"

USING(ENGINE)

CRcCol::CRcCol(LPDIRECT3DDEVICE9 pGraphicDev)
	:CViBuffer(pGraphicDev)
{
}

CRcCol::CRcCol(const CRcCol & rhs)
	:CViBuffer(rhs)
{
}

CRcCol::~CRcCol(void)
{
}

HRESULT CRcCol::Ready_Buffer(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_COL;
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTX_COL);

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwIdxSize = sizeof(INDEX32);

	FAILED_CHECK_RETURN(CViBuffer::Ready_Buffer(), E_FAIL);

	VTX_COL* pVtx = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVtx[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVtx[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVtx[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVtx[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVtx[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVtx[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVtx[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIdx = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0]._0 = 0;
	pIdx[0]._1 = 1;
	pIdx[0]._2 = 2;

	pIdx[1]._0 = 0;
	pIdx[1]._1 = 2;
	pIdx[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

CRcCol * CRcCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcCol* pInstance = new CRcCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		ERR_BOX("RcCol Create Failed!");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CResourceses * CRcCol::Clone(void)
{
	return new CRcCol(*this);
}

void CRcCol::Free(void)
{
	CViBuffer::Free();
}
