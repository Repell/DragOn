#ifndef Weapon_h__
#define Weapon_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class CTransform;
class CCollider;
class CShader;
class CStaticMesh;
class CSphereColl;
class ENGINE_DLL CWeapon : public CComponent
{
private:
	explicit CWeapon(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWeapon() = default;

public:
	void Set_AttackState(_bool bState, _uint iCurAni, _uint iPower = 0);
	_bool Check_ComponentColl(CSphereColl* pSphere);

public:
	virtual _int Update_Component(const _double& TimeDelta) override;
	virtual void Late_Update_Component() override;
	void Render_Weapon(const _matrix pParentMat);

private:
	HRESULT Ready_Component(CTransform * pTarget, UNITINFO vInfo, const _tchar* szWeapon);
	HRESULT Add_Component(_vec3 vPos, const _tchar* szWeapon, _float fRadius);

private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	//Collision
	void Check_PlayerColl(const _tchar* pObjTag);
	void Check_EnemyColl(const _tchar* pObjTag);
	void Reset_EnemyColl(const _tchar* pObjTag);
	

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

private:		//Transform
	CTransform*	 m_pTarget;	//Player
	CTransform*	 m_pTransform;
	CStaticMesh* m_pMesh;
	CCollider*			m_pCollider;
	CShader*			m_pShader;

private:
	_bool m_bPlayer;
	_bool m_bAttack;

	_uint iDamage;
	_uint m_iCurAni;
	_uint m_iOldAni;
	_float m_fAngle;

public:
	static CWeapon* Create(LPDIRECT3DDEVICE9 pDevice, CTransform * pTarget, UNITINFO vInfo, const _tchar* szWeapon);
	virtual void Free();

};

NS_END

#endif // Weapon_h__
