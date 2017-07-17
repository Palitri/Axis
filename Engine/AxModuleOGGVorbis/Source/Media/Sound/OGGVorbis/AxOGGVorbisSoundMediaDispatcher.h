#pragma once

#include "..\..\..\AxisEngine.h"

class AxOGGVorbisSoundMediaDispatcher :
	public AxModuleDispatcher
{
public:
	AxOGGVorbisSoundMediaDispatcher(void);
	~AxOGGVorbisSoundMediaDispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};

