#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

class AXDLLCLASS AxTorusGeometry
{
public:
	AxTorusGeometry(AxDeviceMesh &result, int radialSegments, int ringSegments, int twists, float radius, float ringRadius, float spiralSize, float helicalHeight, float turns, bool weldEnds, bool weldSurface);
	~AxTorusGeometry(void);
};

