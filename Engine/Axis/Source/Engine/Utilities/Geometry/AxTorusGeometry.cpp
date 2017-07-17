//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTorusGeometry.h"

#include "..\..\..\Tools\AxMath.h"
#include "..\..\Primitive\AxVector3.h"

AxTorusGeometry::AxTorusGeometry(AxDeviceMesh &result, int radialSegments, int ringSegments, int twists, float radius, float ringRadius, float spiralSize, float helicalHeight, float turns, bool weldEnds, bool weldSurface)
{
	int radialRingsCount = weldEnds ? radialSegments : radialSegments + 1;
	int ringVertexCount = weldSurface ? ringSegments : ringSegments + 1;

	result.CreateVertexBuffer(radialRingsCount * ringVertexCount);

	int index = 0;
	for (int radialIndex = 0; radialIndex < radialRingsCount; radialIndex++)
	{
		float u = (float)radialIndex / (float)radialSegments;
		float uUnit = (float)radialIndex / (float)(radialRingsCount + 1);

		float radialAngle = turns * AxMath::Pi2 * u;

		AxVector3 radialVector(AxMath::Cos(radialAngle), 0.0f, AxMath::Sin(radialAngle));
		AxVector3 ringPos;
		AxVector3::Scale(ringPos, radialVector, radius + (spiralSize * uUnit));
		ringPos.y = uUnit * helicalHeight;

		float twistOffsetAngle = AxMath::Pi2 * (float)twists * (float)radialIndex / (float)(radialRingsCount - 1);
		for (int ringIndex = 0; ringIndex < ringVertexCount; ringIndex++)
		{
			float v = (float)ringIndex / (float)ringSegments;

			float ringAngle = twistOffsetAngle + AxMath::Pi2 * v;

			float ringX = AxMath::Cos(ringAngle);
			float ringY = AxMath::Sin(ringAngle);

			AxVector3 ringVector(ringX * radialVector.x, ringY, ringX * radialVector.z);
			AxVector3::Scale(ringVector, ringVector, ringRadius);
			AxVector3::Add(ringVector, ringVector, ringPos);

			result.SetVertexPosition(index, ringVector);
			result.SetVertexTexCoords(index, AxVector2(u, v));

			index++;
		}
	}

	result.CreateIndexBuffer(radialSegments * ringSegments * 2 * 3);

	index = 0;
	for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
	{
		int radialIndex0 = radialIndex * ringVertexCount;
		int radialIndex1 = ((radialIndex + 1) % radialRingsCount) * ringVertexCount;
		for (int ringIndex = 0; ringIndex < ringSegments; ringIndex++)
		{
			int ringIndex1 = (ringIndex + 1) % ringVertexCount;
			
			int ring00 = radialIndex0 + ringIndex;
			int ring01 = radialIndex0 + ringIndex1;
			int ring10 = radialIndex1 + ringIndex;
			int ring11 = radialIndex1 + ringIndex1;

			result.SetIndex(index++, ring00);
			result.SetIndex(index++, ring10);
			result.SetIndex(index++, ring01);

			result.SetIndex(index++, ring10);
			result.SetIndex(index++, ring11);
			result.SetIndex(index++, ring01);
		}
	}
}


AxTorusGeometry::~AxTorusGeometry(void)
{
}
