#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

#include "..\..\..\Tools\AxList.h"

struct AXDLLCLASS AxEdge
{
	int index1, index2;
	int subIndex;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxEdge>;

class AXDLLCLASS AxGeosphereGeometry
{
private:
	AxList<AxEdge> edges;
	AxDeviceMesh *result;
	int currentIndex, currentVertexIndex;
	float radius;

	void SubDivideTriangle(int triangleIndex, int subLevel);
	int GetEdgeSubIndex(int edgeIndex1, int edgeIndex2);

public:
	AxGeosphereGeometry(AxDeviceMesh &result, float radius, int subDivisions);
	~AxGeosphereGeometry(void);

};

