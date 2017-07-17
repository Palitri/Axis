#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

class AXDLLCLASS AxFloatValueMechanism
	: public AxMechanism
{
public:
	static const int classId = AxMechanism::classId | 101;

	static const int propertyIndex_Value = AxResource::propertyIndex_ChildPropertiesIndex + 0;

	AxFloatValueMechanism(float value);
	virtual ~AxFloatValueMechanism(void);

	bool Process(float deltaTime);
};

