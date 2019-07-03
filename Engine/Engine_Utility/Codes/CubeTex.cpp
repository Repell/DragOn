#include "CubeTex.h"

USING(ENGINE)

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CViBuffer(pGraphicDev)
{
}

CCubeTex::~CCubeTex()
{

}

HRESULT CCubeTex::Ready_Buffer()
{

	//�ε��� ���� ���
	m_dwVtxSize = sizeof(VTX_CUBE);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = FVF_CUBE;
	m_dwTriCnt = 12;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;


	FAILED_CHECK_RETURN(CViBuffer::Ready_Buffer(), E_FAIL);

	VTX_CUBE* pVtxCube = nullptr;

	//ť�� �����ͷ� ���ؽ��� ü���.
	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	//���� ť���� ���ؽ���
	//ť���� ���� �����̽��� �������� ��ġ�� �����Ѵ�.
	pVtxCube[0].vPos = { -1.f, 1.f, -1.f };
	pVtxCube[0].vTex = pVtxCube[0].vPos;

	pVtxCube[1].vPos = { 1.f, 1.f, -1.f };
	pVtxCube[1].vTex = pVtxCube[1].vPos;

	pVtxCube[2].vPos = { 1.f, -1.f, -1.f };
	pVtxCube[2].vTex = pVtxCube[2].vPos;

	pVtxCube[3].vPos = { -1.f, -1.f, -1.f };
	pVtxCube[3].vTex = pVtxCube[3].vPos;

	pVtxCube[4].vPos = { -1.f, 1.f, 1.f };
	pVtxCube[4].vTex = pVtxCube[4].vPos;

	pVtxCube[5].vPos = { 1.f, 1.f, 1.f };
	pVtxCube[5].vTex = pVtxCube[5].vPos;

	pVtxCube[6].vPos = { 1.f, -1.f, 1.f };
	pVtxCube[6].vTex = pVtxCube[6].vPos;

	pVtxCube[7].vPos = { -1.f, -1.f, 1.f };
	pVtxCube[7].vTex = pVtxCube[7].vPos;

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//ť���� �ﰢ������ �����Ѵ�.
	//������ ����
	pIndex[0] = { 1, 5, 6 };
	pIndex[1] = { 1, 6, 2 };
	//���� ����
	pIndex[2] = { 4, 0, 3 };
	pIndex[3] = { 4, 3, 7 };
	//���
	pIndex[4] = { 4, 5, 1 };
	pIndex[5] = { 4, 1, 0 };
	//�ϴ�
	pIndex[6] = { 3, 2, 6 };
	pIndex[7] = { 3, 6, 7 };
	//����
	pIndex[8] = { 0, 1, 2 };
	pIndex[9] = { 0, 2, 3 };
	//�ĸ�
	pIndex[10] = { 7, 5, 4 };
	pIndex[11] = { 7, 6, 5 };

	m_pIB->Unlock();

	return S_OK;
}

void CCubeTex::Render_Buffer()
{
	CViBuffer::Render_Buffer();
}

void CCubeTex::Free()
{
	CViBuffer::Free();
}

CCubeTex * CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeTex* pInstance = new CCubeTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CResourceses * CCubeTex::Clone(void)
{
	return new CCubeTex(*this);
}
