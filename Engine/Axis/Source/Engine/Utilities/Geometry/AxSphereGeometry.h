#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

class AXDLLCLASS AxSphereGeometry
{
public:
	AxSphereGeometry(AxDeviceMesh &result, float radius, int radialSegments, int heightSegments, float twist, bool weldSurface);
	virtual ~AxSphereGeometry(void);
};

