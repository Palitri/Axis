#pragma once

#include "..\..\AxGlobals.h"

#include "AxEntity.h"

class Axis;

class AXDLLCLASS AxEntityDispatcher
{
public:
	AxEntityDispatcher(void);
	~AxEntityDispatcher(void);

	virtual AxEntity *CreateEntity(Axis *context, int typeId) = 0;
};

