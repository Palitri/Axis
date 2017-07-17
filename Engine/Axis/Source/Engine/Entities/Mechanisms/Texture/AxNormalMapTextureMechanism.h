#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxNormalMapTextureMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 303;

	static const int propertyIndex_Emphasis					= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Filter					= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_SourceHeightMap			= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Texture					= AxResource::propertyIndex_ChildPropertiesIndex + 3;

	AxNormalMapTextureMechanism(void);
	virtual ~AxNormalMapTextureMechanism(void);

	void OnPropertiesChanged();
};

