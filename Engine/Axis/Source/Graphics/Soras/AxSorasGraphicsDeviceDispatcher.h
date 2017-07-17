#pragma once

#include "..\..\Engine\Base\AxModuleDispatcher.h"

class AxSorasGraphicsDeviceDispatcher :
	public AxModuleDispatcher
{
public:
	AxSorasGraphicsDeviceDispatcher(void);
	~AxSorasGraphicsDeviceDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

