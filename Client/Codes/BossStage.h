#ifndef BossStage_h__
#define BossStage_h__

#include "Scene.h"
#include "TestStage.h"
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
#include "Enemy_Spearman.h"
#include "Enemy_Shieldman.h"
#include "Enemy_Bowman.h"

#include "Boss_Keroberos.h"
#include "Boss_Blue.h"

//Static
#include "StaticObj.h"
#include "Static_Tower.h"
#include "Static_Relic.h"
#include "Sword.h"
#include "Spear.h"

#include "Effect_Tex.h"
#include "Shade.h"

#include "DynamicCamera.h"
#include "StaticCamera.h"

class CBossStage : public ENGINE::CScene
{
private:
	explicit CBossStage(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CBossStage();

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
	static CBossStage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // BossStage_h__
