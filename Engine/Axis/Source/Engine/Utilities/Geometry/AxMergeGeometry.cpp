//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMergeGeometry.h"

#include "..\..\..\Graphics\DeviceIndependent\AxDeviceIndependentMesh.h"

#include "AxCopyGeometry.h"


// Merges two meshes into a new one, incorporating the given transformations to the source meshes
// If the resulting mesh is also used as a source, its original geometry will be put into the final result
// Position, normals and texture coordinates are copied
// Indices are copied
AxMergeGeometry::AxMergeGeometry(AxDeviceMesh &result, AxDeviceMesh &geometry1, AxDeviceMesh &geometry2, AxMatrix &transform1, AxMatrix &transform2)
{
	this->CreateGeometry(result, geometry1, geometry2, transform1, transform2);
}

// Merges two meshes into a new one
// If the resulting mesh is also used as a source, its original geometry will be put into the final result
// Position, normals and texture coordinates are copied
// Indices are copied
AxMergeGeometry::AxMergeGeometry(AxDeviceMesh &result, AxDeviceMesh &geometry1, AxDeviceMesh &geometry2)
{
	AxMatrix identity;
	AxMatrix::CreateIdentity(identity);

	this->CreateGeometry(result, geometry1, geometry2, identity, identity);
}


AxMergeGeometry::~AxMergeGeometry(void)
{
}

void AxMergeGeometry::CreateGeometry(AxDeviceMesh &result, AxDeviceMesh &geometry1, AxDeviceMesh &geometry2, AxMatrix &transform1, AxMatrix &transform2)
{
	AxDeviceMesh *finalResult;

	bool buffered = (&result == &geometry1) || (&result == &geometry2);
	if (buffered)
		finalResult = new AxDeviceIndependentMesh();
	else
		finalResult = &result;

	finalResult->CreateVertexBuffer(geometry1.GetVertexCount() + geometry2.GetVertexCount());

	AxVector3 position;
	AxVector3 normal;
	AxVector2 texCoords;

	int finalVertices = finalResult->GetVertexCount();
	for (int i = 0; i < finalVertices; i++)
	{
		int i2 = i - geometry1.GetVertexCount();
		AxDeviceMesh *source = i2 < 0 ? &geometry1 : &geometry2;
		int sourceIndex = i2 < 0 ? i : i2;
		AxMatrix *transform = i2 < 0 ? &transform1 : &transform2;

		source->GetVertexPosition(sourceIndex, position);
		source->GetVertexNormal(sourceIndex, normal);
		source->GetVertexTexCoords(sourceIndex, texCoords);

		AxVector3::Transform(position, position, *transform);
		AxVector3::TransformNormal(normal, normal, *transform);

		finalResult->SetVertexPosition(i, position);
		finalResult->SetVertexNormal(i, normal);
		finalResult->SetVertexTexCoords(i, texCoords);
	}


	finalResult->CreateIndexBuffer(geometry1.GetIndexCount() + geometry2.GetIndexCount());

	int finalIndices = finalResult->GetIndexCount();
	for (int i = 0; i < finalIndices; i++)
	{
		int i2 = i - geometry1.GetIndexCount();
		AxDeviceMesh *source = i2 < 0 ? &geometry1 : &geometry2;
		int sourceIndex = i2 < 0 ? i : i2;
		int destIndex = i2 < 0 ? source->GetIndex(sourceIndex) : source->GetIndex(sourceIndex) + geometry1.GetVertexCount();

		finalResult->SetIndex(i, destIndex);
	}

	if (buffered)
	{
		AxCopyGeometry(result, *finalResult);
		delete finalResult;
	}
};