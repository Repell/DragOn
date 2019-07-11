#ifndef TestStage_h__
#define TestStage_h__

#include "Scene.h"
#include "Loading.h"
#include "Terrain.h"
#include "TerrainSkyBox.h"

#include "Player.h"
#include "Sword.h"

#include "Effect_Tex.h"
#include "Shade.h"

#include "DynamicCamera.h"

class CTestStage : public ENGINE::CScene
{
private:
	explicit CTestStage(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CTestStage();

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_float fTimeDelta);
	virtual void Late_Update_Scene();
	virtual void Render_Scene();

protected:
	virtual HRESULT Add_Environment_Layer() override;
	virtual HRESULT Add_GameObject_Layer() override;
	virtual HRESULT Add_UI_Layer() override;

private:
	void RenderPipeLine_SetUp();
	void Create_Light();

private:
	CLoading*								m_pLoading;


public:
	static CTestStage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // TestStage_h__
