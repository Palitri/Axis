#pragma once

#include "..\..\..\AxisEngine.h"

class AxAssimpSceneDispatcher :
	public AxModuleDispatcher
{
public:
	AxAssimpSceneDispatcher(void);
	~AxAssimpSceneDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};
