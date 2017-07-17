#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxString.h"
#include "..\..\Tools\AxList.h"

class AxScriptDataType
{
public:
	int size;
	AxString name;
	AxList<AxScriptDataType*> fields;

	AxScriptDataType(AxString name);
	AxScriptDataType(AxString name, int size);
	~AxScriptDataType(void);

	void AddField(AxScriptDataType *field);
};

