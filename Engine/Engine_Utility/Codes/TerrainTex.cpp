#include "TerrainTex.h"

USING(ENGINE)

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CViBuffer(pGraphicDev), m_vPos(nullptr), m_bClone(false)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex & rhs)
	: CViBuffer(rhs), m_vPos(rhs.m_vPos), m_bClone(true)
{
}

CTerrainTex::~CTerrainTex()
{
	
}

HRESULT CTerrainTex::Ready_Buffer(WORD wCntX, WORD wCntZ, WORD wItv, WORD wDetail)
{
	//정점 개수로 진행
	m_dwVtxCnt = wCntX * wCntZ;
	m_dwTriCnt = (wCntX - 1) * (wCntZ - 1) * 2;
	m_dwVtxSize = sizeof(VTX_TEX);
	m_dwVtxFVF = FVF_TEX;
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_vPos = new _vec3[m_dwVtxCnt];

	FAILED_CHECK_RETURN(CViBuffer::Ready_Buffer(), E_FAIL);

	// 
	DWORD* pPixel = LoadHeightMap();
	NULL_CHECK_RETURN(pPixel, E_FAIL);

	// 버텍스 버퍼
	VTX_TEX* pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong wIndex = 0;

	for (_ulong i = 0; i < wCntZ; ++i)
	{
		for (_ulong j = 0; j < wCntX; ++j)
		{
			wIndex = j + wCntX * i;

			pVtxTex[wIndex].vPos.x = (float)j * wItv;
			//pVtxTex[wIndex].vPos.y = 0.f;
			pVtxTex[wIndex].vPos.y = (pPixel[wIndex] & 0x000000ff) / 50.f;
			pVtxTex[wIndex].vPos.z = (float)i * wItv;

			m_vPos[wIndex] = pVtxTex[wIndex].vPos;

			pVtxTex[wIndex].vNormal = {0.f, 0.f, 0.f};

			pVtxTex[wIndex].vTexUV.x = pVtxTex[wIndex].vPos.x / ((wCntX - 1) * wItv) *wDetail;
			pVtxTex[wIndex].vTexUV.y = 1.f - pVtxTex[wIndex].vPos.z / ((wCntZ - 1) * wItv) *wDetail;
		}
	}

	///m_pVB->Unlock();

	Set_OriginVtx(pVtxTex, m_dwVtxCnt);

	Safe_Delete_Array(pPixel);

	// 인덱스 버퍼
	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong iTriIndex = 0;

	wIndex = 0;

	for (_ulong i = 0; i < wCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < wCntX - 1; ++j)
		{
			wIndex = j + wCntX * i;

			/////////////// 위 삼각형
			pIndex[iTriIndex]._0 = wIndex + wCntX;
			pIndex[iTriIndex]._1 = wIndex + wCntX + 1;
			pIndex[iTriIndex]._2 = wIndex + 1;

			_vec3 p1 = pVtxTex[pIndex[iTriIndex]._0].vPos - pVtxTex[pIndex[iTriIndex]._1].vPos;
			_vec3 p2 = pVtxTex[pIndex[iTriIndex]._1].vPos - pVtxTex[pIndex[iTriIndex]._2].vPos;
			_vec3 pNormal = *D3DXVec3Cross(&pNormal, &p1, &p2);
			D3DXVec3Normalize(&pNormal, &pNormal);
			pVtxTex[pIndex[iTriIndex]._0].vNormal = pNormal;
			pVtxTex[pIndex[iTriIndex]._1].vNormal = pNormal;
			pVtxTex[pIndex[iTriIndex]._2].vNormal = pNormal;

			++iTriIndex;
			////////////// 아래 삼각형
			pIndex[iTriIndex]._0 = wIndex + wCntX;
			pIndex[iTriIndex]._1 = wIndex + 1;
			pIndex[iTriIndex]._2 = wIndex;

			p1 = pVtxTex[pIndex[iTriIndex]._0].vPos - pVtxTex[pIndex[iTriIndex]._1].vPos;
			p2 = pVtxTex[pIndex[iTriIndex]._1].vPos - pVtxTex[pIndex[iTriIndex]._2].vPos;
			pNormal = *D3DXVec3Cross(&pNormal, &p1, &p2);
			D3DXVec3Normalize(&pNormal, &pNormal);
			pVtxTex[pIndex[iTriIndex]._0].vNormal = pNormal;
			pVtxTex[pIndex[iTriIndex]._1].vNormal = pNormal;
			pVtxTex[pIndex[iTriIndex]._2].vNormal = pNormal;

			++iTriIndex;

		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CTerrainTex::Render_Buffer()
{
	CViBuffer::Render_Buffer();
}

void CTerrainTex::Copy_Index(INDEX32 * pIndex, const _ulong & dwTriCnt)
{
	INDEX32*		pOriginIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriginIndex, 0);

	memcpy(pOriginIndex, pIndex, sizeof(INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

DWORD * CTerrainTex::LoadHeightMap()
{
	HANDLE hFile = nullptr;
	DWORD dwByte = 0;

	hFile = CreateFile(L"../../Client/Bin/Resources/Texture/Terrain/Height0.bmp",
		GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return nullptr;

	//한개의 BMP 파일 대한 파일 및 이미지 정보
	BITMAPFILEHEADER tBmpHead;		//Bitmap 파일 정보
	BITMAPINFOHEADER tBmpInfo;		//Bitmap 이미지 정보

	ReadFile(hFile, &tBmpHead, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &tBmpInfo, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	//한개의 BMP 파일에 대한 각 픽셀의 정보(배열 = 가로 픽셀 수 * 세로 픽셀 수)를 저장할 DWORD 배열
	DWORD* pPixel = new DWORD[tBmpInfo.biWidth * tBmpInfo.biHeight];

	//파일로부터 각 픽셀의 위치별 DATA를 받아옴
	ReadFile(hFile, pPixel, sizeof(DWORD) * tBmpInfo.biWidth * tBmpInfo.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	return pPixel;
}

void CTerrainTex::Free()
{
	if(!m_bClone)
		Safe_Delete_Array(m_vPos);

	CViBuffer::Free();
}

CTerrainTex * CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const WORD & wCntX, const WORD & wCntY, const WORD & wItv, const WORD wDetail)
{
	CTerrainTex* pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(wCntX, wCntY, wItv, wDetail)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CResourceses * CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

