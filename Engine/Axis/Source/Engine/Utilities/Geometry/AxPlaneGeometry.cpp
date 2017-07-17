//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPlaneGeometry.h"


// Creates a plane with the given number of segments, along the given vectors
// New mesh vertices are created with their position filled
// New mesh indices are created
AxPlaneGeometry::AxPlaneGeometry(AxDeviceMesh &result, int xSegments, int ySegments, AxVector3 &xAxis, AxVector3 &yAxis)
{
	this->CreateGeometry(result, xSegments, ySegments, xAxis, yAxis);
}

// Creates a plane with the given number of segments
// New mesh vertices are created with their position filled
// New mesh indices are created
AxPlaneGeometry::AxPlaneGeometry(AxDeviceMesh &result, int xSegments, int ySegments)
{
	AxVector3 xAxis(1.0f, 0.0f, 0.0f);
	AxVector3 yAxis(0.0f, 1.0f, 0.0f);

	this->CreateGeometry(result, xSegments, ySegments, xAxis, yAxis);
}

AxPlaneGeometry::~AxPlaneGeometry(void)
{
}


void AxPlaneGeometry::CreateGeometry(AxDeviceMesh &result, int xSegments, int ySegments, AxVector3 &xAxis, AxVector3 &yAxis)
{
	result.CreateVertexBuffer((xSegments + 1) * (ySegments + 1));

	AxVector3 pos, xPos, yPos;
	int index = 0;
	for (int y = 0; y <= ySegments; y++)
	{
		float v = y / (float)ySegments;
		AxVector3::Scale(yPos, yAxis, v - 0.5f);

		for (int x = 0; x <= xSegments; x++)
		{
			float u = x / (float)xSegments;
			AxVector3::Scale(xPos, xAxis, u - 0.5f);

			AxVector3::Add(pos, xPos, yPos);

			result.SetVertexTexCoords(index, AxVector2(u, v));
			result.SetVertexPosition(index++, pos);
		}
	}


	result.CreateIndexBuffer(xSegments * ySegments * 2 * 3);

	index = 0;
	for (int y = 0; y < ySegments; y++)
	{
		for (int x = 0; x < xSegments; x++)
		{
			int line0 = y * (xSegments + 1) + x;
			int line1 = (y + 1) * (xSegments + 1) + x;

			result.SetIndex(index++, line0);
			result.SetIndex(index++, line0 + 1);
			result.SetIndex(index++, line1);

			result.SetIndex(index++, line0 + 1);
			result.SetIndex(index++, line1 + 1);
			result.SetIndex(index++, line1);
		}
	}
}
