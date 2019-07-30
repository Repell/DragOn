#include "Transform.h"

USING(ENGINE)

#define _ANGLE 60.f

CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f),
	m_vAngle(0.f, 0.f, 0.f),
	m_vLook(0.f, 0.f, 0.f),
	m_vDir(0.f, 0.f, 0.f),
	m_fJump(0.f),
	bCamTarget(true)
{
	m_bFront = FALSE;
	bRotate = FALSE;
	m_eCurDir = DIR_UP;
	m_bDead = FALSE;
	m_fRotate = 0;
}

CTransform::~CTransform()
{
}

_vec3 CTransform::Get_vLookDir()
{
	_vec3 vLookPos = m_vInfo[ENGINE::INFO_LOOK];;
	vLookPos.y = 0.f;
	
	return *D3DXVec3Normalize(&vLookPos, &vLookPos);
}

_vec3 CTransform::Get_vInfoPos(ENGINE::INFO eInfo)
{
	_vec3 vPos;

	switch (eInfo)
	{
	case ENGINE::INFO_RIGHT:
		vPos = m_vInfo[ENGINE::INFO_RIGHT];
		break;
	case ENGINE::INFO_UP:
		vPos = m_vInfo[ENGINE::INFO_UP];
		break;
	case ENGINE::INFO_LOOK:
		vPos = m_vInfo[ENGINE::INFO_LOOK];
		break;
	case ENGINE::INFO_POS:
		vPos = m_vInfo[ENGINE::INFO_POS];
		break;
	}

	return vPos;
}

HRESULT CTransform::Ready_Trasnform(_vec3 vLook)
{
	m_vLook = vLook;
	D3DXMatrixIdentity(&m_matWorld);

	for (_ulong i = 0; i < 4; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	//memcpy(&m_matWorld.m[2][0], m_vLook, sizeof(_vec3));
	//m_vInfo[ENGINE::INFO_LOOK] = m_vLook;

	return S_OK;
}

_int CTransform::Update_Component(const _double& TimeDelta)
{
	m_TimeDelta = TimeDelta;
	//Rotation_AngleY(TimeDelta);

	D3DXMatrixIdentity(&m_matWorld);
	m_vInfo[INFO_POS].y += m_fJump;

	for (_ulong i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	//m_vInfo[ENGINE::INFO_LOOK] = m_vLook;

	// Scale 
	for (_ulong i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
		//m_vSclae 첫 주소를 주고 거기에 증가한 i값(4byte)만큼 주소를 변경해가면서 값을 변경
		//m_vScale이 12byte 벡터이기에 4byte로 변환하며 x, y, z값을 변환해준다!
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

	D3DXVec3TransformNormal(&m_vDir, &m_vInfo[INFO_LOOK], &m_matWorld);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return 0;
}

void CTransform::Late_Update_Component()
{
	if (m_vAngle.x > 360.f || m_vAngle.x < -360.f)
		m_vAngle.x = 0.f;
	else if (m_vAngle.y > 360.f || m_vAngle.y < -360.f)
		m_vAngle.y = 0.f;
	else if (m_vAngle.z > 360.f || m_vAngle.z < -360.f)
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
	//객체가 대상을 바라보는 방향 
	_vec3 vLookDir = *pTargetPos - m_vInfo[INFO_POS];
	//외적을 이용한 임의의 회전축 생성(객체의 위치와 대상을 바라보는 방향을 외적)
	_vec3 vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_LOOK], &vLookDir);
	//업(y축) 벡터, 회전축을 받을 벡터 생성
	_vec3 vUp;
	_matrix matRot;

	///Old Style
	////바라보는 방향 정규화
	//D3DXVec3Normalize(&vLookDir, &vLookDir);
	////업 벡터 정규화(객체의 업 벡터)
	//D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]);
	////정규화한 두 벡터를 내적해 cos@ 생성
	//float fCos = D3DXVec3Dot(&vLookDir, &vUp);
	////cos@를 acosf로 라디안값으로 변경
	//float fAngle = acosf(fCos);
	////로테이션 Axis 로 회전축 적용(적용할 행렬, 회전축, 각도)
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

	if (fDist > 1.f)
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vNewDir, &vNewDir) * fSpeed * fTimeDelta;
}

_float CTransform::Fix_TargetLookAngleY(CTransform * pTarget, _float fSearchDist)
{
	_vec3 vNewDir = pTarget->m_vInfo[INFO_POS] - m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vNewDir);

	if (fSearchDist < fDistance)
		return fDistance;

	D3DXVec3Normalize(&vNewDir, &vNewDir);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	_float fRad = D3DXVec3Dot(&m_vLook, &vNewDir);
	//m_vAngle.y = D3DXToDegree(acosf(fRad));
	//if (pTarget->m_vInfo[ENGINE::INFO_POS].x > m_vInfo[ENGINE::INFO_POS].x)
	//	m_vAngle.y *= -1.f;
	//////////////////////////////////////////////

	m_fRotate = D3DXToDegree(acosf(fRad));
	if (pTarget->m_vInfo[ENGINE::INFO_POS].x > m_vInfo[ENGINE::INFO_POS].x)
		m_fRotate *= -1.f;

	_float fRotY = (m_fRotate - m_vAngle.y) * 1.f * m_TimeDelta;

	if (m_vAngle.y > m_fRotate - 1.5f && m_vAngle.y < m_fRotate + 1.5f)
		return fDistance;

	m_vAngle.y += fRotY;
	
	//////////////////////////////////////////////
	return fDistance;
}

_float CTransform::Fix_TargetLook(CTransform * pTarget, _float fSearchDist)
{
	_vec3 vNewDir = pTarget->m_vInfo[INFO_POS] - m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vNewDir);

	if (fSearchDist < fDistance)
		return fDistance;

	D3DXVec3Normalize(&vNewDir, &vNewDir);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	_float fRad = D3DXVec3Dot(&m_vLook, &vNewDir);

	m_vAngle.y = D3DXToDegree(acosf(fRad));

	if (pTarget->m_vInfo[ENGINE::INFO_POS].x > m_vInfo[ENGINE::INFO_POS].x)
		m_vAngle.y *= -1.f;

	if (fSearchDist < fDistance)
		return fDistance;
}

void CTransform::Stalk_Target(CTransform * pTransform, const _double& fTime, const _float fSpeed)
{
	_vec3 vLookDir = pTransform->m_vInfo[INFO_POS] - m_vInfo[INFO_POS];
	vLookDir.y = 0.f;
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vLookDir, &vLookDir) * fSpeed * fTime;
}

_vec3 CTransform::Stalk_TargetDir(CTransform * pTransform, const _double & fTime, const _float fSpeed)
{
	_vec3 vLookDir = pTransform->m_vInfo[INFO_POS] - m_vInfo[INFO_POS];
	return *D3DXVec3Normalize(&vLookDir, &vLookDir) * fSpeed * fTime;
}

_float CTransform::Get_TargetDistance(CTransform * pTarget)
{
	_vec3 vNewDir = pTarget->m_vInfo[INFO_POS] - m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vNewDir);

	return fDistance;
}

_vec3 CTransform::Get_TargetReverseDir(CTransform * pTarget)
{
	_vec3 vRevDir = m_vInfo[INFO_POS] - pTarget->m_vInfo[INFO_POS];
	
	return *D3DXVec3Normalize(&vRevDir, &vRevDir);
}

_bool CTransform::Get_Dead(_bool bDead)
{
	if (bDead)
		m_bDead = TRUE;

	return m_bDead;
}

_vec3 CTransform::Get_NewPlayerPos(_float fDist)
{
	_vec3 vDir = *D3DXVec3Normalize(&vDir, &m_vInfo[ENGINE::INFO_LOOK]);
	_vec3 vNewPos = m_vInfo[ENGINE::INFO_POS] + (vDir * fDist);

	return vNewPos;
}

_bool CTransform::Check_TargetFront()
{
	if (m_vAngle.y > m_fRotate - 4.f && m_vAngle.y < m_fRotate + 4.f)
		m_bFront = TRUE;
	else
		m_bFront = FALSE;

	return m_bFront;
}

void CTransform::Rotation_AngleY(const _double & TimeDelta)
{
	
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

