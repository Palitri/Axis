#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxList.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<int>;

class AXDLLCLASS AxConvexHullGeometry
{
private:
	AxDeviceMesh *result;
	AxList<int> indices, modifyIndices;

	bool IsOutside(int vertexIndex, int triangleIndex);

public:
	AxConvexHullGeometry(AxDeviceMesh &result);
	~AxConvexHullGeometry(void);
};

