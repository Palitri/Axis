#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxList.h"

#include "AxScriptIdentifier.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxScriptIdentifier*>;

class AXDLLCLASS AxScriptIdentifiers
{
public:
	AxList<AxScriptIdentifier*> list;
	int size;

	AxScriptIdentifiers(void);
	~AxScriptIdentifiers(void);

	AxScriptIdentifier *Find(AxString name);
	AxScriptIdentifier *Add(AxString name, AxScriptDataType *dataType, int numElements);
	void Clear();
};

