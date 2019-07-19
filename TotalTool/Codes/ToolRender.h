#ifndef ToolRender_h__
#define ToolRender_h__

#include "GameObject.h"

#include "DynamicCamera.h"
#include "Terrain.h"
#include "MouseCtn.h"
#include "NaviMaker.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "TapTerrain.h"
#include "TabMesh.h"

#include "TestObj.h"

namespace Engine
{
	class CGameObject;
}

class CToolRender
{
public:
	explicit CToolRender();
	~CToolRender();

public:
	void Update_Render(const _double& TimeDelta);
	void Late_Update_Render();
	void Render();

private:
	HRESULT Ready_ToolRender(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	static CToolRender* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	HRESULT Ready_Resources(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCED eID);
	HRESULT Ready_GameObject();
	HRESULT Add_GameObject(wstring strMesh, wstring typeKey, _vec3 vTransform[] = nullptr);
	HRESULT Delete_GameObject(wstring typeKey);

public:
	HRESULT Create_NewTerrain(const WORD vtx_X, const WORD vtx_Z, const WORD Intv, const WORD Detail);
	void Set_TextureString();
	typedef map<wstring, list<ENGINE::CGameObject*>> MAP_RENDER;
	list<ENGINE::CGameObject*>& Get_RenderList(wstring wstrKey);


private:
	ENGINE::CGraphicDev*	m_pGraphicDev;
	LPDIRECT3DDEVICE9		m_pDevice;

	MAP_RENDER						m_mapRender;

	map<CString, vector<_int>> m_MapTexture;

private:
	void Free();

};


#endif // ToolRender_h__
