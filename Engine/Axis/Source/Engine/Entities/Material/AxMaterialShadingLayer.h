#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Primitive\AxShadingOperation.h"
#include "..\..\Primitive\AxColorOperation.h"

class AXDLLCLASS AxMaterialShadingLayer
{
public:
	AxMaterialShadingLayer(void);
	AxMaterialShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp, int propertyFirst, int propertyCount);
	~AxMaterialShadingLayer(void);

	AxShadingOperation shadingOp;
    AxColorOperation colorOp;
    int propertyFirst, propertyCount;
};

