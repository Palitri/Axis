#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxTransformOperation.h"

class AXDLLCLASS AxTransformLayer
{
public:
	AxTransformOperation operation;
    int propertyFirst, propertyCount;

	AxTransformLayer(void);
	AxTransformLayer(AxTransformOperation operation, int propertyFirst, int propertyCount);
	~AxTransformLayer(void);
};

