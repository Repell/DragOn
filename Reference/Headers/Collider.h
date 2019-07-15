#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCollider();

public:
	const _vec3*	Get_Min(void) { return &m_vMin; }
	const _vec3*	Get_Max(void) { return &m_vMax; }
	const _matrix*	Get_ColWorld(void) { return &m_matColWorld; }

public:
	HRESULT Ready_Collider(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride);
	void Render_Collider(COLLTYPE eType, const _matrix* pCollMatrix);

private:
	_vec3 m_vMin;
	_vec3 m_vMax;
	_matrix				m_matColWorld;

#ifdef _DEBUG
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9		m_pTexture[COL_END];
#endif //_DEBUG

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pDevice,
		const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride);
	virtual void Free();

};

NS_END
#endif // Collider_h__