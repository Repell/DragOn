#ifndef Player_Rider_h__
#define Player_Rider_h__

#include "GameObject.h"
#include "Effect_Tex.h"
#include "Spear.h"

namespace ENGINE
{
	class CRenderer;
	class CDynamicMesh;
	class CTransform;
	class CGameObject;
	class CNaviMesh;
	class CCollider;
	class CSphereColl;
}

class CPlayer_Rider : public ENGINE::CGameObject
{
private:
	explicit CPlayer_Rider(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayer_Rider();

public:
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void UI_Sample();
	void Get_ParentMatrix();

private:
	HRESULT Add_Component();

private:
	_bool m_bAi;


private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	ENGINE::CSphereColl*	m_pSphereColl;

	const _matrix*		m_pParentBoneMatrix;
	const _matrix*		m_pParentWorldMatrix;


public:
	static CPlayer_Rider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;



};

#endif // Player_Rider_h__
