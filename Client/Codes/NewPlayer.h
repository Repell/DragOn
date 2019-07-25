#ifndef NewPlayer_h__
#define NewPlayer_h__

#include "GameObject.h"
#include "Sword.h"

namespace ENGINE
{
	class CGameObject;
	class CTransform;
	class CNaviMesh;
	class CDynamicMesh;
	class CSphereColl;
	class CRenderer;
}

class CNewPlayer : public ENGINE::CGameObject
{
private:
	explicit CNewPlayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CNewPlayer();

public:	//OverRide Func
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	//Object Init
	HRESULT Add_Component();

private:
	//Render D3DRS
	void Render_Set();
	void Render_ReSet();
private:
	//Mouse Func
	_vec3 MouseFunc();
	//Key Check Func
	_bool Key_Check_Func(const _double& TimeDelta);

private:
	//Collision Check
	_bool Check_DirectionCollision(_vec3* vRevDir);
private:
	//Animate Func
	VOID Animate_FSM(_uint iAniState);
	VOID Animate_Quick(_uint iAniState);
	typedef VOID(CNewPlayer::*ANIFSM)();
private:
	//Jump Func
	void Jump_Func(const _double& TimeDelta);
	//Dash Func
	void Dash_Func(const _double& TimeDelta);
	//Attack Func
	void Attack_Func(const _double& TimeDelta);
	//Rigd Func
	void Rigd_Func(const _double& TimeDelta);

	//Reset State
	void Reset_State();
		
private:
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CRenderer* m_pRenderer;
	CSword*			m_pSword;
	
private:
	//Animate State
	_bool m_bAnimate;
	_uint m_iCurAniState;
	_uint m_iPreAniState;
	
	_double m_TimeAccel;
	_double m_Delay;

private:
	//Jump
	_bool m_bJump;
	_float m_fPosY;
	_double m_JumpTime;
	_float m_fGravity = 1.8f;
	_float m_fJumpPower = 0.5f;

	//Dash
	_bool m_bDash;
	_vec3 m_vDashDir;
	_double m_DashTime;
	//Attack
	_bool m_bAttack[6];
	_uint m_iAniSet[6]; // 90 - 88 - 84 - 78 - 80 - 67
	_double m_AttackTime;
	//Hit State
	_bool m_bHit;
	_double m_RigdTime;

public:
	static CNewPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;

};

#endif // NewPlayer_h__
