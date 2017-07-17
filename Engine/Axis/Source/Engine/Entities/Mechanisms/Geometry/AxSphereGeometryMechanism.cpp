//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSphereGeometryMechanism.h"

#include "..\..\Mesh\AxMesh.h"

#include "..\..\..\Utilities\Geometry\AxSphereGeometry.h"


AxSphereGeometryMechanism::AxSphereGeometryMechanism(void)
{
	this->typeId = AxSphereGeometryMechanism::classId;

	this->properties.Add(new AxProperty("Radius", 10.0f));
    this->properties.Add(new AxProperty("Radial segments", 10));
    this->properties.Add(new AxProperty("Height segments", 10));
    this->properties.Add(new AxProperty("Twist", 0.0f));
    this->properties.Add(new AxProperty("Weld surface", false));
    this->properties.Add(new AxProperty("Mesh", 0, AxParameterType_ReferenceMesh));
}


AxSphereGeometryMechanism::~AxSphereGeometryMechanism(void)
{
}

void AxSphereGeometryMechanism::OnPropertiesChanged()
{
	float radius = this->properties[AxSphereGeometryMechanism::propertyIndex_Radius]->GetFloat();
	int radialSegments = this->properties[AxSphereGeometryMechanism::propertyIndex_RadialSegments]->GetInt();
	int heightSegments = this->properties[AxSphereGeometryMechanism::propertyIndex_HeightSegments]->GetInt();
	float twist = this->properties[AxSphereGeometryMechanism::propertyIndex_Twist]->GetFloat();
	bool weldSurface = this->properties[AxSphereGeometryMechanism::propertyIndex_WeldSurface]->GetBool();
	AxMesh *mesh = (AxMesh*)this->properties[AxSphereGeometryMechanism::propertyIndex_Mesh]->GetEffectiveValue();

	if (mesh == 0)
		return;

	AxSphereGeometry(*mesh->deviceMesh, radius, radialSegments, heightSegments, twist, weldSurface);

    mesh->ComputeBounds();
    mesh->ComputeNormals();
    mesh->ComputeTangents();

    mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
    mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());
}
