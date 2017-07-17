#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Engine\Base\AxModuleDispatcher.h"

class AXDLLCLASS AxWavSoundMediaDispatcher :
	public AxModuleDispatcher
{
public:
	AxWavSoundMediaDispatcher(void);
	~AxWavSoundMediaDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

