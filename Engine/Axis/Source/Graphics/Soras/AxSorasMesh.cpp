//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSorasMesh.h"

#include "Engine\Primitive\SrUnit.h"

AxSorasMesh::AxSorasMesh(void)
{
	this->sorasMesh = new SrMesh();
}

AxSorasMesh::~AxSorasMesh(void)
{
	delete this->sorasMesh;
}

bool AxSorasMesh::CreateVertexBuffer(int numVertices)
{
	this->sorasMesh->SetVertexCount(numVertices);

	return true;
}

bool AxSorasMesh::CreateIndexBuffer(int numIndices)
{
	this->sorasMesh->SetIndexCount(numIndices);

	return true;
}

bool AxSorasMesh::DeleteVertexBuffer()
{
	this->sorasMesh->SetVertexCount(0);

	return true;
}

bool AxSorasMesh::DeleteIndexBuffer()
{
	this->sorasMesh->SetIndexCount(0);

	return true;
}

bool AxSorasMesh::UpdateVertices(int offset, int count)
{
	// no need to update

	return true;
}

bool AxSorasMesh::UpdateIndices(int offset, int count)
{
	// no need to update

	return true;
}

int AxSorasMesh::GetVertexCount()
{
	return this->sorasMesh->numVertices;
}

int AxSorasMesh::GetIndexCount()
{
	return this->sorasMesh->numIndices;
}

int AxSorasMesh::GetPrimitiveCount()
{
	return this->sorasMesh->numIndices / 3;
}

void AxSorasMesh::GetVertexPosition(int index, AxVector3 &result)
{
	result = *(AxVector3*)(&this->sorasMesh->vertices[index].position);
}

void AxSorasMesh::SetVertexPosition(int index, AxVector3 &value)
{
	this->sorasMesh->vertices[index].position = *(SrVector3*)(&value);
}

void AxSorasMesh::GetVertexNormal(int index, AxVector3 &result)
{
	result = *(AxVector3*)(&this->sorasMesh->vertices[index].normal);
}

void AxSorasMesh::SetVertexNormal(int index, AxVector3 &value)
{
	this->sorasMesh->vertices[index].normal = *(SrVector3*)(&value);
}

void AxSorasMesh::GetVertexTexCoords(int index, AxVector2 &result)
{
	result.x = SrUnit::ToFloat(this->sorasMesh->vertices[index].texCoords.x);
	result.y = SrUnit::ToFloat(this->sorasMesh->vertices[index].texCoords.y);
}

void AxSorasMesh::SetVertexTexCoords(int index, AxVector2 &value)
{
	this->sorasMesh->vertices[index].texCoords.x = SrUnit::FromFloat(value.x);
	this->sorasMesh->vertices[index].texCoords.y = SrUnit::FromFloat(value.y);
}

void AxSorasMesh::GetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	boneIndices.x = (float)this->sorasMesh->vertices[index].boneIndices.x;
	boneIndices.y = (float)this->sorasMesh->vertices[index].boneIndices.y;
	boneIndices.z = (float)this->sorasMesh->vertices[index].boneIndices.z;
	boneIndices.w = (float)this->sorasMesh->vertices[index].boneIndices.w;

	boneWeights.x = this->sorasMesh->vertices[index].boneWeights.x;
	boneWeights.y = this->sorasMesh->vertices[index].boneWeights.y;
	boneWeights.z = this->sorasMesh->vertices[index].boneWeights.z;
	boneWeights.w = this->sorasMesh->vertices[index].boneWeights.w;
}

void AxSorasMesh::SetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	this->sorasMesh->vertices[index].boneIndices.x = (int)AxMath::Round(boneIndices.x);
	this->sorasMesh->vertices[index].boneIndices.y = (int)AxMath::Round(boneIndices.y);
	this->sorasMesh->vertices[index].boneIndices.z = (int)AxMath::Round(boneIndices.z);
	this->sorasMesh->vertices[index].boneIndices.w = (int)AxMath::Round(boneIndices.w);

	this->sorasMesh->vertices[index].boneWeights.x = boneWeights.x;
	this->sorasMesh->vertices[index].boneWeights.y = boneWeights.y;
	this->sorasMesh->vertices[index].boneWeights.z = boneWeights.z;
	this->sorasMesh->vertices[index].boneWeights.w = boneWeights.w;
}

void AxSorasMesh::GetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
}

void AxSorasMesh::SetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
}

int AxSorasMesh::GetIndex(int index)
{
	return this->sorasMesh->indices[index];
}

void AxSorasMesh::SetIndex(int index, int value)
{
	this->sorasMesh->indices[index] = value;
}

