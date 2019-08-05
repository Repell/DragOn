#ifndef Michael_h__
#define Michael_h__

#include "GameObject.h"
#include "Effect_Tex.h"

namespace ENGINE
{
	class CRenderer;
	class CDynamicMesh;
	class CTransform;
	class CGameObject;
	class CNaviMesh;
	class CCollider;
	class CSphereColl;
	class CShader;
}

class CMichael : public ENGINE::CGameObject
{
	enum DRAGONDIRECTION
	{
		FLYIDLE, FLYUP, FLYDOWN, FLYLEFT, FLYRIGHT, FLYBACK, FLYEND
		
	};

	enum DRAGONSTATE
	{
		IDLE, FLY, FIRE, HIT, END
	};

private:
	explicit CMichael(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMichael();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	_vec3 MouseFunc();
	_bool Key_check(const _double& TimeDelta);
	_bool EnemyColl_Check();
	_bool RelicColl_Check(const _double& TimeDelta);

private:
	void Render_Set();
	void Render_ReSet();
	void Find_BoneMatrix(const char* szBone);

private:
	//Animate Func
	VOID Animate_FSM(_uint iAniState);
	VOID Animate_Quick(_uint iAniState);

private: //Func Pointer
		 //Set Behavior
	VOID Set_Behavior_Progress(const _double& TimeDelta);

private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	//Animate State
	_uint m_iCurAniState;
	_uint m_iPreAniState;

	DRAGONSTATE m_eState;
	DRAGONDIRECTION m_eDir;

private:
	//Fire Gauge
	_float m_fFireGauge;

	//Key Input, Camera, NaviMesh
	_bool bAttack;
	_float fDist;
	_uint iCurAniSet;

	_double m_TimeAccel;

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CTransform*	m_pTargetTransform;
	ENGINE::CCollider*			m_pCollider;
	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CSphereColl*	m_pTargetSphereColl;
	ENGINE::CShader*			m_pShader;

private:
	_matrix m_pBoneMatrix;

public:
	static CMichael* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Michael_h__
