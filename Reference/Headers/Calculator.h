#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class CTransform;	//Transform
class CTerrainTex;	//Buffer
class ENGINE_DLL CCalculator :
	public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator();

public:
	HRESULT Ready_Calculator();
	_float Compute_HeightOnTerrain(const _vec3* pPos,
		const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX, const _ulong& dwCntZ);
	_vec3 Picking_Terrain(HWND hWnd, const CTerrainTex* pTerrainBuffer, const CTransform* pTransCom);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

public:
	static CCalculator* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};


NS_END

#endif // Calculator_h__
