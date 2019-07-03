#include "TriTex.h"

#define  RADIUS 1.f

USING(ENGINE)

CTriTex::CTriTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CViBuffer(pGraphicDev), bIsClone(FALSE)
{
}

CTriTex::CTriTex(const CTriTex & rhs)
	: CViBuffer(rhs), bIsClone(TRUE)
{
	for (int i = 0; i < 3; ++i)
		Sphere[i] = rhs.Sphere[i];

}

CTriTex::~CTriTex(void)
{
}

void CTriTex::Set_World(_matrix * pWorld)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, pWorld);
	m_matWorld = (*pWorld);
}

void CTriTex::Set_TriTex(LINE_3D pLine)
{
	VTX_TEX* pVtx = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vPos = pLine.vLine_X[0];
	pVtx[0].vTexUV = _vec2(0.f, 0.f);

	pVtx[1].vPos = pLine.vLine_Y[0];
	pVtx[1].vTexUV = _vec2(1.f, 0.f);

	pVtx[2].vPos = pLine.vLine_Z[0];
	pVtx[2].vTexUV = _vec2(1.f, 1.f);

	//Engine::VTX_TEX* pTex;
	//for (int i = 0; i < 3; ++i)
	//{
	//	if (SUCCEEDED(Sphere[i]->LockVertexBuffer(0, (void**)&pTex)))
	//	{
	//		int numVtx = Sphere[i]->GetNumVertices();

	//		for (int j = 0; j < numVtx; ++j)
	//		{
	//			pTex[j].vPos = m_pVtx_0[j].vPos + pVtx[i].vPos;
	//		}

	//		Sphere[i]->UnlockVertexBuffer();
	//	}
	//}

	//m_pVB->Unlock();

}

HRESULT CTriTex::Ready_Buffer()
{
	m_dwVtxCnt = 3;
	m_dwVtxFVF = FVF_COL;
	m_dwTriCnt = 1;
	m_dwVtxSize = sizeof(VTX_TEX);

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwIdxSize = sizeof(INDEX32);

	FAILED_CHECK_RETURN(CViBuffer::Ready_Buffer(), E_FAIL);

	VTX_TEX* pVtx = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vPos = _vec3(0.f, 0.5f, 0.f);
	pVtx[0].vTexUV = _vec2(0.f, 0.f);

	pVtx[1].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVtx[1].vTexUV = _vec2(1.f, 0.f);

	pVtx[2].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVtx[2].vTexUV = _vec2(1.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIdx = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0]._0 = 0;
	pIdx[0]._1 = 1;
	pIdx[0]._2 = 2;

	m_pIB->Unlock();

	for (int i = 0; i < 3; ++i)
		D3DXCreateSphere(m_pGraphicDev, RADIUS, 10.f, 10.f, &Sphere[i], nullptr);

	return S_OK;
}

void CTriTex::Render_Buffer()
{
	Set_TriTex(m_Line);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	CViBuffer::Render_Buffer();

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);

	//memcpy(&matTemp.m[3][0], m_Line.vLine_X[0], sizeof(_vec3));
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTemp);
	//Sphere[0]->DrawSubset(0);

	//memcpy(&matTemp.m[3][0], m_Line.vLine_Y[0], sizeof(_vec3));
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTemp);
	//Sphere[1]->DrawSubset(1);

	//memcpy(&matTemp.m[3][0], m_Line.vLine_Z[0], sizeof(_vec3));
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTemp);
	//Sphere[2]->DrawSubset(2);


}

CTriTex * CTriTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriTex* pInstance = new CTriTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		ERR_BOX("TriTex Create Failed!");
		Safe_Release(pInstance);
	}

	//pInstance->Set_Line(pLine);

	return pInstance;
}

CResourceses * CTriTex::Clone(void)
{
	return new CTriTex(*this);
}

void CTriTex::Free(void)
{
	if (bIsClone == FALSE)
	{
		for (int i = 0; i < 3; ++i)
			Sphere[i]->Release();
	}

	CViBuffer::Free();
}

