#ifndef FileInfo_h__
#define FileInfo_h__

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	enum RESOURCED
	{
		RESOURCE_PRELOAD,
		RESOURCE_LOGO,
		RESOURCE_TITLE,
		RESOURCE_STAGE,
		RESOURCE_TOOL,
		RESOURCE_END
	};

public:
	static CString ConvertRelativePath(CString strFullPath);
	static void DirInfoExtract(const wstring& strFullPath, map<wstring, ENGINE::MESH_INFO*> PathList[], HTREEITEM* hTemp, CTreeCtrl* Tree);
	static int DirFileCount(const wstring& strFullPath);
};

#endif // FileInfo_h__