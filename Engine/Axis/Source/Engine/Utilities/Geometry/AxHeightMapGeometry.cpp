//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxHeightMapGeometry.h"

// Performs a height map modification to the given mesh, based on its normals and the given map
// The given mesh must have vertices with their position and normal set
// The position of the mesh's vertices is modified
// TODO: consider different pixel formats!
AxHeightMapGeometry::AxHeightMapGeometry(AxDeviceMesh &result, AxImage &map, AxVector4 &colorFilter, float heightScaling)
{
	int numVertices = result.GetVertexCount();
	for (int i = 0; i < numVertices; i++)
	{
		AxVector3 position, normal;
		AxVector2 texCoords;

		result.GetVertexPosition(i, position);
		result.GetVertexNormal(i, normal);
		result.GetVertexTexCoords(i, texCoords);

		float rgba[4];
		map.pixelFormat.GetRGBAUnitValues(rgba,  map.GetSample(texCoords.x, texCoords.y, 0));
		//float heightFactor = (rgba[3] * colorFilter.x + rgba[2] * colorFilter.y + rgba[1] * colorFilter.z + rgba[0] * colorFilter.w) / (colorFilter.x + colorFilter.y + colorFilter.z + colorFilter.w);
		float heightFactor = (rgba[0] * colorFilter.x + rgba[1] * colorFilter.y + rgba[2] * colorFilter.z + rgba[3] * colorFilter.w) / (colorFilter.x + colorFilter.y + colorFilter.z + colorFilter.w);

		AxVector3::Scale(normal, normal, heightScaling * heightFactor);
		AxVector3::Add(position, position, normal);

		result.SetVertexPosition(i, position);
	}

}


AxHeightMapGeometry::~AxHeightMapGeometry(void)
{
}
