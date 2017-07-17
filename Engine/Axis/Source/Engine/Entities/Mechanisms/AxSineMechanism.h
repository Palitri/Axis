#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

class AXDLLCLASS AxSineMechanism
	: public AxMechanism
{
public:
	static const int classId = AxMechanism::classId | 104;

	static const int propertyIndex_Argument		= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_PeriodStart	= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_PeriodEnd	= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_AmplitudeMin	= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_AmplitudeMax	= AxResource::propertyIndex_ChildPropertiesIndex + 4;

	AxSineMechanism(float periodStart, float periodEnd, float amplitudeMin, float amplitudeMax);
	virtual ~AxSineMechanism(void);

	bool Process(float deltaTime);
};

