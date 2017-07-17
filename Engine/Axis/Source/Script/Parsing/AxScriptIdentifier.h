#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxString.h"

#include "AxScriptDataType.h"

class AxScriptIdentifier
{
public:
	AxString name;
	AxScriptDataType* dataType;
	int address;
	int elements;

	AxScriptIdentifier(AxString name, AxScriptDataType* dataType, int address);
	~AxScriptIdentifier(void);
};
