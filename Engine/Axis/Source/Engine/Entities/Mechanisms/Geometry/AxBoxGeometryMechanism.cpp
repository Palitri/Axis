//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxBoxGeometryMechanism.h"

#include "..\..\Mesh\AxMesh.h"

#include "..\..\..\Utilities\Geometry\AxBoxGeometry.h"


AxBoxGeometryMechanism::AxBoxGeometryMechanism(void)
{
	this->typeId = AxBoxGeometryMechanism::classId;

	this->properties.Add(new AxProperty("Width", 10.0f));
    this->properties.Add(new AxProperty("Length", 10.0f));
    this->properties.Add(new AxProperty("Height", 10.0f));
    this->properties.Add(new AxProperty("Width segments", 1));
    this->properties.Add(new AxProperty("Length segments", 1));
    this->properties.Add(new AxProperty("Height segments", 1));
    this->properties.Add(new AxProperty("Texture 2x3", false));
    this->properties.Add(new AxProperty("Mesh", 0, AxParameterType_ReferenceMesh));
}


AxBoxGeometryMechanism::~AxBoxGeometryMechanism(void)
{
}

void AxBoxGeometryMechanism::OnPropertiesChanged()
{
	float width = this->properties[AxBoxGeometryMechanism::propertyIndex_Width]->GetFloat();
	float length = this->properties[AxBoxGeometryMechanism::propertyIndex_Length]->GetFloat();
	float height = this->properties[AxBoxGeometryMechanism::propertyIndex_Height]->GetFloat();
	int widthSegments = this->properties[AxBoxGeometryMechanism::propertyIndex_WidthSegments]->GetInt();
	int lengthSegments = this->properties[AxBoxGeometryMechanism::propertyIndex_LengthSegments]->GetInt();
	int heightSegments = this->properties[AxBoxGeometryMechanism::propertyIndex_HeightSegments]->GetInt();
	bool texture2x3 = this->properties[AxBoxGeometryMechanism::propertyIndex_Texture2x3]->GetBool();
	AxMesh *mesh = (AxMesh*)this->properties[AxBoxGeometryMechanism::propertyIndex_Mesh]->GetEffectiveValue();

	if (mesh == 0)
		return;

	AxBoxGeometry(*mesh->deviceMesh, widthSegments, heightSegments, lengthSegments, texture2x3, AxVector3(width, 0.0f, 0.0f), AxVector3(0.0f, height, 0.0f), AxVector3(0.0f, 0.0f, length));

    mesh->ComputeBounds();
    mesh->ComputeNormals();
    mesh->ComputeTangents();

    mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
    mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());

	mesh->serializeVertices = false;
	mesh->serializeNormals = false;
	mesh->serializeTexCoords = false;
	mesh->serializeTangents = false;
	mesh->serializeBones = false;
}
