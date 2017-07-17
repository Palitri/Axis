//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxConeGeometryMechanism.h"

#include "..\..\Mesh\AxMesh.h"

#include "..\..\..\Utilities\Geometry\AxPrismGeometry.h"


AxConeGeometryMechanism::AxConeGeometryMechanism(void)
{
	this->typeId = AxConeGeometryMechanism::classId;

	this->properties.Add(new AxProperty("Base radius", 10.0f));
    this->properties.Add(new AxProperty("Top radius", 5.0f));
    this->properties.Add(new AxProperty("Height", 20.0f));
    this->properties.Add(new AxProperty("Radial segments", 12));
    this->properties.Add(new AxProperty("Height segments", 1));
    this->properties.Add(new AxProperty("Base cap segments", 0));
    this->properties.Add(new AxProperty("Top cap segments", 0));
    this->properties.Add(new AxProperty("Mesh", 0, AxParameterType_ReferenceMesh));
}


AxConeGeometryMechanism::~AxConeGeometryMechanism(void)
{
}

void AxConeGeometryMechanism::OnPropertiesChanged()
{
	float baseRadius = this->properties[AxConeGeometryMechanism::propertyIndex_BottmRadius]->GetFloat();
	float topRadius = this->properties[AxConeGeometryMechanism::propertyIndex_TopRadius]->GetFloat();
	float height = this->properties[AxConeGeometryMechanism::propertyIndex_Height]->GetFloat();
	int radialSegments = this->properties[AxConeGeometryMechanism::propertyIndex_RadialSegments]->GetInt();
	int heightSegments = this->properties[AxConeGeometryMechanism::propertyIndex_HeightSegments]->GetInt();
	int baseCapSegments = this->properties[AxConeGeometryMechanism::propertyIndex_BottomCapSegments]->GetInt();
	int topCapSegments = this->properties[AxConeGeometryMechanism::propertyIndex_TopCapSegments]->GetInt();
	AxMesh *mesh = (AxMesh*)this->properties[AxConeGeometryMechanism::propertyIndex_Mesh]->GetEffectiveValue();

	if (mesh == 0)
		return;

	AxPrismGeometry(*mesh->deviceMesh, baseRadius, topRadius, height, radialSegments, heightSegments, baseCapSegments, topCapSegments);

    mesh->ComputeBounds();
    mesh->ComputeNormals();
    mesh->ComputeTangents();

    mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
    mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());}
