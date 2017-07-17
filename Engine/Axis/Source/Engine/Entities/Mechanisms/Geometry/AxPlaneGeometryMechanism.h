#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxPlaneGeometryMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 201;

	static const int propertyIndex_Width			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Length			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_WidthSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_LengthSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_Mesh				= AxResource::propertyIndex_ChildPropertiesIndex + 4;

	AxPlaneGeometryMechanism(void);
	virtual ~AxPlaneGeometryMechanism(void);

	void OnPropertiesChanged();
};

