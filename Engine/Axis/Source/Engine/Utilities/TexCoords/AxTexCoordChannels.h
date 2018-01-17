#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Primitive\AxVector2.h"

#include "..\..\..\Tools\AxList.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxVector2>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<int>;

struct AXDLLCLASS AxVertexTexCoordChannel
{
	AxList<AxVector2> coords;
	AxList<int> indices;
};

struct AXDLLCLASS AxTexCoordChannelFace
{
	AxList<int> indices;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxVertexTexCoordChannel*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxTexCoordChannelFace*>;


class AXDLLCLASS AxTexCoordChannels
{
public:
	float coordinatesLocationThreshold;
	int multiChannelVerticesCount;

	AxList<AxVertexTexCoordChannel*> originalVertexTexCoords;
	AxList<AxTexCoordChannelFace*> faces;
	AxList<int> vertexIndices;
	AxList<AxVector2> vertexTexCoords;

	AxTexCoordChannels(float coordinatesLocationThreshold = 0.01f);
	virtual ~AxTexCoordChannels(void);

	void AddFace();
	void AddFaceVertexTexCoords(int vertexIndex, AxVector2 texCoords);

	int GetOriginalVertexIndex(int originalIndex);
	int GetFaceVertexIndex(int faceIndex, int faceVertexIndex);
//	AxVector2 GetFaceVertexIndex
};

