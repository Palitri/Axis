//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPlaneGeometryMechanism.h"

#include "..\..\Mesh\AxMesh.h"

#include "..\..\..\Utilities\Geometry\AxPlaneGeometry.h"

#include "..\..\..\Utilities\TexCoords\AxPlanarTexCoords.h"

AxPlaneGeometryMechanism::AxPlaneGeometryMechanism(void)
{
	this->typeId = AxPlaneGeometryMechanism::classId;

	this->properties.Add(new AxProperty("Width", 10.0f));
    this->properties.Add(new AxProperty("Length", 10.0f));
    this->properties.Add(new AxProperty("Width segments", 1));
    this->properties.Add(new AxProperty("Length segments", 1));
    this->properties.Add(new AxProperty("Mesh", 0, AxParameterType_ReferenceMesh));
}


AxPlaneGeometryMechanism::~AxPlaneGeometryMechanism(void)
{
}

void AxPlaneGeometryMechanism::OnPropertiesChanged()
{
	float width = this->properties[AxPlaneGeometryMechanism::propertyIndex_Width]->GetFloat();
	float length = this->properties[AxPlaneGeometryMechanism::propertyIndex_Length]->GetFloat();
	int widthSegments = this->properties[AxPlaneGeometryMechanism::propertyIndex_WidthSegments]->GetInt();
	int lengthSegments = this->properties[AxPlaneGeometryMechanism::propertyIndex_LengthSegments]->GetInt();
	AxMesh *mesh = (AxMesh*)this->properties[AxPlaneGeometryMechanism::propertyIndex_Mesh]->GetEffectiveValue();

	if (mesh == 0)
		return;

    // Create a flat plane
    AxPlaneGeometry(*mesh->deviceMesh, widthSegments, lengthSegments, AxVector3(width, 0, 0), AxVector3(0, 0, length));
    AxPlanarTexCoords(*mesh->deviceMesh, AxVector3(width, 0, 0), AxVector3(0, 0, length));

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
