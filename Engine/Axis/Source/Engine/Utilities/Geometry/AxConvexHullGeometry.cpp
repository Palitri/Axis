//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxConvexHullGeometry.h"


// Creates triangle indices in the given mesh, based on the mesh's vertices, so that the resulting triangle geometry is a convex hull which contains all the vertices in its volume
// The mesh has to have the position of its vertices filled
// New mesh indices are created
AxConvexHullGeometry::AxConvexHullGeometry(AxDeviceMesh &result)
{
	int numVertices = result.GetVertexCount();
	if (numVertices < 3)
		return;

	int vertexIndex = 0;

	// Put a triangle
	this->indices.Add(vertexIndex++);
	this->indices.Add(vertexIndex++);
	this->indices.Add(vertexIndex++);

	int triangleIndex = 1;

	this->modifyIndices.SetSize(numVertices);
	for (int i = 0; i < this->modifyIndices.count; i++)
		this->modifyIndices[i] = 0;

	// Begin building convex hull
	while(vertexIndex < numVertices)
	{
		for (int triangleIndex = 0; triangleIndex < this->indices.count; triangleIndex += 3)
		{
			if (this->IsOutside(vertexIndex, triangleIndex))
			{
				this->modifyIndices[result.GetIndex(triangleIndex)]++;
				this->modifyIndices[result.GetIndex(triangleIndex)]++;
				this->modifyIndices[result.GetIndex(triangleIndex)]++;
			}
		}

		vertexIndex++;
	}
}


AxConvexHullGeometry::~AxConvexHullGeometry(void)
{
}

bool AxConvexHullGeometry::IsOutside(int vertexIndex, int triangleIndex)
{
	return true;
}
