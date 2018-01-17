//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTexCoordChannels.h"


AxTexCoordChannels::AxTexCoordChannels(float coordinatesLocationThreshold)
{
	this->coordinatesLocationThreshold = coordinatesLocationThreshold;
	this->multiChannelVerticesCount = 0;
}


AxTexCoordChannels::~AxTexCoordChannels(void)
{
	for (int i = 0; i < this->originalVertexTexCoords.count; i++)
		delete this->originalVertexTexCoords[i];

	for (int i = 0; i < this->faces.count; i++)
		delete this->faces[i];
}


void AxTexCoordChannels::AddFace()
{
	this->faces.Add(new AxTexCoordChannelFace());
}

void AxTexCoordChannels::AddFaceVertexTexCoords(int vertexIndex, AxVector2 texCoords)
{
	for (int i = this->originalVertexTexCoords.count; i <= vertexIndex; i++)
		this->originalVertexTexCoords.Add(new AxVertexTexCoordChannel());

	AxVertexTexCoordChannel *originalVertexTexCoords = this->originalVertexTexCoords[vertexIndex];

	bool isNewVertex = true;
	int faceVertex = 0;
	for (int i = 0; i < originalVertexTexCoords->coords.count; i++)
	{
		if (originalVertexTexCoords->coords[i].DistanceTo(texCoords) < this->coordinatesLocationThreshold)
		{
			isNewVertex = false;
			faceVertex = originalVertexTexCoords->indices[i];
			break;
		}
	}

	if (isNewVertex)
	{
		faceVertex = this->vertexIndices.count;

		originalVertexTexCoords->coords.Add(texCoords);
		originalVertexTexCoords->indices.Add(faceVertex);

		this->vertexIndices.Add(vertexIndex);
		this->vertexTexCoords.Add(texCoords);

		if (originalVertexTexCoords->coords.count == 2)
			this->multiChannelVerticesCount++;
	}


	this->faces[this->faces.count - 1]->indices.Add(faceVertex);
}


int AxTexCoordChannels::GetOriginalVertexIndex(int vertexIndex)
{
	if (vertexIndex >= this->vertexIndices.count)
		return vertexIndex;

	return this->vertexIndices[vertexIndex];
}

int AxTexCoordChannels::GetFaceVertexIndex(int faceIndex, int faceVertexIndex)
{
	if (faceIndex >= this->faces.count)
		return faceVertexIndex;

	if (faceVertexIndex >= this->faces[faceIndex]->indices.count)
		return faceVertexIndex;

	return this->faces[faceIndex]->indices[faceVertexIndex];
}
