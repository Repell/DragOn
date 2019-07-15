#ifndef Loading_h__
#define Loading_h__

#include "Base.h"

#define MID_STR 128

class CLoading :
	public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	LOADINGID Get_LoadingID() const { return m_eID; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool	Get_Finished() { return m_bFisihed; }
	const _tchar* Get_String() { return m_szLoading; }

public:
	HRESULT	Ready_Loading(LOADINGID eLoading);
	_uint		Loading_ForLogo();
	_uint		Loading_ForStage();
	_uint		Load_DefaultResource();

private:
	HRESULT LoadForTerrainDat();

public:
	static _uint CALLBACK Thread_Main(void* pArg);

private:
	HANDLE						m_hThread;
	CRITICAL_SECTION	m_Crt;
	LOADINGID					m_eID;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool							m_bFisihed;
	_tchar						m_szLoading[MID_STR];

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
private:
	virtual void Free();
};

#endif // Loading_h__

