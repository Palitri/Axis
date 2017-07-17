//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "SrMesh.h"

#include "..\..\..\Tools\AxMem.h"

SrMesh::SrMesh(void)
{
	this->indices = 0;
	this->vertices = 0;
	this->transformedVertices = 0;
	this->visiblePrimitives = 0;

	this->numIndices = 0;
	this->numVertices = 0;
}


SrMesh::SrMesh(SrMesh &original)
{
	this->SetIndexCount(original.numIndices);
	this->SetVertexCount(original.numVertices);

	AxMem::Copy(this->indices, original.indices, original.numIndices * sizeof(int));
	AxMem::Copy(this->vertices, original.vertices, original.numVertices * sizeof(SrVertex));
}

SrMesh::~SrMesh(void)
{
	this->SetIndexCount(0);
	this->SetVertexCount(0);
}

// TODO: consider only a SetCount method for AxDeviceMesh and all mesh-related objects. The creation and destruction of the buffers will happen simply at create/destroy of on object

void SrMesh::SetIndexCount(int numIndices)
{
	if (numIndices == this->numIndices)
		return;
	
	
	if (this->indices != 0)
		delete[] this->indices;

	if (this->visiblePrimitives != 0)
		delete[] this->visiblePrimitives;


	if (numIndices != 0)
	{
		this->indices = new int[numIndices];
		this->visiblePrimitives = new int[numIndices / 3];
	}
	else
	{
		this->indices = 0;
		this->visiblePrimitives = 0;
	}


	this->numIndices = numIndices;
}

void SrMesh::SetVertexCount(int numVertices)
{
	if (numVertices == this->numVertices)
		return;

	
	if (this->vertices != 0)
		delete[] this->vertices;

	if (this->transformedVertices != 0)
		delete[] this->transformedVertices;


	if (numVertices != 0)
	{
		this->vertices = new SrVertex[numVertices];
		this->transformedVertices = new SrVertex[numVertices];
	}
	else
	{
		this->vertices = 0;
		this->transformedVertices = 0;
	}


	this->numVertices = numVertices;
}
