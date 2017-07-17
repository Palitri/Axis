#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxSphereGeometryMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 203;

	static const int propertyIndex_Radius			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_RadialSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_HeightSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Twist			= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_WeldSurface		= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_Mesh				= AxResource::propertyIndex_ChildPropertiesIndex + 5;

	AxSphereGeometryMechanism(void);
	virtual ~AxSphereGeometryMechanism(void);

	void OnPropertiesChanged();
};

