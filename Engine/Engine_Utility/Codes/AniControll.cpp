#include "AniControll.h"

USING(ENGINE)

CAniControll::CAniControll(LPD3DXANIMATIONCONTROLLER pAniCon)
	:	m_pAniCon(pAniCon),
	m_iCurrentTrack(0),
	m_iNewTrack(1),
	m_iOldAniIndex(99),
	m_AccTime(0.0),
	m_dPeriod(0.0),
	m_iIndex(0)
{
	m_pAniCon->AddRef();
}

CAniControll::CAniControll(const CAniControll & rhs)
	:	m_iCurrentTrack(rhs.m_iCurrentTrack),
	m_iNewTrack(rhs.m_iNewTrack),
	m_iOldAniIndex(rhs.m_iOldAniIndex),
	m_AccTime(rhs.m_AccTime),
	m_dPeriod(0.0),
	m_iIndex(0)
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

	m_pAniCon->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);	//현재 재생중인 트렉의 정보를 받아옴

	if (tTrackInfo.Position >= m_dPeriod - 0.1)	//0.1초간  다음애니메이션의 보간시간을 준다.
		return TRUE;

	return FALSE;
}

_double CAniControll::Get_Current_AnimationTime()
{
	return m_dPeriod;
}

HRESULT CAniControll::Ready_AniControll()
{
	return S_OK;
}

void CAniControll::Set_AnimationSet(const _uint & iIndex)
{
	if (m_iOldAniIndex == iIndex)
		return;

	m_iIndex = iIndex;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET pAS = nullptr;

	//애니메이션의 인덱스를 검색하는 함수
	m_pAniCon->GetAnimationSet(iIndex, &pAS);	//애니메이션 셋 포인터를 얻어옴

	m_dPeriod = pAS->GetPeriod();
	

	//0번트랙에 얻어온 애니메이션을 세팅
	m_pAniCon->SetTrackAnimationSet(m_iNewTrack, pAS);

	//메쉬안에 삽입된 이벤트 및 이벤트 관련 파일 미사용하기위해 호출하는 함수
	//메쉬안의 이벤트 정보로인해 애니메이션 셋간의 보간이 안되는 경우가 있음
	m_pAniCon->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCon->UnkeyAllTrackEvents(m_iNewTrack);

	//키 프레임 : 에니메이션 셋 재생중에 현재 재생 지점을 의미

	//현재 진행되는 동작을 끄거켜는 함수
	//3번째 인자값은 반드시 더블타입이어야한다.
	m_pAniCon->KeyTrackEnable(m_iCurrentTrack, FALSE, m_AccTime + 0.1);

	//0.25라는 인터벌 값동안 재생되던 키프레임의 속도를 제어하는 함수(2번째 인자 : 속도의 상수 값은 무조건 1로 가정)
	m_pAniCon->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_AccTime, 0.1, D3DXTRANSITION_LINEAR);

	//애니메이션 셋 합성 시 에 가중치를 지정해주는 함수
	m_pAniCon->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_AccTime, 0.1, D3DXTRANSITION_LINEAR);

	//0번 트랙 활성화
	m_pAniCon->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCon->KeyTrackSpeed(m_iNewTrack, 1.f, m_AccTime, 0.1, D3DXTRANSITION_LINEAR);
	m_pAniCon->KeyTrackWeight(m_iNewTrack, 0.9f, m_AccTime, 0.1, D3DXTRANSITION_LINEAR);

	m_pAniCon->ResetTime();	//AdvanceTime 함수가 호출되면 애니메이션이 재생되는 동안 내부적으로 시간을 누적하는데 그 시간값 초기화
	m_AccTime = 0.0;

	//기존의 재생 중이던 트랙에서 새로운 트랙으로 교체했기떄문에 0.0부터 애니메이션이 재생되도록 세팅
	m_pAniCon->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIndex = iIndex;

	m_iCurrentTrack = m_iNewTrack;

}

void CAniControll::Set_QuickSet(const _uint & iIndex)
{
	if (m_iOldAniIndex == iIndex)
		return;

	LPD3DXANIMATIONSET		pAS = NULL;

	m_iIndex = iIndex;

	m_pAniCon->GetAnimationSet(iIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName(); 애니메이션 셋을 문자열로 찾아 가져오는 함수

	m_dPeriod = pAS->GetPeriod();

	// 0번 트랙에 얻어온 애니메이션 셋을 올려놓는다(세팅한다)
	m_pAniCon->SetTrackAnimationSet(m_iCurrentTrack, pAS);


	// 0번 트랙을 활성화해라(재생하라는 명령이 아님, 트랙 활성화일 뿐)
	m_pAniCon->SetTrackEnable(m_iCurrentTrack, TRUE);

	m_iOldAniIndex = iIndex;
}

void CAniControll::Play_AnimationSet(const _double & TimeDelta)
{
	//애니메이션 재생 함수, 2번쨰 인자는 애니메이션 동작에 따른 이펙트나 사운드를 재생하기위한 주소
	m_pAniCon->AdvanceTime(TimeDelta, nullptr);

	m_AccTime += TimeDelta;
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
