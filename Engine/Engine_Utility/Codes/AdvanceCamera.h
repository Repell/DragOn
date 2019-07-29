#ifndef AdvanceCamera_h__
#define AdvanceCamera_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class CTransform;
class ENGINE_DLL	CAdvanceCamera :
	public CComponent
{
private:
	explicit CAdvanceCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CAdvanceCamera() = default;

public:
	_vec3 Set_Transform_Pos(ENGINE::INFO eInfo);
	ENGINE::CTransform* Get_Transform() 
	{		return m_pTransform;	}
	ENGINE::CTransform* Get_PlayerTransform()
	{		return m_pTarget;	}
	_vec3 Get_INFO(ENGINE::INFO eInfo);
	_vec3 Get_LookDir();
	_matrix Get_ViewMatrix();
	
public:
	HRESULT Ready_Component(ENGINE::CTransform * pTarget);
	HRESULT Make_Camera();
	void Update_Target(const _double & TimeDelta);

public:
	virtual _int Update_Component(const _double& TimeDelta) override;
	virtual void Late_Update_Component() override;
		
private:
	void Make_ViewMatrix(
		const D3DXVECTOR3* pEye,
		const D3DXVECTOR3* pAt,
		const D3DXVECTOR3* pUp);

	void Make_ProjectionMatrix(
		const float& fFovY,
		const float& fAspect,
		const float& fNear,
		const float& fFar);



private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	
	D3DXMATRIX	 m_matView;
	D3DXMATRIX	 m_matProj;

	D3DXVECTOR3	m_vEye;
	D3DXVECTOR3	m_vAt;
	D3DXVECTOR3	m_vUp;
	_float m_fCamDist;

private:		//Transform
	ENGINE::CTransform* m_pTarget;	//Player
	ENGINE::CTransform* m_pTransform;

	_double m_Time;

public:
	static CAdvanceCamera* Create(LPDIRECT3DDEVICE9 pGraphic, ENGINE::CTransform * pTarget);
	virtual void Free();

};

NS_END

#endif // AdvanceCamera_h__
