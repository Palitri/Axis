#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\AxPropertyChangeNotifiedMechanism.h"

class AXDLLCLASS AxCheckerTextureMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	static const int classId = AxMechanism::classId | 305;

	static const int propertyIndex_Width			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Height			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_SquareSize		= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Color1			= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_Color2			= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_Texture			= AxResource::propertyIndex_ChildPropertiesIndex + 5;

	AxCheckerTextureMechanism(void);
	virtual ~AxCheckerTextureMechanism(void);

	void OnPropertiesChanged();
};

