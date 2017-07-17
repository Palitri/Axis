#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Engine\Base\AxModuleDispatcher.h"

class AxNativeImageDispatcher
	: public AxModuleDispatcher
{
public:
	AxNativeImageDispatcher(void);
	~AxNativeImageDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

