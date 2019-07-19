#include "stdafx.h"	//tool
#include "ValueMgr.h"

//Static Data
TABSTATE CValueMgr::eTab = TABEND;

_matrix CValueMgr::matTerrain = {};
_matrix CValueMgr::matProj = {};
_matrix CValueMgr::matView = {};
_matrix CValueMgr::matCam = {};

_bool CValueMgr::bWireFrm_Terrain = FALSE;
_bool CValueMgr::bWireFrm_Obj = FALSE;
_bool CValueMgr::bAxisLine = TRUE;
_bool CValueMgr::bMeshState = TRUE;
_bool CValueMgr::bMakeLine = FALSE;
_bool CValueMgr::bVisibleBone = FALSE;

_vec2 CValueMgr::vRatio = {};
_vec2 CValueMgr::vMouse = {};

_int CValueMgr::iStaticCnt = 0;
_int CValueMgr::iDynamicCnt = 0;

CString CValueMgr::strCurObj = L"";
_int CValueMgr::iCurIndex = 0;

WORD CValueMgr::wSizeX = TERRAIN_VTX_X;
WORD CValueMgr::wSizeZ = TERRAIN_VTX_Z;

_int CValueMgr::iNaviCount = 0;

CValueMgr::CValueMgr()
{
}

CValueMgr::~CValueMgr()
{
}
