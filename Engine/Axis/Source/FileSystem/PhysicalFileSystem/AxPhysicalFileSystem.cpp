//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPhysicalFileSystem.h"

#include "..\..\Tools\Streams\AxFileStream.h"

#ifndef UNICODE
#	define UNICODE
#endif

#include <windows.h>

AxPhysicalFileSystem::AxPhysicalFileSystem(void)
	: AxFileSystem()
{
}


AxPhysicalFileSystem::~AxPhysicalFileSystem(void)
{
}

AxString AxPhysicalFileSystem::ToNativePathFormat(AxString &originalPath)
{
	return AxString(originalPath.Replace(AxFileSystem::dirSplitter, '\\'));
}

AxString AxPhysicalFileSystem::ToOriginalPathFormat(AxString &nativePath)
{
	return AxString(nativePath.Replace('\\', AxFileSystem::dirSplitter));
}

AxStream *AxPhysicalFileSystem::OpenFile(AxString path)
{
	path = this->ToNativePathFormat(path);

	if (!this->FileExists(path.contents))
		return 0;

	return new AxFileStream(path.contents, FileStreamAccessMode_Read);
}

AxStream *AxPhysicalFileSystem::CreateNewFile(AxString path)
{
	return new AxFileStream(this->ToNativePathFormat(path), FileStreamAccessMode_Write);
}

bool AxPhysicalFileSystem::GetFileInfo(AxString path, AxFileInfo &result)
{
	path = this->ToNativePathFormat(path);

	WIN32_FIND_DATA findData;
	HANDLE findHandle = FindFirstFile(path.contents, &findData);
	if (findHandle == INVALID_HANDLE_VALUE)
		return false;

	FindClose(findHandle);

	result.fileName = this->ToOriginalPathFormat(path);
	result.isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	result.size = findData.nFileSizeHigh;
	result.size = (result.size  << 32) | findData.nFileSizeLow;

	return true;
}

bool AxPhysicalFileSystem::GetDirectoryContents(AxString path, AxList<AxFileInfo> &result)
{
	path = this->ToNativePathFormat(path);

	if (path[path.length - 1] != L'\\')
		path += "\\";
   
	path += "*";

	WIN32_FIND_DATA findData;

	HANDLE findHandle = FindFirstFile(path.contents, &findData);

	AxString thisDir("."), previousDir("..");
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(thisDir.Equals(findData.cFileName) || previousDir.Equals(findData.cFileName)))
			{
				AxFileInfo fileInfo;
				fileInfo.fileName = this->ToOriginalPathFormat(AxString(findData.cFileName));
				fileInfo.isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
				fileInfo.size = findData.nFileSizeHigh;
				fileInfo.size = (fileInfo.size << 32) | findData.nFileSizeLow;

				result.Add(fileInfo);
			}
		}
		while (FindNextFile(findHandle, &findData) != 0);
	}
	else
		return false;

	if (GetLastError() != ERROR_NO_MORE_FILES) 
	{
		FindClose(findHandle);
		return false;
	}

	FindClose(findHandle);

	return true;
}
