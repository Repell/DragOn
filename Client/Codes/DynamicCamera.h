#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

namespace ENGINE
{
	class CCamera;
	class CTransform;
}

class CDynamicCamera :
	public ENGINE::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pDev);
	virtual ~CDynamicCamera();

public:
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Late_Update_Object() override;

private:
	HRESULT Add_Component();
	void Reset_Camera();
	void Key_Input(const _float& fTimeDelta);
	void Key_Spectre(const _float& fTimeDelta);
	void Target_Renewal();
	void Target_Spectre();
		
public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CTransform*				m_pCamTarget;
	ENGINE::CTransform*				m_pTransCom;

	bool			bSpectre;
	float		m_fTargetDist;
	float		m_fCamAngle;
	float		m_fCamSpeed;
	float		m_fTargetDistMax;

private:
	virtual void Free() override;
	
};

#endif // DynamicCamera_h__
