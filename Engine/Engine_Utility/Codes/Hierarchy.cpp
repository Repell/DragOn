#include "Hierarchy.h"

USING(ENGINE)

CHierarchy::CHierarchy(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath)
	:	m_pGraphicDev(pGraphicDev),
	m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

CHierarchy::~CHierarchy()
{
}

STDMETHODIMP CHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED* pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pNewFrame->Name, Name);
	
	//�� ����� �׵���ķ� ����� ��
	pNewFrame->combinedTransformMatrix = *D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP CHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	//�Ļ�(Derived) �Ž������̳� :: MS���� ���� �Ž� �����̳ʸ� �ڽ�(������)�� �Ը��� �°� �����ؼ� �������� ���� ����ü
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));	//���ΰ� �ʱ�ȭ

	Allocate_Name(&pMeshContainer->Name, Name);

	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;

	_ulong dwNumFaces = pMesh->GetNumFaces();	//�Žð� ���� ������ ���� ��ȯ
	
	pMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];	//�Žð� ���� ������ �� * �̿� ������ ����
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);		//�̿��� ��ü ������ ���� ����

	_ulong dwFVF = pMesh->GetFVF();	//pMesh�� FVF ������ ����

	if (!(dwFVF & D3DFVF_NORMAL))	//���������� �������͸� �����ϴ��� Ȯ��
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), 
			dwFVF | D3DFVF_NORMAL, 
			m_pGraphicDev, 
			&pMeshContainer->MeshData.pMesh);
		//pMesh������ �̿� ���� ������ �̿��� �������͸� ������ִ� �Լ�
		D3DXComputeNormals(pMeshContainer->MeshData.pMesh, pMeshContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pMeshContainer->MeshData.pMesh);
	}
	///////////////////////////////////////////////////////////////

	pMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);	// 0�̸� ? 1�̰� :�ƴϸ� �״��

	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];	//���׸��� ������ŭ �����Ҵ�
	ZeroMemory(pMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);

	pMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	ZeroMemory(pMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			_tchar szFullPath[MAX_STR] = L"";
			_tchar szFileName[MAX_STR] = L"";

			MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[i].pTextureFilename,
				strlen(pMeshContainer->pMaterials[i].pTextureFilename), szFileName, MAX_STR);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pMeshContainer->ppTexture[i])))
			{
				ERR_BOX("DynamicMesh's Texture Create Failed!");
				return E_FAIL;
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pMeshContainer->ppTexture[0] = NULL;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pMeshContainer->pSkinInfo = pSkinInfo;
	pMeshContainer->pSkinInfo->AddRef();

	pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(),
		pMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphicDev, &pMeshContainer->pOriMesh);

	//�޽� �����̳ʿ� ������ ��ġ�� ���� ������ ����
	pMeshContainer->dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();


	//
	pMeshContainer->pFrameOffSetMatrix = new _matrix[pMeshContainer->dwNumBones];
	ZeroMemory(pMeshContainer->pFrameOffSetMatrix, sizeof(_matrix) * pMeshContainer->dwNumBones);

	//�޽ø� �׸������� ����鿡�� �ε������� ����. �� ������� ��ȸ�ϸ鼭 ��� ������ ���ʴ�� �����ϴ� �Լ���
	for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		pMeshContainer->pFrameOffSetMatrix[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
		
	//������� ������ CombinredMatrix�� �Ź� �����ͼ� �����ִ� ���� �۾��� ���� �����ϴ� �迭
	pMeshContainer->ppFrameCombinedMatrix = new _matrix*[pMeshContainer->dwNumBones];
	ZeroMemory(pMeshContainer->ppFrameCombinedMatrix, sizeof(_matrix*) * pMeshContainer->dwNumBones);

	//��������� ���ؼ� �Ҵ��س��� pMeshContainer->pFrameOffSetMatrix�� pMeshCOnteiner->ppFrameCombinedMatrix�� ������
	pMeshContainer->pRenderingMatrix = new _matrix[pMeshContainer->dwNumBones];
	ZeroMemory(pMeshContainer->pRenderingMatrix, sizeof(_matrix) * pMeshContainer->dwNumBones);

	*ppNewMeshContainer = pMeshContainer;
	
	return S_OK;
}

STDMETHODIMP CHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP CHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		Safe_Release(pMeshContainer->ppTexture[i]);

	Safe_Delete_Array(pMeshContainer->ppTexture);

	Safe_Release(pMeshContainer->pOriMesh);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete_Array(pMeshContainer->pFrameOffSetMatrix);
	Safe_Delete_Array(pMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrix);
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pAdjacency);

	Safe_Delete(pMeshContainer);


	return S_OK;
}

void CHierarchy::Allocate_Name(char ** ppName, const char * pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	//pFrameName�� ppName�� �������ش�.
	strcpy_s(*ppName, iLength + 1, pFrameName);

}

CHierarchy * CHierarchy::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath)
{
	return new CHierarchy(pGraphicDev, pPath);
}

_ulong CHierarchy::Release()
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}
