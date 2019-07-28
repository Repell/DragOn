#ifndef AniControll_h__
#define AniControll_h__

#include "Engine_Define.h"
#include "Base.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CAniControll :
	public CBase
{
private:
	explicit CAniControll(LPD3DXANIMATIONCONTROLLER pAniCon);
	explicit CAniControll(const CAniControll& rhs);
	virtual ~CAniControll();

public:
	LPD3DXANIMATIONCONTROLLER Get_AniControll() { return m_pAniCon; }
	_bool Is_AnimationSetEnd();
	_double Get_Current_AnimationTime();

public:
	HRESULT Ready_AniControll();
	void Set_AnimationSet(const _uint& iIndex);
	void Set_QuickSet(const _uint& iIndex);
	void Play_AnimationSet(const _double& TimeDelta);

private:
	LPD3DXANIMATIONCONTROLLER m_pAniCon;
	_uint	m_iCurrentTrack;
	_uint m_iNewTrack;
	_uint m_iOldAniIndex;
	_uint m_iIndex;
	
	_double m_AccTime;
	_double m_dPeriod;

public:
	static CAniControll* Create(LPD3DXANIMATIONCONTROLLER pAniCon);
	static CAniControll* Create(const CAniControll& rhs);
	virtual void Free();

};

NS_END	

#endif // AniControll_h__
