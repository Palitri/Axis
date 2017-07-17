//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPrismGeometry.h"

#include "..\..\..\Tools\AxMath.h"

// Constructs a cone with height along the Y-Axis
AxPrismGeometry::AxPrismGeometry(AxDeviceMesh &result, float bottomRadius, float topRadius, float height, int radialSegments, int heightSegments, int bottomCapSegments, int topCapSegments)
{
	if (radialSegments < 3)
		return;

	int vertexCount = radialSegments * (heightSegments + 1);
	if (bottomCapSegments > 0)
		vertexCount += radialSegments * bottomCapSegments + 1;
	if (topCapSegments > 0)
		vertexCount += radialSegments * topCapSegments + 1;


	int facesCount = radialSegments * heightSegments * 2;
	if (bottomCapSegments > 0)
		facesCount += radialSegments * ((bottomCapSegments - 1) * 2 + 1);
	if (topCapSegments > 0)
		facesCount += radialSegments * ((topCapSegments - 1) * 2 + 1);

	result.CreateVertexBuffer(vertexCount);
	result.CreateIndexBuffer(facesCount * 3);


	int vertexOffset = 0;
	int vertexIndex = 0;

	int indexIndex = 0;

	// Cone body
	// Vertices
	for (int segmentIndex = 0; segmentIndex <= heightSegments; segmentIndex ++)
	{
		float heightUnit = (float)segmentIndex / (float)heightSegments;
		float segmentRadius = bottomRadius + (topRadius - bottomRadius) * heightUnit;
		float segmentHeight = height * (heightUnit - 0.5f);

		for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
		{
			float angle = AxMath::Pi2 * (float)radialIndex / (float)radialSegments;

			AxVector3 pos = AxVector3(AxMath::Cos(angle) * segmentRadius, segmentHeight, AxMath::Sin(angle) * segmentRadius);

			result.SetVertexPosition(vertexIndex++, pos);
		}
	}

	// Faces
	for (int segmentIndex = 0; segmentIndex < heightSegments; segmentIndex++)
	{
		for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
		{
			result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + radialIndex);
			result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
			result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);

			result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
			result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + (radialIndex + 1) % radialSegments);
			result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);
		}
	}

	vertexOffset = vertexIndex;

	// Cone bottom cap
	if (bottomCapSegments > 0)
	{
		// Vertices
		for (int segmentIndex = 0; segmentIndex < bottomCapSegments; segmentIndex++)
		{
			float radiusUnit = 1.0f - (float)segmentIndex / (float)bottomCapSegments;
			float segmentRadius = bottomRadius * radiusUnit;
			float segmentHeight = - height / 2.0f;
			for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
			{
				float angle = AxMath::Pi2 * (float)radialIndex / (float)radialSegments;

				AxVector3 pos = AxVector3(AxMath::Cos(angle) * segmentRadius, segmentHeight, AxMath::Sin(angle) * segmentRadius);

				result.SetVertexPosition(vertexIndex++, pos);
			}

		}
		AxVector3 pos = AxVector3(0.0f, - height / 2.0f, 0.0f);
		result.SetVertexPosition(vertexIndex++, pos);

		// Faces
		for (int segmentIndex = 0; segmentIndex < bottomCapSegments - 1; segmentIndex++)
		{
			for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
			{
				result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
				result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + radialIndex);
				result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);

				result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + (radialIndex + 1) % radialSegments);
				result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
				result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);
			}
		}

		for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
		{
			result.SetIndex(indexIndex++, vertexOffset + (bottomCapSegments - 1) * radialSegments + (radialIndex + 1) % radialSegments);
			result.SetIndex(indexIndex++, vertexOffset + (bottomCapSegments - 1) * radialSegments + radialIndex);
			result.SetIndex(indexIndex++, vertexOffset + bottomCapSegments * radialSegments);
		}

	}


	vertexOffset = vertexIndex;

	// Cone top cap
	if (topCapSegments > 0)
	{
		// Vertices
		for (int segmentIndex = 0; segmentIndex < topCapSegments; segmentIndex++)
		{
			float radiusUnit = 1.0f - (float)segmentIndex / (float)topCapSegments;
			float segmentRadius = topRadius * radiusUnit;
			float segmentHeight = height / 2.0f;
			for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
			{
				float angle = AxMath::Pi2 * (float)radialIndex / (float)radialSegments;

				AxVector3 pos = AxVector3(AxMath::Cos(angle) * segmentRadius, segmentHeight, AxMath::Sin(angle) * segmentRadius);

				result.SetVertexPosition(vertexIndex++, pos);
			}

		}
		AxVector3 pos = AxVector3(0.0f, height / 2.0f, 0.0f);
		result.SetVertexPosition(vertexIndex++, pos);

		// Faces
		for (int segmentIndex = 0; segmentIndex < topCapSegments - 1; segmentIndex++)
		{
			for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
			{
				result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + radialIndex);
				result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
				result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);

				result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
				result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + (radialIndex + 1) % radialSegments);
				result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);
			}
		}

		for (int radialIndex = 0; radialIndex < radialSegments; radialIndex++)
		{
			result.SetIndex(indexIndex++, vertexOffset + (topCapSegments - 1) * radialSegments + radialIndex);
			result.SetIndex(indexIndex++, vertexOffset + (topCapSegments - 1) * radialSegments + (radialIndex + 1) % radialSegments);
			result.SetIndex(indexIndex++, vertexOffset + topCapSegments * radialSegments);
		}

	}
}


AxPrismGeometry::~AxPrismGeometry(void)
{
}
