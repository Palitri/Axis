//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFile3dsMesh.h"

AxFile3dsMesh::AxFile3dsMesh(void)
{
	this->objectType = AxFile3dsObjectType_Mesh;
	
	localCoordSystem._11 = 1.0f;
	localCoordSystem._12 = 0.0f;
	localCoordSystem._13 = 0.0f;
	localCoordSystem._14 = 0.0f;
	localCoordSystem._21 = 0.0f;
	localCoordSystem._22 = 1.0f;
	localCoordSystem._23 = 0.0f;
	localCoordSystem._24 = 0.0f;
	localCoordSystem._31 = 0.0f;
	localCoordSystem._32 = 0.0f;
	localCoordSystem._33 = 1.0f;
	localCoordSystem._34 = 0.0f;
	localCoordSystem._41 = 0.0f;
	localCoordSystem._42 = 0.0f;
	localCoordSystem._43 = 0.0f;
	localCoordSystem._44 = 1.0f;

	this->vertices = 0;
	this->faces = 0;

	this->numVertices = 0;
	this->numFaces = 0;
}


AxFile3dsMesh::~AxFile3dsMesh(void)
{
	if (this->vertices != 0)
		delete this->vertices;

	if (this->faces != 0)
		delete this->faces;
}

void AxFile3dsMesh::InverseTransformation()
{
    float idet = 1.0f / (this->localCoordSystem._11 * this->localCoordSystem._22 * this->localCoordSystem._33 +
						 this->localCoordSystem._12 * this->localCoordSystem._23 * this->localCoordSystem._31 +
						 this->localCoordSystem._13 * this->localCoordSystem._21 * this->localCoordSystem._32 -
						 this->localCoordSystem._11 * this->localCoordSystem._23 * this->localCoordSystem._32 -
						 this->localCoordSystem._12 * this->localCoordSystem._21 * this->localCoordSystem._33 -
						 this->localCoordSystem._13 * this->localCoordSystem._22 * this->localCoordSystem._31);

    AxFile3dsObjectTransformation inv;
    inv._11 = idet * (this->localCoordSystem._22 * this->localCoordSystem._33 - this->localCoordSystem._23 * this->localCoordSystem._32);
    inv._12 = idet * (this->localCoordSystem._13 * this->localCoordSystem._32 - this->localCoordSystem._12 * this->localCoordSystem._33);
    inv._13 = idet * (this->localCoordSystem._12 * this->localCoordSystem._23 - this->localCoordSystem._13 * this->localCoordSystem._22);
    inv._21 = idet * (this->localCoordSystem._23 * this->localCoordSystem._31 - this->localCoordSystem._21 * this->localCoordSystem._33);
    inv._22 = idet * (this->localCoordSystem._11 * this->localCoordSystem._33 - this->localCoordSystem._13 * this->localCoordSystem._31);
    inv._23 = idet * (this->localCoordSystem._13 * this->localCoordSystem._21 - this->localCoordSystem._11 * this->localCoordSystem._23);
    inv._31 = idet * (this->localCoordSystem._21 * this->localCoordSystem._32 - this->localCoordSystem._22 * this->localCoordSystem._31);
    inv._32 = idet * (this->localCoordSystem._12 * this->localCoordSystem._31 - this->localCoordSystem._11 * this->localCoordSystem._32);
    inv._33 = idet * (this->localCoordSystem._11 * this->localCoordSystem._22 - this->localCoordSystem._12 * this->localCoordSystem._21);

    for (int i = 0; i < this->numVertices; i++)
    {
		float x = this->vertices[i].x - this->localCoordSystem._41;
		float y = this->vertices[i].y - this->localCoordSystem._42;
		float z = this->vertices[i].z - this->localCoordSystem._43;

		this->vertices[i].x = x * inv._11 +
							  y * inv._21 +
						      z * inv._31;
        this->vertices[i].y = x * inv._12 +
                              y * inv._22 +
                              z * inv._32;
        this->vertices[i].z = x * inv._13 +
                              y * inv._23 +
                              z * inv._33;
    }
}