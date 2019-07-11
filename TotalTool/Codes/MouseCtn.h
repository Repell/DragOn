#ifndef MouseCtn_h__
#define MouseCtn_h__

#include "GameObject.h"
#include "Coll_Mouse.h"

#include "MainFrm.h"
#include "TotalToolView.h"
#include "ToolRender.h"
#include "Terrain.h"



class CMouseCtn :
	public ENGINE::CGameObject	
{
public:
	explicit CMouseCtn(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMouseCtn();


public:
	virtual HRESULT Ready_Object();
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _float& fTimeDelta);
	virtual void Late_Update_Object();
	virtual void Render_Object();

private:
	HRESULT Add_Component();
	void Render_Font(const _vec2* pPos);
	_vec2 MousePos(const _vec2* pMice);
	
public:
	void Insert_TabNavi();
	void Delete_TabNavi(_int iNavi, _int iTri);

	void Load_NaviList(ENGINE::LINE_3D pLine, _int iCount);
	ENGINE::LINE_3D Get_vecLine3D(_int iNavi);
	void Transform_NaviMesh(_int iNavi, _int iTri, _vec3 vPos);

	map<_int, ENGINE::LINE_3D>& Get_MapLine() { return m_MapLine; }

private:
	void Make_Line();
	void Make_NaviMesh();
	_bool Pick_Mesh(_vec3 * pPos);
	void Render_NaviMesh();
	_bool Coll_Navi();
	_bool Coll_Sphere(ENGINE::LINE_3D pLine);

public:
	static CMouseCtn* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::LINE_3D m_tLine;
	
	_ulong wCount;

	_vec2 m_vMousePos;
	_vec3 m_vPickPos;
	_vec3 m_LinePos[3];

	bool bMakeLine;
	bool bRenderLine;

	_int m_iCurNavi;
	_int m_iCurNavi_Tri;

	//vector<ENGINE::LINE_3D> m_vecLine;
	//vector<ENGINE::CTriTex*> m_vecNavi;
	map<_int, ENGINE::LINE_3D>m_MapLine;
	map<_int, ENGINE::CTriTex*>m_MapNavi;

	//Coll
	CColl_Mouse*						m_pCollMouse;
	ENGINE::VTX_TEX*				m_pVtxOrigin;
	ENGINE::VTX_TEX*				m_pVtxCopy;

	//D3DX
	LPD3DXSPRITE						m_pSprite;
	LPD3DXLINE							m_pLine;

private:
	void Free();

};


#endif // MouseCtn_h__
