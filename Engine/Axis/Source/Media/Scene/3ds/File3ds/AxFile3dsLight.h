#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxFile3dsPrimitiveTypes.h"

#include "AxFile3dsBaseObject.h"

class AXDLLCLASS AxFile3dsLight :
	public AxFile3dsBaseObject
{
public:
	bool active;
	AxFile3dsVector position;
	AxFile3dsColorFloat color;
	bool spotLight;
	AxFile3dsVector spotlight_Target;
	float spotlight_Hotspot, spotlight_Falloff;

	AxFile3dsLight(void);
	~AxFile3dsLight(void);
};

