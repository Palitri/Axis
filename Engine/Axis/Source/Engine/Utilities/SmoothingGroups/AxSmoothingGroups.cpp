//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSmoothingGroups.h"

AxSmoothingGroups::AxSmoothingGroups(int originalFacesCount, int originalVertexCount)
{
	this->originalVerticesInfo.SetSize(originalVertexCount);
	for (int i = 0; i < this->originalVerticesInfo.count; i++)
	{
		this->originalVerticesInfo[i].accumulatedSmoothingGroups = 0;
		this->originalVerticesInfo[i].indices.EnsureCapacity(4);
	}

	this->vertexIndices.EnsureCapacity(originalVertexCount);

	this->faces.SetSize(originalFacesCount);
	for (int i = 0; i < this->faces.count; i++)
	{
		this->faces[i].smoothingGroups = 0;
	}
}


AxSmoothingGroups::~AxSmoothingGroups(void)
{
}

void AxSmoothingGroups::SetFaceSmoothingGroups(int faceIndex, unsigned long long smoothingGroupFlags)
{
	this->faces[faceIndex].smoothingGroups = smoothingGroupFlags;
}

int AxSmoothingGroups::AddFaceVertex(int faceIndex, int originalVertexIndex)
{
	AxSmoothingGroupsFace *face = &this->faces[faceIndex];

	AxSmoothingGroupsVertex vertex;
	vertex.accumulatedSmoothingGroups = face->smoothingGroups;

	if ((vertex.accumulatedSmoothingGroups & this->originalVerticesInfo[originalVertexIndex].accumulatedSmoothingGroups) == 0)
	{
		int newVertexIndex = this->vertexIndices.count;
		vertex.index = originalVertexIndex;
		face->indices.Add(newVertexIndex);
		this->vertexIndices.Add(vertex);
		this->originalVerticesInfo[originalVertexIndex].indices.Add(newVertexIndex);
	}
	else
	{
		int indicesCount = this->originalVerticesInfo[originalVertexIndex].indices.count;
		for (int i = 0; i < indicesCount; i++)
		{
			vertex.index = this->originalVerticesInfo[originalVertexIndex].indices[i];
			if ((vertex.accumulatedSmoothingGroups & this->vertexIndices[vertex.index].accumulatedSmoothingGroups) != 0)
			{
				this->vertexIndices[vertex.index].accumulatedSmoothingGroups |= vertex.accumulatedSmoothingGroups;
				face->indices.Add(vertex.index);
				break;
			}
		}
	}

	this->originalVerticesInfo[originalVertexIndex].accumulatedSmoothingGroups |= vertex.accumulatedSmoothingGroups;

	return vertex.index;
}
