#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxPlasmaFractalTextureMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 302;

	static const int propertyIndex_Size						= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Color					= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Roughness				= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Texture					= AxResource::propertyIndex_ChildPropertiesIndex + 3;

	AxPlasmaFractalTextureMechanism(void);
	virtual ~AxPlasmaFractalTextureMechanism(void);

	void OnPropertiesChanged();
};

