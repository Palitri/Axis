//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDeviceMesh.h"


AxDeviceMesh::AxDeviceMesh(void)
{
}


AxDeviceMesh::~AxDeviceMesh(void)
{
}

void AxDeviceMesh::CopyFrom(AxDeviceMesh *source)
{
	if (source == this)
		return;
	
	int numIndices = source->GetIndexCount();
	this->CreateIndexBuffer(numIndices);
	for (int i = 0; i < numIndices; i++)
	{
		this->SetIndex(i, source->GetIndex(i));
	}
	this->UpdateIndices(0, numIndices);

	int numVertices = source->GetVertexCount();
	this->CreateVertexBuffer(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		AxVector3 v3;
		AxVector2 v2;

		source->GetVertexPosition(i, v3);
		this->SetVertexPosition(i, v3);

		source->GetVertexNormal(i, v3);
		this->SetVertexNormal(i, v3);

		source->GetVertexTexCoords(i, v2);
		this->SetVertexTexCoords(i, v2);
	}
	this->UpdateVertices(0, numVertices);
}