#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

#include "..\..\Primitive\AxVector3.h"

class AXDLLCLASS AxPlaneGeometry
{
private:
	void CreateGeometry(AxDeviceMesh &result, int xSegments, int ySegments, AxVector3 &xAxis, AxVector3 &yAxis);

public:
	AxPlaneGeometry(AxDeviceMesh &result, int xSegments, int ySegments, AxVector3 &xAxis, AxVector3 &yAxis);
	AxPlaneGeometry(AxDeviceMesh &result, int xSegments, int ySegments);
	~AxPlaneGeometry(void);
};

