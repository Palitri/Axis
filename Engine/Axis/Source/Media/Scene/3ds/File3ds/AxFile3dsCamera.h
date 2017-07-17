#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxFile3dsPrimitiveTypes.h"

#include "AxFile3dsBaseObject.h"

class AXDLLCLASS AxFile3dsCamera :
	public AxFile3dsBaseObject
{
public:
	AxFile3dsVector position, target;
	float bank, lens;

	AxFile3dsCamera(void);
	~AxFile3dsCamera(void);
};

