#ifndef TestStage_h__
#define TestStage_h__

#include "Scene.h"
#include "Loading.h"
#include "Terrain.h"
#include "TerrainSkyBox.h"

//Dynamic
#include "NewPlayer.h"
#include "Player.h"
#include "Michael.h"
#include "Player_Rider.h"
#include "Troll.h"
#include "Enemy_Swordman.h"
//Static
#include "StaticObj.h"
#include "Sword.h"
#include "Spear.h"

#include "Effect_Tex.h"
#include "Shade.h"

#include "DynamicCamera.h"
#include "StaticCamera.h"

class CTestStage : public ENGINE::CScene
{
private:
	explicit CTestStage(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CTestStage();

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(_double fTimeDelta);
	virtual void Late_Update_Scene();
	virtual void Render_Scene();

protected:
	virtual HRESULT Add_Environment_Layer() override;
	virtual HRESULT Add_GameObject_Layer() override;
	virtual HRESULT Add_UI_Layer() override;

private:
	void RenderPipeLine_SetUp();
	void Create_Light();
	void LoadForStaticDat(ENGINE::CLayer* pLayer);

private:
	CLoading*								m_pLoading;


public:
	static CTestStage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // TestStage_h__
