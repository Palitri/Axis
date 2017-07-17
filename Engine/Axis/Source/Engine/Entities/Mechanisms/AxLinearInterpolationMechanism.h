#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

class AXDLLCLASS AxLinearInterpolationMechanism :
	public AxMechanism
{
public:
	static const int classId = AxMechanism::classId | 105;

	static const int propertyIndex_Argument			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_ArgumentLeft		= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_ArgumentRight	= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Left				= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_Right			= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_ClampLeft		= AxResource::propertyIndex_ChildPropertiesIndex + 5;
	static const int propertyIndex_ClampRight		= AxResource::propertyIndex_ChildPropertiesIndex + 6;

	AxLinearInterpolationMechanism(float argumentLeft, float argumentRight, float left, float right, bool clampLeft, bool clampRight);
	virtual ~AxLinearInterpolationMechanism(void);

	bool Process(float deltaTime);
};

