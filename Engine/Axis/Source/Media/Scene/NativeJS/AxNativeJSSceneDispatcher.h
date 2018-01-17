#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Engine\Base\AxModuleDispatcher.h"

class AXDLLCLASS AxNativeJSSceneDispatcher :
	public AxModuleDispatcher
{
public:
	AxNativeJSSceneDispatcher(void);
	~AxNativeJSSceneDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

