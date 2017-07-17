#pragma once

#include "..\..\Engine\Base\AxModuleDispatcher.h"

class AxPhysicalFileSystemDispatcher :
	public AxModuleDispatcher
{
public:
	AxPhysicalFileSystemDispatcher(void);
	~AxPhysicalFileSystemDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

