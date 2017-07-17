#pragma once

#include "..\..\AxisEngine.h"

class AxDirect3D11Dispatcher :
	public AxModuleDispatcher
{
public:
	AxDirect3D11Dispatcher(void);
	~AxDirect3D11Dispatcher(void);

	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info);
	virtual void *CreateObject();
};