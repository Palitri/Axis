//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxCopyGeometry.h"

// Copies the source mesh into the result. 
// A transform parameter can be given to be applied to the resulting mesh. If omitted, the mesh is copied as is
// Position, normal and texture coordinates are copied
// Indices are copied
AxCopyGeometry::AxCopyGeometry(AxDeviceMesh &result, AxDeviceMesh &source)
{
	AxMatrix identity;
	AxMatrix::CreateIdentity(identity);

	AxCopyGeometry::CopyGeometry(result, source, identity);
}

// Copies the source mesh into the result. 
// A transform parameter can be given to be applied to the resulting mesh. If omitted, the mesh is copied as is
// Position, normal and texture coordinates are copied
// Indices are copied
AxCopyGeometry::AxCopyGeometry(AxDeviceMesh &result, AxDeviceMesh &source, AxMatrix &transform)
{
	AxCopyGeometry::CopyGeometry(result, source, transform);
}

void AxCopyGeometry::CopyGeometry(AxDeviceMesh &result, AxDeviceMesh &source, AxMatrix &transform)
{
	if ((&result == &source) && (transform.IsIdentity()))
		return;

	int numVertices = source.GetVertexCount();
	result.CreateVertexBuffer(numVertices);

	AxVector3 position;
	AxVector3 normal;
	AxVector2 texCoords;

	AxMatrix normalsTransform;
	AxMatrix::Invert3x3(normalsTransform, transform);
	AxMatrix::Transpose(normalsTransform, normalsTransform);

	for (int i = 0; i < numVertices; i++)
	{
		source.GetVertexPosition(i, position);
		source.GetVertexNormal(i, normal);
		source.GetVertexTexCoords(i, texCoords);

		AxVector3::Transform(position, position, transform);
		AxVector3::TransformNormal(normal, normal, normalsTransform);

		result.SetVertexPosition(i, position);
		result.SetVertexNormal(i, normal);
		result.SetVertexTexCoords(i, texCoords);
	}


	int numIndices = source.GetIndexCount();
	result.CreateIndexBuffer(numIndices);

	for (int i = 0; i < numIndices; i++)
	{
		result.SetIndex(i, source.GetIndex(i));
	}
	 
}


AxCopyGeometry::~AxCopyGeometry(void)
{
}
