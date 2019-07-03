#include "RcTex.h"

USING(ENGINE);

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CViBuffer(pGraphicDev)
{
}

CRcTex::CRcTex(const CRcTex & rhs)
	:	CViBuffer(rhs)
{
}

CRcTex::~CRcTex()
{
	
}

HRESULT CRcTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTX_TEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CViBuffer::Ready_Buffer(), E_FAIL);

	VTX_TEX* pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = { -1.f, 1.f, 0.f };	
	pVtxTex[0].vTexUV = { 0.f, 0.f };	//UV ÁÂÇ¥
	
	pVtxTex[1].vPos = { 1.f, 1.f, 0.f };
	pVtxTex[1].vTexUV = { 1.f, 0.f };	//UV ÁÂÇ¥

	pVtxTex[2].vPos = { 1.f, -1.f, 0.f };
	pVtxTex[2].vTexUV = { 1.f, 1.f };	//UV ÁÂÇ¥

	pVtxTex[3].vPos = { -1.f, -1.f, 0.f };
	pVtxTex[3].vTexUV = { 0.f, 1.f };	//UV ÁÂÇ¥

	

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0] = {0, 1, 2};
	_vec3 p1 = pVtxTex[0].vPos - pVtxTex[1].vPos;
	_vec3 p2 = pVtxTex[1].vPos - pVtxTex[2].vPos;
	_vec3 pNormal;
	D3DXVec3Cross(&pNormal, &p2, &p1);
	D3DXVec3Normalize(&pNormal, &pNormal);

	pVtxTex[0].vNormal = pNormal;
	pVtxTex[1].vNormal = pNormal;
	pVtxTex[2].vNormal = pNormal;
	pVtxTex[3].vNormal = pNormal;

	pIndex[1] = { 0, 2, 3 };

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CRcTex::Render_Buffer()
{
	CViBuffer::Render_Buffer();
}

void CRcTex::Free(void)
{
	CViBuffer::Free();
}

CRcTex * CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex* pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CResourceses * CRcTex::Clone(void)
{
	return new CRcTex(*this);
}


