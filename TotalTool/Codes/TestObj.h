#ifndef TestObj_h__
#define TestObj_h__

#include "GameObject.h"

class CTestObj :
	public ENGINE::CGameObject
{
private:
	explicit CTestObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestObj();

public:
	void Set_MeshState(_bool bMesh) { bMeshState = bMesh; }
	void Set_DeleteObj();
	void Set_Transform(const _vec3 vPos, const _vec3 vRot, const _vec3 vSize);
	_vec3 Get_vPos();
	_vec3 Get_vRot();
	_vec3 Get_vScale();
	_ulong		Get_NumVtx(void);
	_ulong		Get_Stride(void);
	_vec3*	Get_VtxMeshPos(void);
	ENGINE::CStaticMesh* Get_Static() { return m_pMeshCom; }
	ENGINE::CDynamicMesh* Get_Dynamic() { return m_pDynamicCom; }
	_matrix Get_Matrix() { return m_matWorld; }

public:
	HRESULT Ready_MeshObject(wstring strMesh);
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _float& fTimeDelta) override;
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
	ENGINE::CTransform*	m_pTransCom;
	ENGINE::CRenderer* m_pRenderCom;
	ENGINE::CStaticMesh* m_pMeshCom;
	ENGINE::CDynamicMesh* m_pDynamicCom;

	//DeleteObj
	_bool bDelete;
	
	//
	_bool bMeshState;

public:
	_matrix m_matWorld;

public:
	static CTestObj* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CTestObj* Create_MeshObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring strMesh, _vec3 vTransform[], _bool bMesh);

private:
	virtual void Free() override;


};

#endif // TestObj_h__
