//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxGeosphereGeometry.h"

#include "..\..\..\Tools\AxMath.h"

#include "..\..\..\Graphics\DeviceIndependent\AxDeviceIndependentMesh.h"

#include "AxIcosahedronGeometry.h"

// Creates a sphere (geosphere) with the given radius and number of subdivisions, starting from an icosahedron
// New mesh vertices are created with their position filled
// New mesh indices are created
AxGeosphereGeometry::AxGeosphereGeometry(AxDeviceMesh &result, float radius, int subDivisions)
{
	this->result = &result;
	this->radius = radius;

	AxDeviceIndependentMesh icosa;
	AxIcosahedronGeometry(icosa, radius);

	this->result->CreateIndexBuffer(icosa.GetIndexCount() * AxMath::Power(4, subDivisions));
	this->result->CreateVertexBuffer(this->result->GetIndexCount() / 6 + 2);
	
	this->currentVertexIndex = icosa.numVertices;
	this->currentIndex = 0;

	AxVector3 position;
	AxVector3 normal;
	AxVector2 texCoords;

	int numVertices = icosa.GetVertexCount();
	for (int i = 0; i < numVertices; i++)
	{
		icosa.GetVertexPosition(i, position);
		icosa.GetVertexNormal(i, normal);
		icosa.GetVertexTexCoords(i, texCoords);

		this->result->SetVertexPosition(i, position);
		this->result->SetVertexNormal(i, normal);
		this->result->SetVertexTexCoords(i, texCoords);
	}


	int numTriangles = icosa.GetPrimitiveCount();
	int index = 0;
	for (int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
	{
		this->result->SetIndex(this->currentIndex++, icosa.GetIndex(index++));
		this->result->SetIndex(this->currentIndex++, icosa.GetIndex(index++));
		this->result->SetIndex(this->currentIndex++, icosa.GetIndex(index++));

		this->SubDivideTriangle(this->currentIndex - 3, subDivisions);
	}
}


AxGeosphereGeometry::~AxGeosphereGeometry(void)
{
}


void AxGeosphereGeometry::SubDivideTriangle(int triangleIndex, int subLevel)
{
	if (subLevel == 0)
		return;

	int i0 = this->result->GetIndex(triangleIndex);
	int i1 = this->result->GetIndex(triangleIndex + 1);
	int i2 = this->result->GetIndex(triangleIndex + 2);

	int s0 = this->GetEdgeSubIndex(i0, i1);
	int s1 = this->GetEdgeSubIndex(i1, i2);
	int s2 = this->GetEdgeSubIndex(i2, i0);

	this->result->SetIndex(triangleIndex, i0);
	this->result->SetIndex(triangleIndex + 1, s0);
	this->result->SetIndex(triangleIndex + 2, s2);
	this->SubDivideTriangle(triangleIndex, subLevel - 1);

	this->result->SetIndex(this->currentIndex++, i1);
	this->result->SetIndex(this->currentIndex++, s1);
	this->result->SetIndex(this->currentIndex++, s0);
	this->SubDivideTriangle(this->currentIndex - 3, subLevel - 1);

	this->result->SetIndex(this->currentIndex++, i2);
	this->result->SetIndex(this->currentIndex++, s2);
	this->result->SetIndex(this->currentIndex++, s1);
	this->SubDivideTriangle(this->currentIndex - 3, subLevel - 1);

	this->result->SetIndex(this->currentIndex++, s0);
	this->result->SetIndex(this->currentIndex++, s1);
	this->result->SetIndex(this->currentIndex++, s2);
	this->SubDivideTriangle(this->currentIndex - 3, subLevel - 1);
}

int AxGeosphereGeometry::GetEdgeSubIndex(int edgeIndex1, int edgeIndex2)
{
	// Look for existing subvertex
	for (int i = 0; i < this->edges.count; i++)
		if (((this->edges[i].index1 == edgeIndex1) && (this->edges[i].index2 == edgeIndex2)) ||
			((this->edges[i].index1 == edgeIndex2) && (this->edges[i].index2 == edgeIndex1)))
			return this->edges[i].subIndex;

	// Generate subvertex
	AxVector3 v1, v2, vsub;
	this->result->GetVertexPosition(edgeIndex1, v1);
	this->result->GetVertexPosition(edgeIndex2, v2);
	AxVector3::Subtract(vsub, v2, v1);
	AxVector3::Scale(vsub, vsub, 0.5f);
	AxVector3::Add(vsub, v1, vsub);
	AxVector3::SetLength(vsub, vsub, this->radius);
	this->result->SetVertexPosition(this->currentVertexIndex, vsub);

	// Add the edge's subvertex to the list
	AxEdge newEdge;
	newEdge.index1 = edgeIndex1;
	newEdge.index2 = edgeIndex2;
	newEdge.subIndex = this->currentVertexIndex;
	this->edges.Add(newEdge);

	this->currentVertexIndex++;

	return newEdge.subIndex;
}
