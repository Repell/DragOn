#include "Transform.h"

USING(ENGINE)

CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f),
	m_vAngle(0.f, 0.f, 0.f),
	m_vLook(0.f, 0.f, 0.f),
	m_vDir(0.f, 0.f, 0.f),
	m_fJump(0.f),
	bCamTarget(true)
{
	
}

CTransform::~CTransform()
{
}

HRESULT CTransform::Ready_Trasnform(_vec3 vLook)
{
	m_vLook = vLook;
	D3DXMatrixIdentity(&m_matWorld);

	for (_ulong i = 0; i < 4; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vInfo[INFO_POS].y += m_fJump;

	for (_ulong i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// Scale 
	for (_ulong i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
		//m_vSclae ù �ּҸ� �ְ� �ű⿡ ������ i��(4byte)��ŭ �ּҸ� �����ذ��鼭 ���� ����
		//m_vScale�� 12byte �����̱⿡ 4byte�� ��ȯ�ϸ� x, y, z���� ��ȯ���ش�!
	}

	//Rotation X, Y, Z
	_matrix matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], D3DXToRadian(m_vAngle.x));
	D3DXMatrixRotationY(&matRot[ROT_Y], D3DXToRadian(m_vAngle.y));
	D3DXMatrixRotationZ(&matRot[ROT_Z], D3DXToRadian(m_vAngle.z));
	
	for (_ulong i = 0; i < INFO_POS; ++i)
	{
		for (_ulong j = 0; j < ROT_END; ++j)
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);	
	}
	
	//Translation
	for (_ulong i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	D3DXVec3TransformNormal(&m_vDir, &m_vLook, &m_matWorld);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return 0;
}

void CTransform::Late_Update_Component()
{
	if (m_vAngle.x >= 360.f || m_vAngle.x <= -360.f)
		m_vAngle.x = 0.f;
	else if (m_vAngle.y >= 360.f || m_vAngle.y <= -360.f)
		m_vAngle.y = 0.f;
	else if (m_vAngle.z >= 360.f || m_vAngle.z <= -360.f)
		m_vAngle.z = 0.f;

}

void CTransform::Chase_Target(const _vec3 * pTarget, const _float & fSpeed)
{
	_vec3 vNewDir = *pTarget - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vNewDir, &vNewDir) * fSpeed;

	_matrix matRot = *Compute_LockTarget(pTarget);
	_matrix matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale *  matRot * matTrans;
}

_matrix * CTransform::Compute_LockTarget(const _vec3 * pTargetPos)
{
	//��ü�� ����� �ٶ󺸴� ���� 
	_vec3 vLookDir = *pTargetPos - m_vInfo[INFO_POS];
	//������ �̿��� ������ ȸ���� ����(��ü�� ��ġ�� ����� �ٶ󺸴� ������ ����)
	_vec3 vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_LOOK], &vLookDir);
	//��(y��) ����, ȸ������ ���� ���� ����
	_vec3 vUp;
	_matrix matRot;

	///Old Style
	////�ٶ󺸴� ���� ����ȭ
	//D3DXVec3Normalize(&vLookDir, &vLookDir);
	////�� ���� ����ȭ(��ü�� �� ����)
	//D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]);
	////����ȭ�� �� ���͸� ������ cos@ ����
	//float fCos = D3DXVec3Dot(&vLookDir, &vUp);
	////cos@�� acosf�� ���Ȱ����� ����
	//float fAngle = acosf(fCos);
	////�����̼� Axis �� ȸ���� ����(������ ���, ȸ����, ����)
	//D3DXMatrixRotationAxis(&matRot, &vAxis, fAngle);
	
	///New Style
	return D3DXMatrixRotationAxis(&matRot, &vAxis,
		acosf(D3DXVec3Dot(D3DXVec3Normalize(&vLookDir, &vLookDir),
						D3DXVec3Normalize(&vUp, &m_vInfo[INFO_LOOK]))));
}

void CTransform::Move_TargetPos(const _vec3 * pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3 vNewDir = *pTargetPos - m_vInfo[INFO_POS];
	_float fDist = D3DXVec3Length(&vNewDir);

	if(fDist > 1.f)
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vNewDir, &vNewDir) * fSpeed * fTimeDelta;
}

CTransform * CTransform::Create(_vec3& vLook)
{
	CTransform* pInstance = new CTransform;
	if (FAILED(pInstance->Ready_Trasnform(vLook)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTransform::Free()
{

}

