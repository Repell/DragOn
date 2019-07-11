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

	//현재 작업경로 받아오기
	GetCurrentDirectory(MAX_STR, curPath);

	//절대 경로를 상대경로로 변환해주는 함수
	///CurPath에서 strFulPath를 찾아가는 상대경로를 찾아 szRelativePath에 반환 (단, 같은 드라이브상에 존재해야함)
	PathRelativePathTo(szRelativePath, curPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath, FILE_ATTRIBUTE_DIRECTORY);

	CString cStr = L"";
	cStr = szRelativePath;
	cStr.Replace(L"\\", L"/");


	return cStr;
}

void CFileInfo::DirInfoExtract(const wstring & strFullPath, map<wstring, ENGINE::MESH_INFO*> PathList[], HTREEITEM* hTemp, CTreeCtrl* Tree)
{
	CFileFind finder;		//검색클래스 생성
	HTREEITEM hLevel;

	wstring filePath = strFullPath + L"/*.*";		//경로 내 파일 지정(*.* == 경로 내 파일 전부)

	//검색결과 파일이나 폴더가 있으면 TRUE 반환
	bool bWorking = finder.FindFile(filePath.c_str());

	while (bWorking)
	{
		//파일이 있을경우 파일선택하는 함수
		bWorking = finder.FindNextFile();
		CString strFile = finder.GetFileName();	//파일명.확장자

		if (finder.IsDots())		// .  이거나 .. 일때는 그냥 진행
			continue;
		else if (finder.IsDirectory())
		{
			hLevel = Tree->InsertItem(strFile, 0, 0, *hTemp);
			//찾은 파일이 폴더일 경우 자기 자신을 다시한번 실행하여 하위폴더에서 파일찾기
			DirInfoExtract(finder.GetFilePath().GetString(), PathList, &hLevel, Tree);
		}
		else  if(finder.IsArchived())//파일을 찾음
		{
			if (finder.IsSystem())		//시스템 파일 거르고
				continue;

			if (0 < strFile.Find(_T(".x")) || 0 < strFile.Find(_T(".X")))	//찾은 파일이 .X 파일이면?
			{
				ENGINE::MESH_INFO* pMesh = new ENGINE::MESH_INFO;

				CString strPath, strName;
				
				ENGINE::MESHTYPE eType;

				TCHAR szCurPath[MAX_STR] = L"";
				lstrcpy(szCurPath, finder.GetFilePath().GetString());	//찾은 파일의 경로를 받아온다
				PathRemoveExtension(szCurPath);			///확장자 제거
				PathRemoveFileSpec(szCurPath);		///파일명 제거
				//구조체에 파일 상대 경로 입력
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

				//구조체에 파일네임으로 집어넣음
				pMesh->pFileName = strFile.GetString();
				hLevel = Tree->InsertItem(strFile, 0, 0, *hTemp);
				strName = strFile;

				//구조체에 매시태그로 집어넣음
				strFile = finder.GetFileTitle().GetString();
				wstring MeshTag = L"Mesh_" + strFile;
				pMesh->pMeshTag = MeshTag;

				//현재 경로에서 상위폴더로 경로 재설정
				//PathRemoveFileSpec(szCurPath);										
				PathList[pMesh->wMeshType].emplace(pMesh->pFileName.c_str(), pMesh);												//저장

				if (FAILED(ENGINE::Ready_Meshes(ENGINE::Get_GraphicDev()->Get_Device(), RESOURCE_TOOL, pMesh->pMeshTag.c_str(), eType, strPath.GetString(), pMesh->pFileName.c_str())))
				{
					ERR_BOX("Mesh Insert Failed!");
					AfxMessageBox(pMesh->pFileName.c_str());
				}

				//continue; // 수정필요 ㅅㅂ
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
