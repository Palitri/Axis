#pragma once

#include "..\..\AxGlobals.h"

class AXDLLCLASS AxMutex
{
private:
	void *handle;

public:

	AxMutex(void);
	virtual ~AxMutex(void);

	virtual bool Lock();
	virtual void Unlock();
};

