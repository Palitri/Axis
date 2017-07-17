#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxTorusGeometryMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 207;

	static const int propertyIndex_Radius				= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_RingRadius			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_SpiralSize			= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_HelicalHeight		= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_Turns				= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_RadialSegments		= AxResource::propertyIndex_ChildPropertiesIndex + 5;
	static const int propertyIndex_RingSegments			= AxResource::propertyIndex_ChildPropertiesIndex + 6;
	static const int propertyIndex_StartCapSegments		= AxResource::propertyIndex_ChildPropertiesIndex + 7;
	static const int propertyIndex_EndCapSegments		= AxResource::propertyIndex_ChildPropertiesIndex + 8;
	static const int propertyIndex_Twists				= AxResource::propertyIndex_ChildPropertiesIndex + 9;
	static const int propertyIndex_WeldEnds				= AxResource::propertyIndex_ChildPropertiesIndex + 10;
	static const int propertyIndex_WeldSurface			= AxResource::propertyIndex_ChildPropertiesIndex + 11;
	static const int propertyIndex_Mesh					= AxResource::propertyIndex_ChildPropertiesIndex + 12;

	AxTorusGeometryMechanism(void);
	virtual ~AxTorusGeometryMechanism(void);

	void OnPropertiesChanged();
};

