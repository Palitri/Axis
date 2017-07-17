#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"
#include "..\..\Primitive\AxMatrix.h"

class AXDLLCLASS AxCopyGeometry
{
private:
	static void CopyGeometry(AxDeviceMesh &result, AxDeviceMesh &source, AxMatrix &transform);

public:
	AxCopyGeometry(AxDeviceMesh &result, AxDeviceMesh &source);
	AxCopyGeometry(AxDeviceMesh &result, AxDeviceMesh &source, AxMatrix &transform);
	~AxCopyGeometry(void);
};

