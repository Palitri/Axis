//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSubMesh.h"


AxSubMesh::AxSubMesh(int originalVertexCount)
{
	this->originalVertexCount = originalVertexCount;
	this->originalVertexReferences = new int[this->originalVertexCount];
	for (int i = 0; i < this->originalVertexCount; i++)
	{
		this->originalVertexReferences[i] = -1;
	}
}


AxSubMesh::~AxSubMesh(void)
{
	delete[] this->originalVertexReferences;

	for (int i = 0; i < this->faces.count; i++)
	{
		delete this->faces[i];
	}
}

int AxSubMesh::AddFace()
{
	this->faces.Add(new AxSubMeshFace());
	
	return this->faces.count - 1;
}

int AxSubMesh::AddFaceVertex(int faceIndex, int originalVertexIndex)
{
	int vertexIndex = this->originalVertexReferences[originalVertexIndex];
	if (vertexIndex == -1)
	{
		vertexIndex = this->vertexIndices.count;
		this->vertexIndices.Add(originalVertexIndex);
		this->originalVertexReferences[originalVertexIndex] = vertexIndex;
	}

	this->faces[faceIndex]->indices.Add(vertexIndex);

	return vertexIndex;
}


void AxSubMesh::Reset()
{
	for (int i = 0; i < this->originalVertexCount; i++)
	{
		this->originalVertexReferences[i] = -1;
	}

	for (int i = 0; i < this->faces.count; i++)
	{
		delete this->faces[i];
	}
	this->faces.SetSize(0);
}