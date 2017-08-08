#pragma once

#include "..\..\AxisEngine.h"

class AxDirectInputDispatcher :
	public AxModuleDispatcher
{
public:
	AxDirectInputDispatcher(void);
	virtual ~AxDirectInputDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

