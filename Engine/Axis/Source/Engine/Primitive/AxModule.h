#pragma once

#include "..\..\AxGlobals.h"

#include "..\Base\AxModuleDispatcher.h"

#include "..\..\Tools\AxString.h"
#include "..\..\Tools\AxList.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxModuleDispatcher*>;

class AXDLLCLASS AxModule
{
public:
	AxString name;
	void *handle;
	AxList<AxModuleDispatcher*> dispatchersReferences;

	AxModule(void);
	AxModule(void *handle);
	AxModule(void *handle, const AxString &name);
	~AxModule(void);
};

