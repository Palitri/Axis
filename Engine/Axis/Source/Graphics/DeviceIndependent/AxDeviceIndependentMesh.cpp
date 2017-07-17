//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDeviceIndependentMesh.h"


AxDeviceIndependentMesh::AxDeviceIndependentMesh(void)
	: AxDeviceMesh()
{
	this->positions = 0;
	this->normals = 0;
	this->texCoords = 0;
	this->boneIndices = 0;
	this->boneWeights = 0;
	this->tangents = 0;
	this->biTangents = 0;


	this->indices = 0;


	this->numVertices = 0;
	this->numIndices = 0;
}


AxDeviceIndependentMesh::~AxDeviceIndependentMesh(void)
{
	this->DeleteVertexBuffer();
	this->DeleteIndexBuffer();
}

bool AxDeviceIndependentMesh::CreateVertexBuffer(int numVertices)
{
	this->DeleteVertexBuffer();
	
	this->positions = new float[numVertices * 3];
	this->normals = new float[numVertices * 3];
	this->texCoords = new float[numVertices * 2];
	this->boneIndices = new float[numVertices * 4];
	this->boneWeights = new float[numVertices * 4];
	this->tangents = new float[numVertices * 3];
	this->biTangents = new float[numVertices * 3];

	this->numVertices = numVertices;

	return true;
}

bool AxDeviceIndependentMesh::CreateIndexBuffer(int numIndices)
{
	this->DeleteIndexBuffer();

	this->indices = new unsigned short[numIndices];

	this->numIndices = numIndices;
	
	return true;
}

bool AxDeviceIndependentMesh::DeleteVertexBuffer()
{
	if (this->positions != 0)
		delete[] this->positions;
	if (this->normals != 0)
		delete[] this->normals;
	if (this->texCoords != 0)
		delete[] this->texCoords;
	if (this->boneIndices != 0)
		delete[] this->boneIndices;
	if (this->boneWeights != 0)
		delete[] this->boneWeights;
	if (this->tangents != 0)
		delete[] this->tangents;
	if (this->biTangents != 0)
		delete[] this->biTangents;

	this->positions = 0;
	this->normals = 0;
	this->texCoords = 0;
	this->boneIndices = 0;
	this->boneWeights = 0;
	this->tangents = 0;
	this->biTangents = 0;

	this->numVertices = 0;

	return true;
}

bool AxDeviceIndependentMesh::DeleteIndexBuffer()
{
	if (this->indices != 0)
		delete[] this->indices;

	this->indices = 0;

	this->numIndices = 0;

	return true;
}

bool AxDeviceIndependentMesh::UpdateVertices(int offset, int count)
{
	return true;
}

bool AxDeviceIndependentMesh::UpdateIndices(int offset, int count)
{
	return true;
}

int AxDeviceIndependentMesh::GetVertexCount()
{
	return this->numVertices;
}

int AxDeviceIndependentMesh::GetIndexCount()
{
	return this->numIndices;
}

int AxDeviceIndependentMesh::GetPrimitiveCount()
{
	return this->numIndices / 3;
}

void AxDeviceIndependentMesh::GetVertexPosition(int index, AxVector3 &result)
{
	index *= 3;
	result.x = this->positions[index++];
	result.y = this->positions[index++];
	result.z = this->positions[index];
}

void AxDeviceIndependentMesh::SetVertexPosition(int index, AxVector3 &value)
{
	index *= 3;
	this->positions[index++] = value.x;
	this->positions[index++] = value.y;
	this->positions[index] = value.z;
}

void AxDeviceIndependentMesh::GetVertexNormal(int index, AxVector3 &result)
{
	index *= 3;
	result.x = this->normals[index++];
	result.y = this->normals[index++];
	result.z = this->normals[index];
}

void AxDeviceIndependentMesh::SetVertexNormal(int index, AxVector3 &value)
{
	index *= 3;
	this->normals[index++] = value.x;
	this->normals[index++] = value.y;
	this->normals[index] = value.z;
}

void AxDeviceIndependentMesh::GetVertexTexCoords(int index, AxVector2 &result)
{
	index *= 2;
	result.x = this->texCoords[index++];
	result.y = this->texCoords[index];
}

void AxDeviceIndependentMesh::SetVertexTexCoords(int index, AxVector2 &value)
{
	index *= 2;
	this->texCoords[index++] = value.x;
	this->texCoords[index] = value.y;
}

void AxDeviceIndependentMesh::GetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	index *= 4;
	boneIndices.x = this->boneIndices[index];
	boneWeights.x = this->boneWeights[index++];

	boneIndices.y = this->boneIndices[index];
	boneWeights.y = this->boneWeights[index++];

	boneIndices.z = this->boneIndices[index];
	boneWeights.z = this->boneWeights[index++];

	boneIndices.w = this->boneIndices[index];
	boneWeights.w = this->boneWeights[index++];
}

void AxDeviceIndependentMesh::SetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	index *= 4;
	this->boneIndices[index] = boneIndices.x;
	this->boneWeights[index++] = boneWeights.x;

	this->boneIndices[index] = boneIndices.y;
	this->boneWeights[index++] = boneWeights.y;

	this->boneIndices[index] = boneIndices.z;
	this->boneWeights[index++] = boneWeights.z;

	this->boneIndices[index] = boneIndices.w;
	this->boneWeights[index] = boneWeights.w;
}

void AxDeviceIndependentMesh::GetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
	index *= 3;
	tangent.x = this->tangents[index];
	biTangent.x = this->biTangents[index++];

	tangent.y = this->tangents[index];
	biTangent.y = this->biTangents[index++];

	tangent.z = this->tangents[index];
	biTangent.z = this->biTangents[index];
}

void AxDeviceIndependentMesh::SetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
	index *= 3;
	this->tangents[index] = tangent.x;
	this->biTangents[index++] = biTangent.x;

	this->tangents[index] = tangent.y;
	this->biTangents[index++] = biTangent.y;

	this->tangents[index] = tangent.z;
	this->biTangents[index] = biTangent.z;
}


int AxDeviceIndependentMesh::GetIndex(int index)
{
	return this->indices[index];
}

void AxDeviceIndependentMesh::SetIndex(int index, int value)
{
	this->indices[index] = value;
}
