#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

#include "..\..\Primitive\AxVector3.h"

class AXDLLCLASS AxOrientationMechanism
	: public AxMechanism
{
private:
    AxVector3 lastSpherical;

public:
	static const int classId = AxMechanism::classId | 106;

	static const int propertyIndex_Transform		= AxResource::propertyIndex_ChildPropertiesIndex + 0;
    static const int propertyIndex_TargetTransform	= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Inertness		= AxResource::propertyIndex_ChildPropertiesIndex + 2;
    static const int propertyIndex_MinAngle			= AxResource::propertyIndex_ChildPropertiesIndex + 3;

	AxOrientationMechanism(void);
	virtual ~AxOrientationMechanism(void);

	bool Process(float deltaTime);
};

