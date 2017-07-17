#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Engine\Base\AxModuleDispatcher.h"

class AXDLLCLASS Ax3dsSceneDispatcher :
	public AxModuleDispatcher
{
public:
	Ax3dsSceneDispatcher(void);
	~Ax3dsSceneDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

