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
}


void AxTexCoordChannels::AddFace()
{
	this->faces.Add(new AxTexCoordChannelFace());
}

void AxTexCoordChannels::AddFaceVertexTexCoords(int vertexIndex, AxVector2 texCoords)
{
	for (int i = this->vertexTexCoordChannels.count; i <= vertexIndex; i++)
		this->vertexTexCoordChannels.Add(new AxTexCoordChannel());


	bool isNewVertex = true;
	for (int i = 0; i < this->vertexTexCoordChannels[vertexIndex]->coords.count; i++)
	{
		if (this->vertexTexCoordChannels[vertexIndex]->coords[i].DistanceTo(texCoords) < this->coordinatesLocationThreshold)
		{
			isNewVertex = false;
			break;
		}
	}

	if (isNewVertex)
	{
		this->vertexTexCoordChannels[vertexIndex]->coords.Add(texCoords);

		if (this->vertexTexCoordChannels[vertexIndex]->coords.count == 2)
			this->multiChannelVerticesCount++;

		this->vertexIndices.Add(vertexIndex);
	}


	this->faces[this->faces.count - 1]->indices.Add(this->vertexIndices.count - 1);
}
