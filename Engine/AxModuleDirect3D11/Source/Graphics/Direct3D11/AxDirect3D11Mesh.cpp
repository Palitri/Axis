//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirect3D11Mesh.h"


AxDirect3D11Mesh::AxDirect3D11Mesh(void)
{
	this->vertexBuffer = 0;
	this->indexBuffer = 0;

	this->positionOffset = 0;
	this->normalOffset = this->positionOffset + 3 * 4;
	this->texCoordsOffset = this->normalOffset + 3 * 4;
	this->bonesOffset = this->texCoordsOffset + 2 * 4;
	this->tangentsOffset = this->bonesOffset + (4 + 4) * 4;

	this->vertexSize = (3 + 3 + 2 + 4 + 4 + 3 + 3) * 4;

	this->vertexData = 0;
	this->vertexCount = 0;

	this->indexData = 0;
	this->indexCount = 0;
}

AxDirect3D11Mesh::~AxDirect3D11Mesh(void)
{
	this->DeleteVertexBuffer();
	this->DeleteIndexBuffer();
}

bool AxDirect3D11Mesh::CreateVertexBuffer(int numVertices)
{
	this->DeleteVertexBuffer();

	this->vertexCount = numVertices;
	this->vertexData = new char[this->vertexSize * this->vertexCount];

	return true;
}

bool AxDirect3D11Mesh::CreateIndexBuffer(int numIndices)
{
	this->DeleteIndexBuffer();

	this->indexCount = numIndices;
	this->indexData = new int[this->indexCount];

	return true;
}

bool AxDirect3D11Mesh::DeleteVertexBuffer()
{
	if (this->vertexData != 0)
	{
		delete[] this->vertexData;
		this->vertexData = 0;
	}
	
	this->vertexCount = 0;

	if (this->vertexBuffer != 0)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = 0;
	}

	return true;
}

bool AxDirect3D11Mesh::DeleteIndexBuffer()
{
	if (this->indexData != 0)
	{
		delete[] this->indexData;
		this->indexData = 0;
	}
	
	this->indexCount = 0;

	if (this->indexBuffer != 0)
	{
		this->indexBuffer->Release();
		this->indexBuffer = 0;
	}

	return true;
}

bool AxDirect3D11Mesh::UpdateVertices(int offset, int count)
{
	if (this->vertexBuffer == 0)
	{
		D3D11_BUFFER_DESC bufferDescription;
		AxMem::Zero(&bufferDescription, sizeof(bufferDescription));
		bufferDescription.ByteWidth = this->vertexCount * this->vertexSize;
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDescription.CPUAccessFlags = 0;
		bufferDescription.MiscFlags = 0;
		bufferDescription.StructureByteStride = this->vertexSize;

		if (bufferDescription.ByteWidth == 0)
			return false;

		D3D11_SUBRESOURCE_DATA initData;
		AxMem::Zero(&initData, sizeof(initData));
		initData.pSysMem = this->vertexData;
		
		if (FAILED(this->context->device->CreateBuffer(&bufferDescription, &initData, &this->vertexBuffer)))
			return false;
	}

	return true;
}

bool AxDirect3D11Mesh::UpdateIndices(int offset, int count)
{
	if (this->indexBuffer == 0)
	{
		D3D11_BUFFER_DESC bufferDescription;
		AxMem::Zero(&bufferDescription, sizeof(bufferDescription));
		bufferDescription.ByteWidth = this->indexCount * 4;
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDescription.CPUAccessFlags = 0;
		bufferDescription.MiscFlags = 0;
		bufferDescription.StructureByteStride = 4;

		if (bufferDescription.ByteWidth == 0)
			return false;

		D3D11_SUBRESOURCE_DATA initData;
		AxMem::Zero(&initData, sizeof(initData));
		initData.pSysMem = this->indexData;
		
		if (FAILED(this->context->device->CreateBuffer(&bufferDescription, &initData, &this->indexBuffer)))
			return false;
	}

	return true;
}

int AxDirect3D11Mesh::GetVertexCount()
{
	return this->vertexCount;
}

int AxDirect3D11Mesh::GetIndexCount()
{
	return this->indexCount;
}

int AxDirect3D11Mesh::GetPrimitiveCount()
{
	return this->indexCount / 3;
}

void AxDirect3D11Mesh::GetVertexPosition(int index, AxVector3 &result)
{
	float *pos = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->positionOffset);
	result.x = *pos++;
	result.y = *pos++;
	result.z = *pos;
}

void AxDirect3D11Mesh::SetVertexPosition(int index, AxVector3 &value)
{
	float *pos = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->positionOffset);
	*pos++ = value.x;
	*pos++ = value.y;
	*pos = value.z;
}

void AxDirect3D11Mesh::GetVertexNormal(int index, AxVector3 &result)
{
	float *normal = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->normalOffset);
	result.x = *normal++;
	result.y = *normal++;
	result.z = *normal;
}

void AxDirect3D11Mesh::SetVertexNormal(int index, AxVector3 &value)
{
	float *normal = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->normalOffset);
	*normal++ = value.x;
	*normal++ = value.y;
	*normal = value.z;
}

void AxDirect3D11Mesh::GetVertexTexCoords(int index, AxVector2 &result)
{
	float *texCoords = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->texCoordsOffset);
	result.x = *texCoords++;
	result.y = -*texCoords++;
}

void AxDirect3D11Mesh::SetVertexTexCoords(int index, AxVector2 &value)
{
	float *texCoords = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->texCoordsOffset);
	*texCoords++ = value.x;
	*texCoords++ = -value.y;
}

void AxDirect3D11Mesh::GetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	float *bone = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->bonesOffset);
	boneIndices.x = *bone++;
	boneIndices.y = *bone++;
	boneIndices.z = *bone++;
	boneIndices.w = *bone++;
	
	boneWeights.x = *bone++;
	boneWeights.y = *bone++;
	boneWeights.z = *bone++;
	boneWeights.w = *bone;
}

void AxDirect3D11Mesh::SetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	float *bone = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->bonesOffset);
	*bone++ = boneIndices.x;
	*bone++ = boneIndices.y;
	*bone++ = boneIndices.z;
	*bone++ = boneIndices.w;

	*bone++ = boneWeights.x;
	*bone++ = boneWeights.y;
	*bone++ = boneWeights.z;
	*bone = boneWeights.w;
}

void AxDirect3D11Mesh::GetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
	float *tangents = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->tangentsOffset);
	tangent.x = *tangents++;
	tangent.y = *tangents++;
	tangent.z = *tangents++;
	
	biTangent.x = *tangents++;
	biTangent.y = *tangents++;
	biTangent.z = *tangents;
}

void AxDirect3D11Mesh::SetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
	float *tangents = (float*)((unsigned int)this->vertexData + index * this->vertexSize + this->tangentsOffset);
	*tangents++ = tangent.x;
	*tangents++ = tangent.y;
	*tangents++ = tangent.z;

	*tangents++ = biTangent.x;
	*tangents++ = biTangent.y;
	*tangents = biTangent.z;
}

int AxDirect3D11Mesh::GetIndex(int index)
{
	return this->indexData[index];
}

void AxDirect3D11Mesh::SetIndex(int index, int value)
{
	this->indexData[index] = value;
}
