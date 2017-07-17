#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Engine\Base\AxModuleDispatcher.h"

class AXDLLCLASS AxNativeSceneDispatcher :
	public AxModuleDispatcher
{
public:
	AxNativeSceneDispatcher(void);
	~AxNativeSceneDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

