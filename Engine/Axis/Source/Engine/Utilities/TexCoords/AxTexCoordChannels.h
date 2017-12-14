#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Primitive\AxVector2.h"

#include "..\..\..\Tools\AxList.h"

struct AxTexCoordChannel
{
	AxList<AxVector2> coords;
};

struct AxTexCoordChannelFace
{
	AxList<int> indices;
};

class AXDLLCLASS AxTexCoordChannels
{
public:
	float coordinatesLocationThreshold;
	int multiChannelVerticesCount;

	AxList<AxTexCoordChannel*> vertexTexCoordChannels;
	AxList<AxTexCoordChannelFace*> faces;
	AxList<int> vertexIndices;

	AxTexCoordChannels(float coordinatesLocationThreshold = 0.01f);
	virtual ~AxTexCoordChannels(void);

	void AddFace();
	void AddFaceVertexTexCoords(int vertexIndex, AxVector2 texCoords);

};

