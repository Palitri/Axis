#pragma once

#include "..\..\AxisEngine.h"

class AxOpenGL1_1Dispatcher :
	public AxModuleDispatcher
{
public:
	AxOpenGL1_1Dispatcher(void);
	~AxOpenGL1_1Dispatcher(void);
	
	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

