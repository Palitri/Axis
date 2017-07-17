#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

class AXDLLCLASS AxPlanarTexCoords
{
public:
	AxPlanarTexCoords(AxDeviceMesh &result, AxVector3 &planeX, AxVector3 &planeY);
	~AxPlanarTexCoords(void);
};

