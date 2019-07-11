#include "AniControll.h"

USING(ENGINE)

CAniControll::CAniControll(LPD3DXANIMATIONCONTROLLER pAniCon)
	:	m_pAniCon(pAniCon),
	m_iCurrentTrack(0),
	m_iNewTrack(1),
	m_iOldAniIndex(99),
	m_fAccTime(0.f),
	m_dPeriod(0.0)
{
	m_pAniCon->AddRef();
}

CAniControll::CAniControll(const CAniControll & rhs)
	:	m_iCurrentTrack(rhs.m_iCurrentTrack),
	m_iNewTrack(rhs.m_iNewTrack),
	m_iOldAniIndex(rhs.m_iOldAniIndex),
	m_fAccTime(rhs.m_fAccTime),
	m_dPeriod(0.0)
{
	rhs.m_pAniCon->CloneAnimationController(rhs.m_pAniCon->GetMaxNumAnimationOutputs(),
		rhs.m_pAniCon->GetMaxNumAnimationSets(),
		rhs.m_pAniCon->GetMaxNumTracks(),
		rhs.m_pAniCon->GetMaxNumEvents(),
		&m_pAniCon);

}

CAniControll::~CAniControll()
{
}

_bool CAniControll::Is_AnimationSetEnd()
{
	D3DXTRACK_DESC tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCon->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);	//���� ������� Ʈ���� ������ �޾ƿ�

	if (tTrackInfo.Position >= m_dPeriod - 0.1)	//0.1�ʰ�  �����ִϸ��̼��� �����ð��� �ش�.
		return TRUE;

	return FALSE;
}

HRESULT CAniControll::Ready_AniControll()
{
	return S_OK;
}

void CAniControll::Set_AnimationSet(const _uint & iIndex)
{
	if (m_iOldAniIndex == iIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET pAS = nullptr;

	//�ִϸ��̼��� �ε����� �˻��ϴ� �Լ�
	m_pAniCon->GetAnimationSet(iIndex, &pAS);

	m_dPeriod = pAS->GetPeriod();

	//0��Ʈ���� ���� �ִϸ��̼��� ����
	m_pAniCon->SetTrackAnimationSet(m_iNewTrack, pAS);

	//�޽��ȿ� ���Ե� �̺�Ʈ �� �̺�Ʈ ���� ���� �̻���ϱ����� ȣ���ϴ� �Լ�
	//�޽����� �̺�Ʈ ���������� �ִϸ��̼� �°��� ������ �ȵǴ� ��찡 ����
	m_pAniCon->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCon->UnkeyAllTrackEvents(m_iNewTrack);

	//Ű ������ : ���ϸ��̼� �� ����߿� ���� ��� ������ �ǹ�

	//���� ����Ǵ� ������ �����Ѵ� �Լ�
	//3��° ���ڰ��� �ݵ�� ����Ÿ���̾���Ѵ�.
	m_pAniCon->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);

	//0.25��� ���͹� ������ ����Ǵ� Ű�������� �ӵ��� �����ϴ� �Լ�(2��° ���� : �ӵ��� ��� ���� ������ 1�� ����)
	m_pAniCon->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	//�ִϸ��̼� �� �ռ� �� �� ����ġ�� �������ִ� �Լ�
	m_pAniCon->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	//0�� Ʈ�� Ȱ��ȭ
	m_pAniCon->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCon->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCon->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCon->ResetTime();	//AdvanceTime �Լ��� ȣ��Ǹ� �ִϸ��̼��� ����Ǵ� ���� ���������� �ð��� �����ϴµ� �� �ð��� �ʱ�ȭ
	m_fAccTime = 0.f;

	//������ ��� ���̴� Ʈ������ ���ο� Ʈ������ ��ü�߱⋚���� 0.0���� �ִϸ��̼��� ����ǵ��� ����
	m_pAniCon->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIndex = iIndex;

	m_iCurrentTrack = m_iNewTrack;

}

void CAniControll::Play_AnimationSet(const _float & fTimeDelta)
{
	//�ִϸ��̼� ��� �Լ�, 2���� ���ڴ� �ִϸ��̼� ���ۿ� ���� ����Ʈ�� ���带 ����ϱ����� �ּ�
	m_pAniCon->AdvanceTime(fTimeDelta, nullptr);

	m_fAccTime += fTimeDelta;
}

CAniControll * CAniControll::Create(LPD3DXANIMATIONCONTROLLER pAniCon)
{
	CAniControll* pInstance = new CAniControll(pAniCon);

	if (FAILED(pInstance->Ready_AniControll()))
		Safe_Release(pInstance);
	
	return pInstance;
}

CAniControll * CAniControll::Create(const CAniControll & rhs)
{
	CAniControll* pInstance = new CAniControll(rhs);

	if (FAILED(pInstance->Ready_AniControll()))
		Safe_Release(pInstance);

	return pInstance;
}

void CAniControll::Free()
{
	Safe_Release(m_pAniCon);
}
