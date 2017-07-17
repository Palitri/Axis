#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

#include "..\..\Primitive\AxMatrix.h"

class AXDLLCLASS AxMergeGeometry
{
public:
	AxMergeGeometry(AxDeviceMesh &result, AxDeviceMesh &geometry1, AxDeviceMesh &geometry2, AxMatrix &transform1, AxMatrix &transform2);
	AxMergeGeometry(AxDeviceMesh &result, AxDeviceMesh &geometry1, AxDeviceMesh &geometry2);
	~AxMergeGeometry(void);

	void CreateGeometry(AxDeviceMesh &result, AxDeviceMesh &geometry1, AxDeviceMesh &geometry2, AxMatrix &transform1, AxMatrix &transform2);
};

