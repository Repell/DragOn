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
	class CAdvanceCamera;
	class CShader;
	class CWeapon;
}

class CNewPlayer : public ENGINE::CGameObject
{
public:
	enum PLAYERDIR
	{
		LEFT, RIGHT, DOWN, UP, 
		LEFTUP, LEFTDOWN,
		RIGHTUP, RIGHTDOWN,
		DIR_END
	};
	enum PLAYERSTATE
	{
		MOVE, FIGHT, FIGHT_MOVE, FIGHT_JUMP, DASH, JUMP, NONE

	};
	enum PLAYERFIGHTSTATE
	{
		COMBO_START,
		COMBO1 , COMBO2, COMBO3, COMBO4,
		COMBO5, COMBO6, COMBO7, COMBO8, COMBO9,
		COMBO_END
	};
	//기본 콤보 8동작
	// 88 ~ 73
	//스킬 동작 

private:
	explicit CNewPlayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CNewPlayer();

public:
	void Set_CurDir(PLAYERDIR eDir);

public:	//OverRide Func
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	//Object Init
	HRESULT Set_AniIndex();
	HRESULT Add_Component();

private:
	//Render D3DRS
	void Render_Set();
	void Render_ReSet();
private:
	// OLD FUNC
	//Mouse Func OLD
	_vec3 MouseFunc();
	_bool Key_Check_Func(const _double& TimeDelta);
	//Key Check Move Func  OLD
	void Key_ChecknMoveState(const _double& TimeDelta);
	
	////////////////////////////////
	//Advance Key Check Func
	void Key_Check_Advance(const _double& TimeDelta);
	void Update_PlayerDir(const _double& TimeDelta);
	void Move_Func(const _double& TimeDelta);

	//Key Check Fight Func
	void Key_ChecknFightState(const _double& TimeDelta);
	void Fight_Func(const _double& TimeDelta);
	void Key_ChecknJumpFightState(const _double& TimeDelta);
	void FightJump_Func(const _double& TimeDelta);

	//Jump Func
	void Jump_Func(const _double& TimeDelta);
	void Reset_JumpStat();

	//Dash Func
	void Dash_Func(const _double& TimeDelta);
	
private:
	//Collision Check
	_bool Check_DirectionCollision(_vec3* vRevDir);
private:
	//Animate Func
	VOID Animate_FSM(_uint iAniState);
	VOID Animate_Quick(_uint iAniState);
private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void Get_WeaponMatrix();

	//Attack Func Old
	void Attack_Func(const _double& TimeDelta);

	
		
private:
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CAdvanceCamera* m_pAdvance;
	ENGINE::CShader*				m_pShader;
	ENGINE::CWeapon*				m_pWeapon;
	
	//CSword*			m_pSword;
	
private:
	//Animate State
	_bool m_bAnimate;
	_uint m_iCurAniState;
	_uint m_iPreAniState;	

	//State 
	PLAYERSTATE m_ePlayerState;
	//Move State
	PLAYERDIR m_eCurDir;
	//Fight State
	_uint m_iComboCnt;
	_uint m_iComboIdx[10];
	_uint m_iComboJumpIdx[5];
	PLAYERFIGHTSTATE m_eFightState;


	//Time State
	_double m_TimeAccel;
	_double m_Delay;

	//KnockBack
	_uint m_iKnockIdx[6];
	_uint m_iKnockCnt = 0;

private:
	//Jump
	_bool m_bJump;
	_float m_fPosY;
	_double m_JumpTime;
	_double m_HoldTime;
	_float m_fGravity;
	_float m_fJumpPower;

	//Dash
	_bool m_bDash;
	_vec3 m_vDashDir;
	_double m_DashTime;
	//Attack
	_bool m_bAttack[9];
	_bool m_bJumpAttack;
	_uint m_iJumpComboCnt;
	
	_double m_AttackTime;
	//Hit State
	_bool m_bHit;
	_bool m_bKnockback;
	_double m_RigdTime;

	//Weapon
	_matrix m_WeaponMat;

public:
	static CNewPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;

};

#endif // NewPlayer_h__
