#include "Calculator.h"
#include "TerrainTex.h"

USING(ENGINE)

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CCalculator::~CCalculator()
{

}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(const _vec3 * pPos, const _vec3 * pTerrainVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ)
{
	//�÷��̾� ���� ��ġ �������� 0�� ���ؽ��� �ش��ϴ� �ε����� ã�Ƴ�
	_ulong	dwIndex = _ulong(pPos->z / 1.f) * dwCntX + _ulong(pPos->x / 1.f);

	if (dwIndex < 0 || dwIndex > dwCntX * dwCntZ)
		return 0.f;

	//�÷��̾� ������ġ�� �簢���� ���� �ﰢ������ �Ʒ��� �ﰢ������ Ȯ��
	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / 1.f;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / 1.f;

	//�÷��̾� ��ġ �������� �簢���� ���ؽ����� ã�Ƴ�
	_float	fHeight[4] = { pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y };

	// �� ���
	if (fRatioX > fRatioZ)
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	// �Ʒ� ���
	else
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
}

_vec3 CCalculator::Picking_Terrain(HWND hWnd, const CTerrainTex * pTerrainBuffer, const CTransform * pTransCom)
{
	POINT ptMouse = {};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3	vMousePos;
	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	//�������� �� �����̽��� ��ȯ
	_matrix matProj, matView;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);	//������ ����� = �� ���
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);	//��ġ ���ͷ� ��ȯ

	//�佺���̽� -> ���彺���̽�
	_vec3 vRayDir, vRayPos;
	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);	//����� -> �������
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView); //���� ���ͷ� ��ȯ
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);	//��ġ ���ͷ� ��ȯ

	_ulong dwVtxCntX = TERRAIN_VTX_X;
	_ulong dwVtxCntZ = TERRAIN_VTX_Z;

	const _vec3* pTerrainVtx = pTerrainBuffer->Get_vVtxPos();
	//pTerrainBuffer->Get_VtxPos(pTerrainVtx);

	_ulong dwVtxIdx[3];
	_float fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntX - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntZ - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			//������ ��
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]], 
				&pTerrainVtx[dwVtxIdx[0]], 
				&pTerrainVtx[dwVtxIdx[2]], 
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);

			}
			//���� �Ʒ�
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);

			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator* pInstance = new CCalculator(pGraphicDev);
	if (FAILED(pInstance->Ready_Calculator()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCalculator::Free()
{
	Safe_Release(m_pGraphicDev);
}
