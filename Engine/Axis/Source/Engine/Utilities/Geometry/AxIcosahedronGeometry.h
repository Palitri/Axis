#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

class AXDLLCLASS AxIcosahedronGeometry
{
public:
	AxIcosahedronGeometry(AxDeviceMesh &result, float radius = 1.0f);
	~AxIcosahedronGeometry(void);
};

