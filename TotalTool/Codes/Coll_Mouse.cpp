#include "stdafx.h"
#include "Coll_Mouse.h"


CColl_Mouse::CColl_Mouse()
{
}

CColl_Mouse::~CColl_Mouse()
{
	Free();
}

bool CColl_Mouse::PickTerrain(D3DXVECTOR3 * pOut, const ENGINE::VTX_TEX * pTerrerainVtx)
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	Translation_ViewSpace();
	Translation_WorldSpace();
	Translation_LocalSapce(&matWorld);

	WORD wIndex = 0;
	WORD w[4] = {};

	float fU = 0.f, fV = 0.f, fDist = 0.f;

	for (int iZ = 0; iZ < TERRAIN_VTX_Z - 1; ++iZ)
	{
		for (int iX = 0; iX < TERRAIN_VTX_X - 1; ++iX)
		{
			wIndex = iX + TERRAIN_VTX_X * iZ;
			w[0] = wIndex + TERRAIN_VTX_X;
			w[1] = wIndex + TERRAIN_VTX_X + 1;
			w[2] = wIndex + 1;
			w[3] = wIndex;

			//RightTop Polygon(Triangle)
			if (D3DXIntersectTri(&pTerrerainVtx[w[0]].vPos, &pTerrerainVtx[w[1]].vPos, &pTerrerainVtx[w[2]].vPos,
				&m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = pTerrerainVtx[w[0]].vPos +
					fU *	(pTerrerainVtx[w[1]].vPos - pTerrerainVtx[w[0]].vPos) +
					fV * (pTerrerainVtx[w[2]].vPos - pTerrerainVtx[w[0]].vPos);

				return true;
			}
			//LeftDown Polygon(Triangle)
			if (D3DXIntersectTri(&pTerrerainVtx[w[0]].vPos, &pTerrerainVtx[w[2]].vPos, &pTerrerainVtx[w[3]].vPos,
				&m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = pTerrerainVtx[w[0]].vPos +
					fU *	(pTerrerainVtx[w[2]].vPos - pTerrerainVtx[w[0]].vPos) +
					fV * (pTerrerainVtx[w[3]].vPos - pTerrerainVtx[w[0]].vPos);

				return true;
			}
		}
	}

	return false;
}

bool CColl_Mouse::PickObject(D3DXVECTOR3 * pOut, const ENGINE::VTX_TEX * pTerrerainVtx, const D3DXMATRIX * pMatWorld)
{

	Translation_ViewSpace();
	Translation_WorldSpace();
	Translation_LocalSapce(pMatWorld);

	float fU = 0.f, fV = 0.f, fDist = 0.f;


	//RightTop Polygon(Triangle)
	if (D3DXIntersectTri(&pTerrerainVtx[0].vPos, &pTerrerainVtx[1].vPos, &pTerrerainVtx[2].vPos,
		&m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
	{
		*pOut = pTerrerainVtx[0].vPos +
			fU *	(pTerrerainVtx[1].vPos - pTerrerainVtx[0].vPos) +
			fV * (pTerrerainVtx[2].vPos - pTerrerainVtx[0].vPos);

		D3DXVec3TransformCoord(pOut, pOut, pMatWorld);

		return true;
	}

	//LeftDown Polygon(Triangle)
	if (D3DXIntersectTri(&pTerrerainVtx[0].vPos, &pTerrerainVtx[2].vPos, &pTerrerainVtx[3].vPos,
		&m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
	{
		*pOut = pTerrerainVtx[0].vPos +
			fU *	(pTerrerainVtx[2].vPos - pTerrerainVtx[0].vPos) +
			fV * (pTerrerainVtx[3].vPos - pTerrerainVtx[0].vPos);

		D3DXVec3TransformCoord(pOut, pOut, pMatWorld);
		return true;
	}

	return false;
}

HRESULT CColl_Mouse::initialize()
{

	return S_OK;
}

void CColl_Mouse::Free()
{

}

void CColl_Mouse::Translation_ViewSpace()
{
	//POINT pt = Get_MousePos();
	//D3DXVECTOR3 vMice = {};

	////ViewPort -> Projection Space
	_vec3 vMice = {};
	vMice.x = float(CValueMgr::vMouse.x) / (WINCX >> 1) - 1.f;	// >>1  :: Mean == * 0.5f;
	vMice.y = float(-CValueMgr::vMouse.y) / (WINCY >> 1) + 1.f;
	vMice.z = 1.f;


	//Projection Space-> View Space
	D3DXMATRIX matProj = CValueMgr::matProj;
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMice, &vMice, &matProj);

	//Lay StartPos and Lay Direction in ViewSpace
	m_vPivotPos = { 0.f, 0.f, 0.f };
	m_vRayDir = vMice - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

}

void CColl_Mouse::Translation_WorldSpace()
{
	//View Space -> World Space
	D3DXMATRIX matView = CValueMgr::matView;
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CColl_Mouse::Translation_LocalSapce(const D3DXMATRIX * pMatWorld)
{
	//World Space -> Local Space
	D3DXMATRIX matWorld = *pMatWorld;
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorld);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

CColl_Mouse * CColl_Mouse::Create()
{
	CColl_Mouse* pInstance = new CColl_Mouse;

	if (FAILED(pInstance->initialize()))
	{
		ENGINE::Safe_Delete(pInstance);
		return nullptr;
	}

	return pInstance;
}
