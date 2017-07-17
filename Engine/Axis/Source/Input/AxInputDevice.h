#pragma once

#include "..\AxGlobals.h"

class Axis;

class AXDLLCLASS AxInputDevice
{
public:
	AxInputDevice(void);
	virtual ~AxInputDevice(void);

	virtual void SetContext(Axis *context) = 0;
	virtual void Update() = 0;
};
