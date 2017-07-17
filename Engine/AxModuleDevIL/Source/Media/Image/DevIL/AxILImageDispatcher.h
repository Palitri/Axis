#pragma once

#include "..\..\..\AxisEngine.h"

class AxILImageDispatcher
	: public AxModuleDispatcher
{
public:
	AxILImageDispatcher(void);
	~AxILImageDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

