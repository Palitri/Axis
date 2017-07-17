#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxGeosphereGeometryMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 204;

	static const int propertyIndex_Radius		= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Subdivisions	= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Mesh			= AxResource::propertyIndex_ChildPropertiesIndex + 2;

	AxGeosphereGeometryMechanism(void);
	virtual ~AxGeosphereGeometryMechanism(void);

	void OnPropertiesChanged();
};

