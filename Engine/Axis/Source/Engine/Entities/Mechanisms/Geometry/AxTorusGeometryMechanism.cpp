//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTorusGeometryMechanism.h"

#include "..\..\Mesh\AxMesh.h"

#include "..\..\..\Utilities\Geometry\AxTorusGeometry.h"

#include "..\..\..\..\Tools\AxMath.h"


AxTorusGeometryMechanism::AxTorusGeometryMechanism(void)
{
	this->typeId = AxTorusGeometryMechanism::classId;

    this->properties.Add(new AxProperty("Radius", 10.0f));
    this->properties.Add(new AxProperty("Ring radius", 1.0f));
    this->properties.Add(new AxProperty("Spiral size", 0.0f));
    this->properties.Add(new AxProperty("Helical height", 0.0f));
    this->properties.Add(new AxProperty("Turns", 1.0f));
	this->properties.Add(new AxProperty("Radial segments", 16));
    this->properties.Add(new AxProperty("Ring segments", 8));
    this->properties.Add(new AxProperty("Start cap segments", 0));
    this->properties.Add(new AxProperty("End cap segments", 0));
    this->properties.Add(new AxProperty("Twists", 0));
    this->properties.Add(new AxProperty("Weld ends", true));
    this->properties.Add(new AxProperty("Weld surface", true));
    this->properties.Add(new AxProperty("Mesh", 0, AxParameterType_ReferenceMesh));
}


AxTorusGeometryMechanism::~AxTorusGeometryMechanism(void)
{
}

void AxTorusGeometryMechanism::OnPropertiesChanged()
{
	float radius = this->properties[AxTorusGeometryMechanism::propertyIndex_Radius]->GetFloat();
	float ringRadius = this->properties[AxTorusGeometryMechanism::propertyIndex_RingRadius]->GetFloat();
	float spiralSize = this->properties[AxTorusGeometryMechanism::propertyIndex_SpiralSize]->GetFloat();
	float helicalHeight = this->properties[AxTorusGeometryMechanism::propertyIndex_HelicalHeight]->GetFloat();
	float turns = this->properties[AxTorusGeometryMechanism::propertyIndex_Turns]->GetFloat();
	int radialSegments = this->properties[AxTorusGeometryMechanism::propertyIndex_RadialSegments]->GetInt();
	int ringSegments = this->properties[AxTorusGeometryMechanism::propertyIndex_RingSegments]->GetInt();
	int startCapSegments = this->properties[AxTorusGeometryMechanism::propertyIndex_StartCapSegments]->GetInt();
	int endCapSegments = this->properties[AxTorusGeometryMechanism::propertyIndex_EndCapSegments]->GetInt();
	int twists = this->properties[AxTorusGeometryMechanism::propertyIndex_Twists]->GetInt();
	bool weldEnds = this->properties[AxTorusGeometryMechanism::propertyIndex_WeldEnds]->GetBool();
	bool weldSurface = this->properties[AxTorusGeometryMechanism::propertyIndex_WeldSurface]->GetBool();
	AxMesh *mesh = (AxMesh*)this->properties[AxTorusGeometryMechanism::propertyIndex_Mesh]->GetEffectiveValue();

	if (mesh == 0)
		return;

	AxTorusGeometry(*mesh->deviceMesh, radialSegments, ringSegments, twists, radius, ringRadius, spiralSize, helicalHeight, turns, weldEnds, weldSurface);

    mesh->ComputeBounds();
    mesh->ComputeNormals();
    mesh->ComputeTangents();

    mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
    mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());
}
