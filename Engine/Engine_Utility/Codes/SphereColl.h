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
	_vec3 Get_CollPos();
	_float Get_Radius();
	void Set_Scale(_float fScale);
	_int Get_iHp(_int iDamage = 0);

public:
	HRESULT Ready_SphereColl(_float fRadius, _int iHp);
	void Render_SphereColl(const _matrix * pCollMatrix);
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

public:
	static CSphereColl* Create(LPDIRECT3DDEVICE9 pDevice, _float fRadius, _int iHp);
	virtual void Free();

};

NS_END

#endif // SphereColl_h__
