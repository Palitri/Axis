#pragma once

#include "..\..\AxisEngine.h"

class AxOpenALDispatcher :
	public AxModuleDispatcher
{
public:
	AxOpenALDispatcher(void);
	~AxOpenALDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

