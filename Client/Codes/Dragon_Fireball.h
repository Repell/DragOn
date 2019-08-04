#ifndef Dragon_Fireball_h__
#define Dragon_Fireball_h__

#include "GameObject.h"
#include "Effect_Tex.h"

namespace ENGINE
{
	class CRenderer;
	class CStaticMesh;
	class CTransform;
	class CGameObject;
	class CSphereColl;
	class CShader;
}

class CDragon_Fireball : public ENGINE::CGameObject
{
private:
	explicit CDragon_Fireball(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CDragon_Fireball() = default;

private:
	HRESULT Add_Component();

public:
	virtual HRESULT Ready_Object(ENGINE::UNITINFO tInfo);
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Render_Object() override;

private:
	//Transform
	_bool Check_FireballColl();
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);


private:
	ENGINE::UNITINFO m_tInfo;
	_double m_LifeTime;

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CStaticMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CTransform*	m_pTarget;
	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CShader*			m_pShader;

public:
	static CDragon_Fireball* Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::UNITINFO tInfo);
	virtual void Free() override;
	
};

#endif // Dragon_Fireball_h__
