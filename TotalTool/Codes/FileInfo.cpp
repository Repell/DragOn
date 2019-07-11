#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR szRelativePath[MAX_STR] = L"";
	TCHAR curPath[MAX_STR] = L"";

	//���� �۾���� �޾ƿ���
	GetCurrentDirectory(MAX_STR, curPath);

	//���� ��θ� ����η� ��ȯ���ִ� �Լ�
	///CurPath���� strFulPath�� ã�ư��� ����θ� ã�� szRelativePath�� ��ȯ (��, ���� ����̺�� �����ؾ���)
	PathRelativePathTo(szRelativePath, curPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath, FILE_ATTRIBUTE_DIRECTORY);

	CString cStr = L"";
	cStr = szRelativePath;
	cStr.Replace(L"\\", L"/");


	return cStr;
}

void CFileInfo::DirInfoExtract(const wstring & strFullPath, map<wstring, ENGINE::MESH_INFO*> PathList[], HTREEITEM* hTemp, CTreeCtrl* Tree)
{
	CFileFind finder;		//�˻�Ŭ���� ����
	HTREEITEM hLevel;

	wstring filePath = strFullPath + L"/*.*";		//��� �� ���� ����(*.* == ��� �� ���� ����)

	//�˻���� �����̳� ������ ������ TRUE ��ȯ
	bool bWorking = finder.FindFile(filePath.c_str());

	while (bWorking)
	{
		//������ ������� ���ϼ����ϴ� �Լ�
		bWorking = finder.FindNextFile();
		CString strFile = finder.GetFileName();	//���ϸ�.Ȯ����

		if (finder.IsDots())		// .  �̰ų� .. �϶��� �׳� ����
			continue;
		else if (finder.IsDirectory())
		{
			hLevel = Tree->InsertItem(strFile, 0, 0, *hTemp);
			//ã�� ������ ������ ��� �ڱ� �ڽ��� �ٽ��ѹ� �����Ͽ� ������������ ����ã��
			DirInfoExtract(finder.GetFilePath().GetString(), PathList, &hLevel, Tree);
		}
		else  if(finder.IsArchived())//������ ã��
		{
			if (finder.IsSystem())		//�ý��� ���� �Ÿ���
				continue;

			if (0 < strFile.Find(_T(".x")) || 0 < strFile.Find(_T(".X")))	//ã�� ������ .X �����̸�?
			{
				ENGINE::MESH_INFO* pMesh = new ENGINE::MESH_INFO;

				CString strPath, strName;
				
				ENGINE::MESHTYPE eType;

				TCHAR szCurPath[MAX_STR] = L"";
				lstrcpy(szCurPath, finder.GetFilePath().GetString());	//ã�� ������ ��θ� �޾ƿ´�
				PathRemoveExtension(szCurPath);			///Ȯ���� ����
				PathRemoveFileSpec(szCurPath);		///���ϸ� ����
				//����ü�� ���� ��� ��� �Է�
				CString rPath = ConvertRelativePath(szCurPath);
				//pMesh->pFilePath = rPath + L"\\";
				strPath = rPath + L"/";
				
				if (0 < rPath.Find(_T("StaticMesh")))
				{
					pMesh->wMeshType = 0;
					pMesh->wContainerIdx = CValueMgr::iStaticCnt;
					CValueMgr::iStaticCnt += 1;
					eType = ENGINE::MESH_STATIC;
				}
				else if (0 < rPath.Find(_T("DynamicMesh")))
				{
					pMesh->wMeshType = 1;
					pMesh->wContainerIdx = CValueMgr::iDynamicCnt;
					CValueMgr::iDynamicCnt += 1;
					eType = ENGINE::MESH_DYNAMIC;
				}

				pMesh->iUnitType = 0;

				//����ü�� ���ϳ������� �������
				pMesh->pFileName = strFile.GetString();
				hLevel = Tree->InsertItem(strFile, 0, 0, *hTemp);
				strName = strFile;

				//����ü�� �Ž��±׷� �������
				strFile = finder.GetFileTitle().GetString();
				wstring MeshTag = L"Mesh_" + strFile;
				pMesh->pMeshTag = MeshTag;

				//���� ��ο��� ���������� ��� �缳��
				//PathRemoveFileSpec(szCurPath);										
				PathList[pMesh->wMeshType].emplace(pMesh->pFileName.c_str(), pMesh);												//����

				if (FAILED(ENGINE::Ready_Meshes(ENGINE::Get_GraphicDev()->Get_Device(), RESOURCE_TOOL, pMesh->pMeshTag.c_str(), eType, strPath.GetString(), pMesh->pFileName.c_str())))
				{
					ERR_BOX("Mesh Insert Failed!");
					AfxMessageBox(pMesh->pFileName.c_str());
				}

				//continue; // �����ʿ� ����
				break;
			}

			
		}

	}

}

int CFileInfo::DirFileCount(const wstring & strFullPath)
{
	int iCount = 0;

	CFileFind finder;

	wstring strFind = strFullPath + L"\\*.*";

	bool bWorking = finder.FindFile(strFind.c_str());
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;
		else
		{
			if (finder.IsSystem())
				continue;
			++iCount;
		}
	}

	return iCount;
}
