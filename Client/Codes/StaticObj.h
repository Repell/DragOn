#ifndef StaticObj_h__
#define StaticObj_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
}

class CStaticObj :
	public ENGINE::CGameObject
{
private:
	explicit CStaticObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticObj();

public:
	void Set_Transform(const _vec3 vPos, const _vec3 vRot, const _vec3 vSize);

	_vec3		 Get_vPos();
	_vec3		 Get_vRot();
	_vec3		 Get_vScale();
	_matrix	 Get_Matrix() { return m_matWorld; }
	ENGINE::CStaticMesh* Get_Static() { return m_pMesh; }

public:
	HRESULT Ready_MeshObject(wstring strMesh);
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();
	HRESULT Add_Component_MeshObject(wstring strMesh);

private:
	void Render_Set();
	void Render_ReSet();
	void Render_Font(const _vec2* pPos);

private:
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CRenderer* m_pRender;
	ENGINE::CStaticMesh* m_pMesh;

private:
	_matrix m_matWorld;

public:
	static CStaticObj* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStaticObj* Create_MeshObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring strMesh, _vec3 vTransform[]);

private:
	virtual void Free() override;


};

#endif // StaticObj_h__
