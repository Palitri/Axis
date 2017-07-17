#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxHueMapTextureMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 301;

	static const int propertyIndex_Width					= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Height					= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Lightness				= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Texture					= AxResource::propertyIndex_ChildPropertiesIndex + 3;

	AxHueMapTextureMechanism(void);
	virtual ~AxHueMapTextureMechanism(void);

	void OnPropertiesChanged();
};
