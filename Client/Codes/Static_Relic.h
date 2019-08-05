#ifndef Static_Relic_h__
#define Static_Relic_h__

#include "GameObject.h"
#include "Effect_Torch.h"

namespace ENGINE
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CShader;
	class CSphereColl;
	class CAura;
}

class CStatic_Relic :
	public ENGINE::CGameObject
{
private:
	explicit CStatic_Relic(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStatic_Relic();

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
	void Purification(const _double& TimeDelta);

private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CRenderer* m_pRender;
	ENGINE::CStaticMesh* m_pMesh;
	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CShader*			m_pShader;
	ENGINE::CAura*				m_pAura;
	CEffect_Torch*					m_pEffect;

private:
	_bool m_bPurification[5];
	_bool		m_bCharge;
	_float    m_fScale;
	_matrix m_matWorld;
	_double m_PurificationTime;

public:
	static CStatic_Relic* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStatic_Relic* Create_MeshObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring strMesh, _vec3 vTransform[]);

private:
	virtual void Free() override;


};

#endif // Static_Relic_h__
