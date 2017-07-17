//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxGeosphereGeometryMechanism.h"

#include "..\..\Mesh\AxMesh.h"

#include "..\..\..\Utilities\Geometry\AxGeosphereGeometry.h"

#include "..\..\..\Utilities\TexCoords\AxSphericalTexCoords.h"


AxGeosphereGeometryMechanism::AxGeosphereGeometryMechanism(void)
{
	this->typeId = AxGeosphereGeometryMechanism::classId;

	this->properties.Add(new AxProperty("Radius", 10.0f));
    this->properties.Add(new AxProperty("Subdivisions", 2));
    this->properties.Add(new AxProperty("Mesh", 0, AxParameterType_ReferenceMesh));
}


AxGeosphereGeometryMechanism::~AxGeosphereGeometryMechanism(void)
{
}

void AxGeosphereGeometryMechanism::OnPropertiesChanged()
{
	float radius = this->properties[AxGeosphereGeometryMechanism::propertyIndex_Radius]->GetFloat();
	int subDivisions = this->properties[AxGeosphereGeometryMechanism::propertyIndex_Subdivisions]->GetInt();
	AxMesh *mesh = (AxMesh*)this->properties[AxGeosphereGeometryMechanism::propertyIndex_Mesh]->GetEffectiveValue();

	if (mesh == 0)
		return;

	AxGeosphereGeometry(*mesh->deviceMesh, radius, subDivisions);
    AxSphericalTexCoords(*mesh->deviceMesh);

    mesh->ComputeBounds();
    mesh->ComputeNormals();
    mesh->ComputeTangents();

    mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
    mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());
}
