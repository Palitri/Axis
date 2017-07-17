#pragma once

#include "..\AxGlobals.h"

#include "..\Tools\AxString.h"

struct AxFileInfo
{
	AxString fileName;
	bool isDirectory;
	unsigned long long size;
};