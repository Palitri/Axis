//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxHeightmapGeometryMechanism.h"

#include "..\..\Textures\AxTexture2D.h"
#include "..\..\Mesh\AxMesh.h"

#include "..\..\..\Primitive\AxVector4.h"

#include "..\..\..\Utilities\Geometry\AxPlaneGeometry.h"
#include "..\..\..\Utilities\Geometry\AxHeightMapGeometry.h"

#include "..\..\..\Utilities\TexCoords\AxPlanarTexCoords.h"

#include "..\..\..\Utilities\Normals\AxNormalsGenerator.h"


AxHeightmapGeometryMechanism::AxHeightmapGeometryMechanism(void)
	: AxPropertyChangeNotifiedMechanism()
{
	this->typeId = AxHeightmapGeometryMechanism::classId;

    this->properties.Add(new AxProperty("Width", 100.0f));
    this->properties.Add(new AxProperty("Length", 100.0f));
    this->properties.Add(new AxProperty("Height", 25.0f));
    this->properties.Add(new AxProperty("Width segments", 100));
    this->properties.Add(new AxProperty("Length segments", 100));
    this->properties.Add(new AxProperty("Color filter", AxVector4(1.0f, 1.0f, 1.0f, 0.0f)));
	this->properties.Add(new AxProperty("Height map", 0, AxParameterType_ReferenceTexture));
    this->properties.Add(new AxProperty("Mesh", 0, AxParameterType_ReferenceMesh));
}


AxHeightmapGeometryMechanism::~AxHeightmapGeometryMechanism(void)
{
}

void AxHeightmapGeometryMechanism::OnPropertiesChanged()
{
	float width = this->properties[AxHeightmapGeometryMechanism::propertyIndex_Width]->GetFloat();
	float length = this->properties[AxHeightmapGeometryMechanism::propertyIndex_Length]->GetFloat();
	float height = this->properties[AxHeightmapGeometryMechanism::propertyIndex_Height]->GetFloat();
	int widthSegments = this->properties[AxHeightmapGeometryMechanism::propertyIndex_WidthSegments]->GetInt();
	int lengthSegments = this->properties[AxHeightmapGeometryMechanism::propertyIndex_LengthSegments]->GetInt();
	AxVector4 colorFilter = this->properties[AxHeightmapGeometryMechanism::propertyIndex_ColorFilter]->GetColor();
	AxTexture2D *heightMap = (AxTexture2D*)this->properties[AxHeightmapGeometryMechanism::propertyIndex_HeightMap]->GetEffectiveValue();
	AxMesh *mesh = (AxMesh*)this->properties[AxHeightmapGeometryMechanism::propertyIndex_Mesh]->GetEffectiveValue();

	if ((heightMap == 0) || (mesh == 0))
		return;

    // Create a flat plane
    AxPlaneGeometry(*mesh->deviceMesh, widthSegments, lengthSegments, AxVector3(width, 0, 0), AxVector3(0, 0, length));
    AxPlanarTexCoords(*mesh->deviceMesh, AxVector3(width, 0, 0), AxVector3(0, 0, length));
    AxNormalsGenerator(*mesh->deviceMesh);

    // Heighten the flat plane along its normals
    AxImage *heightMapImage = heightMap->ToImage();
    AxHeightMapGeometry(*mesh->deviceMesh, *heightMapImage, colorFilter, height);
    delete heightMapImage;

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
