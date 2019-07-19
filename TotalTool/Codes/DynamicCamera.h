#ifndef DynamicCamera_h__
#define DynamicCamera_h__


#include "Camera.h"

class CDynamicCamera :
	public ENGINE::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pDev);
	virtual ~CDynamicCamera();

public:
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object();

private:
	HRESULT Add_Component();
	void Reset_Camera();
	void Key_Input(const _double& TimeDelta);
	void Key_Spectre(const _double& TimeDelta);
	//void Target_Renewal();
	void Target_Spectre();

private:
	void Render_Set();
	void Render_ReSet();
	void Render_Font(const _vec2* pPos);
	void Render_Line();
		
public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CTransform*				m_pTransCom;

	bool			bSpectre;
	float		m_fTargetDist;
	float		m_fCamAngle;
	float		m_fCamSpeed;
	float		m_fTargetDistMax;

	_bool bKeyCheck;

	//D3DX Fonts and Sprite
	LPD3DXSPRITE						m_pSprite;
	LPD3DXLINE							m_pLine;

private:
	virtual void Free() override;
	
};

#endif // DynamicCamera_h__
