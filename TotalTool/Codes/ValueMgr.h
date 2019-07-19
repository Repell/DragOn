#pragma once

#ifndef __VALUEMGR_H__
#define __VALUEMGR_H__

class CValueMgr
{
private:
	explicit CValueMgr();
	~CValueMgr();

public:
	//Static º¯¼ö
	static TABSTATE eTab;
	static _matrix matCam;
	static _matrix matTerrain;
	static _matrix matProj;
	static _matrix matView;
	
	static _vec2 vRatio;
	static _vec2 vMouse;

	static _bool bWireFrm_Terrain;
	static _bool bAxisLine;
	static _bool bWireFrm_Obj;
	static _bool bMeshState;
	static _bool bMakeLine;
	static _bool bVisibleBone;

	static _int iStaticCnt;
	static _int iDynamicCnt;

	static CString strCurObj;
	static _int iCurIndex;

	static WORD wSizeX;
	static WORD wSizeZ;

	static _int iNaviCount;
};

#endif // !__VALUEMGR_H__
