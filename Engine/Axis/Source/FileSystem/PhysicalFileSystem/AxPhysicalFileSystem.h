#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxFileSystem.h"

class AXDLLCLASS AxPhysicalFileSystem :
	public AxFileSystem
{
private:
	AxString ToNativePathFormat(AxString &originalPath);
	AxString ToOriginalPathFormat(AxString &nativePath);

public:
	AxPhysicalFileSystem(void);
	~AxPhysicalFileSystem(void);

	AxStream *OpenFile(AxString path);
	AxStream *CreateNewFile(AxString path);

	virtual bool GetFileInfo(AxString path, AxFileInfo &result);
	virtual bool GetDirectoryContents(AxString path, AxList<AxFileInfo> &result);
};

