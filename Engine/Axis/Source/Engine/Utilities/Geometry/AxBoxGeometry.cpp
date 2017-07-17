//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxBoxGeometry.h"

#include "AxPlaneGeometry.h"
#include "AxMergeGeometry.h"
#include "AxCopyGeometry.h"

#include "..\TexCoords\AxTransformTexCoords.h"

#include "..\..\..\Graphics\DeviceIndependent\AxDeviceIndependentMesh.h"

// Creates a box with the given number of segments, along the given vectors
// New mesh vertices are created with their position filled
// New mesh indices are created
AxBoxGeometry::AxBoxGeometry(AxDeviceMesh &result, int xSegments, int ySegments, int zSegments, bool texture3x2, AxVector3 &xAxis, AxVector3 &yAxis, AxVector3 &zAxis)
{
	this->CreateGeometry(result, xSegments, ySegments, zSegments, texture3x2, xAxis, yAxis, zAxis);
}


// Creates a box with the given number of segments
// New mesh vertices are created with their position filled
// New mesh indices are created
AxBoxGeometry::AxBoxGeometry(AxDeviceMesh &result, int xSegments, int ySegments, int zSegments, bool texture3x2)
{
	AxVector3 xAxis(1.0f, 0.0f, 0.0f), 
			  yAxis(0.0f, 1.0f, 0.0f),
			  zAxis(0.0f, 0.0f, 1.0f);
		
	this->CreateGeometry(result, xSegments, ySegments, zSegments, texture3x2, xAxis, yAxis, zAxis);
}

AxBoxGeometry::~AxBoxGeometry(void)
{
}

void AxBoxGeometry::CreateGeometry(AxDeviceMesh &result, int xSegments, int ySegments, int zSegments, bool texture3x2, AxVector3 &xAxis, AxVector3 &yAxis, AxVector3 &zAxis)
{
	AxDeviceIndependentMesh plane;
	AxMatrix transform, textureScaling, identity;
	AxVector3 x, y, up;

	float threshold = 0.01f;
	float thresholdOffset = threshold / 2.0f;

	AxMatrix::CreateIdentity(identity);
	if (texture3x2)
		AxMatrix::CreateScaling(textureScaling, 1.0f / 3.0f, 1.0f / 2.0f - threshold, 1.0f);

	// Left
	x = zAxis.Invert();
	y = yAxis;
	up = xAxis.Scale(-0.5f);
	AxPlaneGeometry(plane, zSegments, ySegments, x, y);
	if (texture3x2)
	{
		AxMatrix::CreateTranslation(transform, 0.0f / 3.0f, 1.0f / 2.0f + thresholdOffset, 0.0f);
		AxMatrix::Multiply(transform, textureScaling, transform);
		AxTransformTexCoords(plane, transform);
	}
	AxMatrix::CreateTranslation(transform, up);
	AxCopyGeometry(result, plane, transform);

	// Back
	x = xAxis;
	y = yAxis;
	up = zAxis.Scale(-0.5f);
	AxPlaneGeometry(plane, xSegments, ySegments, x, y);
	if (texture3x2)
	{
		AxMatrix::CreateTranslation(transform, 1.0f / 3.0f, 1.0f / 2.0f + thresholdOffset, 0.0f);
		AxMatrix::Multiply(transform, textureScaling, transform);
		AxTransformTexCoords(plane, transform);
	}
	AxMatrix::CreateTranslation(transform, up);
	AxMergeGeometry(result, result, plane, identity, transform);

	// Right
	x = zAxis;
	y = yAxis;
	up = xAxis.Scale(0.5f);
	AxPlaneGeometry(plane, zSegments, ySegments, x, y);
	if (texture3x2)
	{
		AxMatrix::CreateTranslation(transform, 2.0f / 3.0f, 1.0f / 2.0f + thresholdOffset, 0.0f);
		AxMatrix::Multiply(transform, textureScaling, transform);
		AxTransformTexCoords(plane, transform);
	}
	AxMatrix::CreateTranslation(transform, up);
	AxMergeGeometry(result, result, plane, identity, transform);


	//AxMatrix::CreateScaling(textureScaling, - 1.0f / 3.0f, 1.0f / 2.0f, 1.0f);

	// Top
	x = zAxis;
	y = xAxis.Invert();
	up = yAxis.Scale(0.5f);
	AxPlaneGeometry(plane, xSegments, zSegments, x, y);
	if (texture3x2)
	{
		AxMatrix::CreateTranslation(transform, 0.0f / 3.0f, 0.0f + thresholdOffset, 0.0f);
		AxMatrix::Multiply(transform, textureScaling, transform);
		AxTransformTexCoords(plane, transform);
	}
	AxMatrix::CreateTranslation(transform, up);
	AxMergeGeometry(result, result, plane, identity, transform);

	// Front
	x = yAxis.Invert();
	y = xAxis.Invert();
	up = zAxis.Scale(0.5f);
	AxPlaneGeometry(plane, xSegments, ySegments, x, y);
	if (texture3x2)
	{
		AxMatrix::CreateTranslation(transform, 1.0f / 3.0f, 0.0f + thresholdOffset, 0.0f);
		AxMatrix::Multiply(transform, textureScaling, transform);
		AxTransformTexCoords(plane, transform);
	}
	AxMatrix::CreateTranslation(transform, up);
	AxMergeGeometry(result, result, plane, identity, transform);

	// Bottom
	x = zAxis.Invert();
	y = xAxis.Invert();
	up = yAxis.Scale(-0.5f);
	AxPlaneGeometry(plane, xSegments, zSegments, x, y);
	if (texture3x2)
	{
		AxMatrix::CreateTranslation(transform, 2.0f / 3.0f, 0.0f + thresholdOffset, 0.0f);
		AxMatrix::Multiply(transform, textureScaling, transform);
		AxTransformTexCoords(plane, transform);
	}
	AxMatrix::CreateTranslation(transform, up);
	AxMergeGeometry(result, result, plane, identity, transform);
}