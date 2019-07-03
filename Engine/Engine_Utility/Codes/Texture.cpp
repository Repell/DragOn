#include "Texture.h"

USING(ENGINE)

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResourceses(pGraphicDev)
{
}

CTexture::CTexture(const CTexture & rhs)
	:CResourceses(rhs)
{
	_uint iContianerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContianerSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iContianerSize; ++i)
		m_vecTexture[i]->AddRef();
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(const _tchar * pPath, TEXTURETYPE eID, const _uint iCnt)
{
	m_vecTexture.reserve(iCnt);
	IDirect3DBaseTexture9* pTexture = nullptr;
	
	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR szFileName[MID_STR] = L"";
		wsprintf(szFileName, pPath, i);

		switch (eID)
		{
		case TEX_NORMAL :
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, 
				szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEX_CUBE :
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev,
				szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		m_vecTexture.emplace_back(pTexture);
	}

	return S_OK;
}

void CTexture::Render_Texture(const _uint & iIndex)
{
	_uint iContainerSize = m_vecTexture.size();

	if (iContainerSize <= iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

void CTexture::Free(void)
{
	CResourceses::Free();

	_uint iContainerSize = m_vecTexture.size();

	for (_uint i = 0; i < iContainerSize; ++i)
	{
		Safe_Release(m_vecTexture[i]);
	}
	m_vecTexture.clear();
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, TEXTURETYPE eID, const _uint & iCnt)
{
	CTexture* pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(pPath, eID, iCnt)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CResourceses* CTexture::Clone(void)
{
	return new CTexture(*this);
}
