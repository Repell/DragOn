#ifndef SphereColl_h__
#define SphereColl_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CSphereColl : public CComponent
{
private:
	explicit CSphereColl(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSphereColl();

public:
	void Set_Scale(_float fScale);
	void Set_Invisible(_bool bState);
	_vec3 Set_KnockBackDist(_bool bCheck, _vec3 fDist = _vec3(0.f, 0.f, 0.f));

	_vec3& Get_CollPos();
	_float Get_Radius();
	_bool Get_HitState();
	_bool Get_KnockBackState();
	_bool Get_AirboneState();
	_int Get_iHp(_int iDamage = 0);
	_uint Get_iHitStack(_bool bState = FALSE);

public:
	HRESULT Ready_SphereColl(_float fRadius, _int iHp);
	void Render_SphereColl(const _matrix * pCollMatrix, _float fY = 0.f);
	//_bool Check_Collision(const _vec3 vTargetPos, _float fTargetRadius);
	_bool Check_ComponentColl(CSphereColl* pSphere);

private:
	HRESULT Make_SphereMesh(_float fRadius);
	HRESULT Make_Texture();

public:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	LPD3DXMESH	m_pMesh;
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	//Collision 
	_vec3 m_vCollPos;
	_float m_fRadius;
	_float m_fScale;

	//Life
	_int m_iHp;
	_bool m_bInvisible;
	
	_bool m_bHit;
	_uint m_iHitStack;
	
	_bool m_bKnockBack;
	_vec3 m_vKnockBackDir;

	_bool m_bAirbone;

public:
	static CSphereColl* Create(LPDIRECT3DDEVICE9 pDevice, _float fRadius, _int iHp);
	virtual void Free();

};

NS_END

#endif // SphereColl_h__
