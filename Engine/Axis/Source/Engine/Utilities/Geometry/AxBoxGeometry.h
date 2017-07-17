#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

#include "..\..\Primitive\AxVector3.h"

class AXDLLCLASS AxBoxGeometry
{
private:
	void CreateGeometry(AxDeviceMesh &result, int xSegments, int ySegments, int zSegments, bool texture3x2, AxVector3 &xAxis, AxVector3 &yAxis, AxVector3 &zAxis);

public:
	AxBoxGeometry(AxDeviceMesh &result, int xSegments, int ySegments, int zSegments, bool texture3x2, AxVector3 &xAxis, AxVector3 &yAxis, AxVector3 &zAxis);
	AxBoxGeometry(AxDeviceMesh &result, int xSegments, int ySegments, int zSegments, bool texture3x2);
	~AxBoxGeometry(void);
};

