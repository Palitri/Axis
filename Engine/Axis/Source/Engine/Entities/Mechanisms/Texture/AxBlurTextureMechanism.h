#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxBlurTextureMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 304;

	static const int propertyIndex_Blurriness				= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_OriginalTexture			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Texture					= AxResource::propertyIndex_ChildPropertiesIndex + 2;

	AxBlurTextureMechanism(void);
	virtual ~AxBlurTextureMechanism(void);

	void OnPropertiesChanged();
};

