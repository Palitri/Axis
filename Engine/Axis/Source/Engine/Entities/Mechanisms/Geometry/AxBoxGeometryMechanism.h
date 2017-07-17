#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxBoxGeometryMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 202;

	static const int propertyIndex_Width			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Length			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Height			= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_WidthSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_LengthSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_HeightSegments	= AxResource::propertyIndex_ChildPropertiesIndex + 5;
	static const int propertyIndex_Texture2x3		= AxResource::propertyIndex_ChildPropertiesIndex + 6;
	static const int propertyIndex_Mesh				= AxResource::propertyIndex_ChildPropertiesIndex + 7;

	AxBoxGeometryMechanism(void);
	virtual ~AxBoxGeometryMechanism(void);

	void OnPropertiesChanged();
};

