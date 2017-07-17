//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSphereGeometry.h"

#include "..\..\..\Tools\AxMath.h"

#include "..\..\Primitive\AxVector2.h"
#include "..\..\Primitive\AxVector3.h"

AxSphereGeometry::AxSphereGeometry(AxDeviceMesh &result, float radius, int radialSegments, int heightSegments, float twist, bool weldSurface)
{
	int heightLinesCount = heightSegments + 1;
	int radialLinesCount = weldSurface ? radialSegments : radialSegments + 1;

	if ((radialSegments < 2) || (heightSegments < 2))
		return;

	result.CreateVertexBuffer(radialLinesCount * heightLinesCount);

	int index = 0;
	for (int heightLineIndex = 0; heightLineIndex < heightLinesCount; heightLineIndex++)
	{
		float v = (float)heightLineIndex / (float)heightSegments;

		float heightAngle = (v - 0.5f) * AxMath::Pi;

		float heightCircleHeight = AxMath::Sin(heightAngle) * radius;
		float heightCircleRadius = AxMath::Cos(heightAngle) * radius;

		for (int radialLineIndex = 0; radialLineIndex < radialLinesCount; radialLineIndex++)
		{
			float u = (float)radialLineIndex / (float)radialSegments;

			float radialAngle = (u + twist * v) * AxMath::Pi2;

			result.SetVertexPosition(index, AxVector3(AxMath::Cos(radialAngle) * heightCircleRadius, heightCircleHeight, AxMath::Sin(radialAngle) * heightCircleRadius));
			result.SetVertexTexCoords(index, AxVector2(u, v));

			index++;
		}
	}


	result.CreateIndexBuffer(heightSegments * radialSegments * 2 * 3);

	index = 0;
	for (int heightIndex = 0; heightIndex < heightSegments; heightIndex++)
	{
		for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
		{
			int line00 = heightIndex * radialLinesCount + radialIndex;
			int line01 = heightIndex * radialLinesCount + (radialIndex + 1) % radialLinesCount;
			int line10 = (heightIndex + 1) * radialLinesCount + radialIndex;
			int line11 = (heightIndex + 1) * radialLinesCount + (radialIndex + 1) % radialLinesCount;

			result.SetIndex(index++, line00);
			result.SetIndex(index++, line01);
			result.SetIndex(index++, line10);

			result.SetIndex(index++, line01);
			result.SetIndex(index++, line11);
			result.SetIndex(index++, line10);
		}
	}
}


AxSphereGeometry::~AxSphereGeometry(void)
{
}
