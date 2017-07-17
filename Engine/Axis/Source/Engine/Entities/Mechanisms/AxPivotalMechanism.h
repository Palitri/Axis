#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Primitive\AxVector3.h"

#include "AxMechanism.h"

class AXDLLCLASS AxPivotalMechanism
	: public AxMechanism
{
private:
	AxVector3 gravityVector, targetPos, targetMoveVector;

public:
	static const int classId = AxMechanism::classId | 108;

	static const int propertyIndex_Transform				= AxResource::propertyIndex_ChildPropertiesIndex + 0;
    static const int propertyIndex_TargetTransform			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_EnvironmentResistance	= AxResource::propertyIndex_ChildPropertiesIndex + 2;
    static const int propertyIndex_GravityStrength			= AxResource::propertyIndex_ChildPropertiesIndex + 3;
    static const int propertyIndex_ConnectionLength			= AxResource::propertyIndex_ChildPropertiesIndex + 4;
    static const int propertyIndex_ElasticityLength			= AxResource::propertyIndex_ChildPropertiesIndex + 5;
    static const int propertyIndex_ConnectionSolid			= AxResource::propertyIndex_ChildPropertiesIndex + 6;
    static const int propertyIndex_TargetMass				= AxResource::propertyIndex_ChildPropertiesIndex + 7;

	AxPivotalMechanism(void);
	virtual ~AxPivotalMechanism(void);

	bool Process(float deltaTime);
};

