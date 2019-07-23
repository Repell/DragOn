#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Effect_Tex.h"
#include "Sword.h"

namespace ENGINE
{
	class CRenderer;
	class CDynamicMesh;
	class CTransform;
	class CGameObject;
	class CNaviMesh;
	class CCollider;
	class CSphereColl;
}

class CPlayer : public ENGINE::CGameObject
{
public:
	enum eANIMATESTATE
	{
		ANI_DASH_W,
		ANI_DASH_S,
		ANI_DASH_A,
		ANI_DASH_D,
		ANI_JUMP,
		ANI_END
	};


private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Render_Set();
	void Render_ReSet();

private:
	//Key Input, Camera, NaviMesh
	_bool Key_check(const _double& TimeDelta);
	//void Jump_Check(const _double& TimeDelta);
	void Jump_Func(const _double& TimeDelta);
	_bool Check_EnemyColl();
	//void Key_Old(_float fTimeDelta);

private:
	VOID Animate_FSM(_uint iAniState);
	typedef VOID(CPlayer::*ANIFSM)();
	ANIFSM AniStateFunc;

	_uint m_iCurAniState;
	_uint m_iPreAniState;

	//eANIMATESTATE m_eCurAniState;
	//eANIMATESTATE m_ePreAniState;

private:
	void UI_Sample();

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	//ENGINE::CCollider*			m_pCollider;
	ENGINE::CSphereColl*	m_pSphereColl;
	CSword*			m_pSword;

private:
	//Jump
	_bool bJump;
	_float fPosY;
	_double JumpTime;
			
	//Animated
	_bool m_bAnimate;
	_bool m_bDash;
	_double m_DashDelay;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;



};

#endif // Player_h__
