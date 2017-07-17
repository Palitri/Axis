#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Primitive\AxMatrix.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

class AXDLLCLASS AxTransformTexCoords
{
public:
	AxTransformTexCoords(AxDeviceMesh &result, AxMatrix &transformation);
	virtual ~AxTransformTexCoords(void);
};

