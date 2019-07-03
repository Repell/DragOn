#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"
#include "TestStage.h"

#include "Logo_Loading.h"

class CLogo : public ENGINE::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CLogo();

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
	void Title_Change();

private:
	CLoading*								m_pLoading;


public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // Logo_h__
