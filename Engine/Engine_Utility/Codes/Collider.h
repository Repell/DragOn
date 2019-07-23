#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class CSphereColl;
class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCollider();

public:
	const _vec3*	Get_Min(void) { return &m_vMin; }
	const _vec3*	Get_Max(void) { return &m_vMax; }
	const _matrix*	Get_ColWorld(void) { return &m_matColWorld; }
	_float Get_Radius();
	void Set_Scale(_float fScale);

public:
	HRESULT Ready_Collider(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride, _float fRadius);
	HRESULT Ready_Collider_Sphere(_float fRadius);
	void Render_Collider(COLLTYPE eType, const _matrix* pCollMatrix, _vec3 CollPos);

	_bool Check_ComponentColl(CSphereColl* pSphere);

private:
	_vec3 m_vMin;
	_vec3 m_vMax;
	_matrix				m_matColWorld;
	_float m_fRadius;
	_float m_fScale;

#ifdef _DEBUG
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexture[COL_END];
#endif //_DEBUG

	LPD3DXMESH m_pMesh;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pDevice, _float fRadius);
	virtual void Free();

};

NS_END
#endif // Collider_h__