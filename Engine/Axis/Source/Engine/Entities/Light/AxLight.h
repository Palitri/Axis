#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

class AXDLLCLASS AxLight :
	public AxResource
{
public:
	static const int classId = (AxResourceType_Light << 16) | 0;

	static const int propertyIndex_Color		= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Intensity	= AxResource::propertyIndex_ChildPropertiesIndex + 1;

public:
	AxLight(void);
	virtual ~AxLight(void);
};

