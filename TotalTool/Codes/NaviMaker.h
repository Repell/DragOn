#ifndef NaviMaker_h__
#define NaviMaker_h__

#include "GameObject.h"
#include "Coll_Mouse.h"

#include "MainFrm.h"
#include "TotalToolView.h"
#include "ToolRender.h"
#include "Terrain.h"

class CNaviMaker :
	public ENGINE::CGameObject	
{
public:
	explicit CNaviMaker(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNaviMaker();

public:
	void Transform_NaviMesh(_int iNavi, _int iTri, _vec3 vPos);
	ENGINE::NAVI Get_Navi(_int iNavi);
	map<_int, ENGINE::NAVI>& Get_MapNavi();

	void Insert_NaviTree();
	void Delete_MapNavi(_int iNavi, _int iTri);
	void Clear_MapNavi();
	void Make_TerrainVertex(WORD iX, WORD iY);

public:
	virtual HRESULT Ready_Object();
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta);
	virtual void Late_Update_Object();
	virtual void Render_Object();

private:
	HRESULT Add_Component();
	void Render_Font(const _vec2* pPos);
	_vec2 MousePos(const _vec2* pMice);
	
private:
	void Make_NaviMesh();
	void Render_Navi();

public:
	void Render_Bones();


private:
	//Collision Point
	_bool Coll_NaviPoint();
	_bool Coll_Terrain(ENGINE::_vec3* pPos);

private:	
	ENGINE::NAVI		m_tNavi;
	
	map<_int, ENGINE::NAVI>m_MapNavi;

	//Mice
	_vec2 m_vMousePos;
	_vec3 m_vPickPos;

	//Make Line
	
	_ulong wCount;

	//Coll
	CColl_Mouse*						m_pCollMouse;
	ENGINE::VTX_TEX*				m_pVtxOrigin;

	_int m_iCurNavi;
	_int m_iCurPoint;

	//D3DX
	LPD3DXSPRITE						m_pSprite;
	LPD3DXLINE							m_pLine;

public:
	static CNaviMaker* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free();

};


#endif // NaviMaker_h__
