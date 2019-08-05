#ifndef Effect_Fireball_h__
#define Effect_Fireball_h__

#include "GameObject.h"

namespace ENGINE
{
	class CRcTex;
	class CGameObject;
	class CTransform;
	class CTexture;
	class CRenderer;
	class CSphereColl;
}

class CEffect_Fireball : public ENGINE::CGameObject
{
private:
	explicit CEffect_Fireball(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEffect_Fireball();

public:
	void Set_Move(_bool bMove) { m_bMove = bMove; }
	void Set_Pos(_vec3 vPos);
	void Set_Scale(_vec3 vScale);
	void Set_TotalScale(_float fScale);
	void Set_LifeTime(_double fLife, _double fDelay = 0.0);

public:
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();
	void Update_Frame(const _double& TimeDelta);
	void Update_Billboard();
	_int Fire_Fireball(const _double& TimeDelta);

private:
	void Render_Set();
	void Render_ReSet();

private:
	ENGINE::CRcTex*					m_pBuffer;
	ENGINE::CTransform*		m_pTransform;
	ENGINE::CTransform*		m_pTarget;
	ENGINE::CTexture*				m_pTexture;
	ENGINE::CRenderer*			m_pRenderer;
	ENGINE::CSphereColl*		m_pTargetSphere;

private:
	_bool m_bMove;
	_vec3 m_vFireDir;
	_float   m_fScale;
	_vec3  constScale;

	_double m_Frame;
	_double m_MaxFrame;
	_double m_LifeTime;
	_double m_Time;

public:
	static CEffect_Fireball* Create(LPDIRECT3DDEVICE9 pDevice);
	static CEffect_Fireball* Create(LPDIRECT3DDEVICE9 pDevice, _vec3 vPos, _vec3 vScale, _double fLife, _double fDelay = 0);
	virtual void Free();

};

#endif // Effect_Fireball_h__
