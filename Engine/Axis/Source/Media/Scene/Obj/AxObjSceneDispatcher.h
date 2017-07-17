#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Engine\Base\AxModuleDispatcher.h"

class AXDLLCLASS AxObjSceneDispatcher :
	public AxModuleDispatcher
{
public:
	AxObjSceneDispatcher(void);
	~AxObjSceneDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};