#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxList.h"

#include "AxScriptDataType.h"

class AXDLLCLASS AxScriptDataTypes :
	public AxList<AxScriptDataType*>
{
public:
	AxScriptDataTypes(void);
	~AxScriptDataTypes(void);

	AxScriptDataType *Find(AxString name);
};
