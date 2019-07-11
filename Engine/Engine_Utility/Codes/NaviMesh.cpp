#include "NaviMesh.h"

USING(ENGINE)

CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pDevice)
	:CMesh(pDevice),
	m_dwCurrentIdx(0)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh & rhs)
	: CMesh(rhs),
	m_dwCurrentIdx(rhs.m_dwCurrentIdx),
	m_vecCell(rhs.m_vecCell)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();

}

CNaviMesh::~CNaviMesh()
{
}

HRESULT CNaviMesh::Ready_NaviMesh()
{
	m_vecCell.reserve(64);

	CCell* pCell = nullptr;
	
	for (_uint Z = 0; Z < 8; ++Z)
	{
		for (_uint X = 0; X < 8; ++X)
		{
			// 0
			pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(X * 2.f, Z +1, (Z + 1) * 2.f), &_vec3((X + 1) * 2.f, Z, Z * 2.f), &_vec3(X * 2.f, Z, Z * 2.f));
			NULL_CHECK_RETURN(pCell, E_FAIL);
			m_vecCell.emplace_back(pCell);

			pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(X * 2.f, Z +1, (Z + 1) * 2.f), &_vec3((X + 1) * 2.f, Z + 1, (Z +1) * 2.f), &_vec3((X+1) * 2.f, Z, Z * 2.f));
			NULL_CHECK_RETURN(pCell, E_FAIL);
			m_vecCell.emplace_back(pCell);

		}
	}
	
	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

void CNaviMesh::Render_NaviMesh()
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();
}

_vec3 CNaviMesh::MoveOn_NaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir)
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwCurrentIdx]->Compare(&vEndPos, &m_dwCurrentIdx))
	{
		vEndPos.y = Compute_OnTerrain(pTargetPos, &m_dwCurrentIdx);
		return vEndPos;
	}

	else if (CCell::COMPARE_STOP == m_vecCell[m_dwCurrentIdx]->Compare(&vEndPos, &m_dwCurrentIdx))
	{
		vEndPos.y = Compute_OnTerrain(pTargetPos, &m_dwCurrentIdx);
		return *pTargetPos;
	}

	return _vec3(0.f, 0.f, 0.f);
}

HRESULT CNaviMesh::Link_Cell()
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
				continue;

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
					m_vecCell[i]->Get_Point(CCell::POINT_B),
					m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
					m_vecCell[i]->Get_Point(CCell::POINT_C),
					m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
					m_vecCell[i]->Get_Point(CCell::POINT_A),
					m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}

		}
	}

	return S_OK;
}

_float CNaviMesh::Compute_OnTerrain(const _vec3 * pPos, _ulong * pCellIndex)
{
	D3DXPLANE d3Plane = {};

	D3DXPlaneFromPoints(&d3Plane,
		m_vecCell[(*pCellIndex)]->Get_Point(CCell::POINT_A),
		m_vecCell[(*pCellIndex)]->Get_Point(CCell::POINT_B),
		m_vecCell[(*pCellIndex)]->Get_Point(CCell::POINT_C));

	return (-d3Plane.a * pPos->x - d3Plane.c * pPos->z - d3Plane.d) / d3Plane.b;
}

CNaviMesh * CNaviMesh::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CNaviMesh*		pInstance = new CNaviMesh(pDevice);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CResourceses * CNaviMesh::Clone()
{
	return new CNaviMesh(*this);
}

void CNaviMesh::Free()
{
	CMesh::Free();

	for (auto iter : m_vecCell)
		Safe_Release(iter);

	m_vecCell.clear();
}
