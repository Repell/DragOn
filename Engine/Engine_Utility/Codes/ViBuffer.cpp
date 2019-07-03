#include "ViBuffer.h"

USING(ENGINE)

CViBuffer::CViBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResourceses(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
	, m_pOriginTex(nullptr)
{

}

CViBuffer::CViBuffer(const CViBuffer & rhs)
	:CResourceses(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CViBuffer::~CViBuffer(void)
{

}

HRESULT CViBuffer::Ready_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,
		0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL), E_FAIL);

	return S_OK;
}

void CViBuffer::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CViBuffer::Set_OriginVtx(VTX_TEX * pVtx, DWORD dSize)
{
	m_pOriginTex = new VTX_TEX[dSize];
	memcpy(m_pOriginTex, pVtx, sizeof(VTX_TEX) * dSize);
}

void CViBuffer::Get_VtxInfo(void * pVtx)
{
	void* pOriginVtx = nullptr;

	m_pVB->Lock(0, 0, &pOriginVtx, 0);
	memcpy(pVtx, pOriginVtx, m_dwVtxCnt * m_dwVtxSize);

	m_pVB->Unlock();
}

void CViBuffer::Free(void)
{
	if (m_pOriginTex != nullptr)
		Safe_Delete_Array(m_pOriginTex);

	CResourceses::Free();

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
