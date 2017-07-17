#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Engine\Base\AxModuleDispatcher.h"

class AxWindowsFormsInputDeviceDispatcher
	: public AxModuleDispatcher
{
public:
	AxWindowsFormsInputDeviceDispatcher(void);
	virtual ~AxWindowsFormsInputDeviceDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

