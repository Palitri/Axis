#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxConeGeometryMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 205;

	static const int propertyIndex_BottmRadius			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_TopRadius			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Height				= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_RadialSegments		= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_HeightSegments		= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_BottomCapSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 5;
	static const int propertyIndex_TopCapSegments		= AxResource::propertyIndex_ChildPropertiesIndex + 6;
	static const int propertyIndex_Mesh					= AxResource::propertyIndex_ChildPropertiesIndex + 7;

	AxConeGeometryMechanism(void);
	virtual ~AxConeGeometryMechanism(void);

	void OnPropertiesChanged();
};

