#pragma once

#include "..\..\..\AxisEngine.h"

class AxFbxSceneDispatcher :
	public AxModuleDispatcher
{
public:
	AxFbxSceneDispatcher(void);
	~AxFbxSceneDispatcher(void);
	
	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};
