#pragma once

#include "..\..\AxisEngine.h"

class AxOpenGLDispatcher :
	public AxModuleDispatcher
{
public:
	AxOpenGLDispatcher(void);
	~AxOpenGLDispatcher(void);
	
	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

