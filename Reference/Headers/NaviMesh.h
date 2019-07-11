#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"
#include "Cell.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh();

public:
	void Set_CurrentIdx(_ulong dwIndex) { m_dwCurrentIdx = dwIndex; }

public:
	HRESULT Ready_NaviMesh();
	void Render_NaviMesh();
	_vec3 MoveOn_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);

private:
	HRESULT Load_NaviMesh(_int iMax);
	HRESULT Link_Cell();
	_float Compute_OnTerrain(const _vec3* pPos, _ulong * pCellIndex);
	
private:
	vector<CCell*> m_vecCell;
	_ulong m_dwCurrentIdx;

public:
	static CNaviMesh* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CResourceses* Clone();
	virtual void Free();

};

NS_END

#endif // NaviMesh_h__
