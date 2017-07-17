//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTransformTexCoords.h"

#include "..\..\Primitive\AxVector2.h"

AxTransformTexCoords::AxTransformTexCoords(AxDeviceMesh &result, AxMatrix &transformation)
{
	int vertexCount = result.GetVertexCount();
	for (int i = 0; i < vertexCount; i++)
	{
		AxVector2 texCoords;
		result.GetVertexTexCoords(i, texCoords);
		AxVector2::Transform(texCoords, texCoords, transformation);
		result.SetVertexTexCoords(i, texCoords);
	}
}


AxTransformTexCoords::~AxTransformTexCoords(void)
{
}
