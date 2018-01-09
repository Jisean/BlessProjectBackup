#pragma once

#include "Include.h"

class CFileInfo
{
public:
	wstring	ConvertRelativePath(wstring wstrFullPath);
	void	DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList);
	int		DirFileCount(const wstring& wstrPath);

public:
	CFileInfo(void);
	~CFileInfo(void);
};