//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGLMesh.h"


AxOpenGLMesh::AxOpenGLMesh(void)
	: AxDeviceMesh()
{
    this->verticesCount = 0;
    
    this->vertexPositionElementsCount = 3;
    this->verticesPositions = 0;
    this->verticesPositionsBuffer = 0;
    
    this->vertexNormalElementsCount = 3;
    this->verticesNormals = 0;
    this->verticesNormalsBuffer = 0;
    
    this->vertexTexCoordsElementsCount = 2;
    this->verticesTexCoords = 0;
    this->verticesTexCoordsBuffer = 0;

    this->vertexBoneIndicesElementsCount = 4;
    this->verticesBoneIndices = 0;
    this->verticesBoneIndicesBuffer = 0;
    
    this->vertexBoneWeightsElementsCount = 4;
    this->verticesBoneWeights = 0;
    this->verticesBoneWeightsBuffer = 0;
    
    this->vertexTangentElementsCount = 3;
    this->verticesTangents = 0;
    this->verticesTangentBuffer = 0;

    this->vertexBitangentElementsCount = 3;
    this->verticesBitangents = 0;
    this->verticesBitangentBuffer = 0;
    
    this->indicesCount = 0;

    this->indices = 0;
    this->indicesBuffer = 0;
}


AxOpenGLMesh::~AxOpenGLMesh(void)
{
	this->DeleteIndexBuffer();
	this->DeleteVertexBuffer();
}

bool AxOpenGLMesh::CreateVertexBuffer(int numVertices)
{
	this->DeleteVertexBuffer();

	this->verticesCount = numVertices;

    this->verticesPositions = new float[this->verticesCount * this->vertexPositionElementsCount];
    this->verticesNormals = new float[this->verticesCount * this->vertexNormalElementsCount];
    this->verticesTexCoords = new float[this->verticesCount * this->vertexTexCoordsElementsCount];
    this->verticesBoneIndices = new float[this->verticesCount * this->vertexBoneIndicesElementsCount];
    this->verticesBoneWeights = new float[this->verticesCount * this->vertexBoneWeightsElementsCount];
    this->verticesTangents = new float[this->verticesCount * this->vertexTangentElementsCount];
    this->verticesBitangents = new float[this->verticesCount * this->vertexBitangentElementsCount];

	glGenBuffers(1, &this->verticesPositionsBuffer);
    glGenBuffers(1, &this->verticesNormalsBuffer);
    glGenBuffers(1, &this->verticesTexCoordsBuffer);
    glGenBuffers(1, &this->verticesBoneIndicesBuffer);
    glGenBuffers(1, &this->verticesBoneWeightsBuffer);
    glGenBuffers(1, &this->verticesTangentBuffer);
    glGenBuffers(1, &this->verticesBitangentBuffer);

//	glCreateBuffers(1, &this->verticesPositionsBuffer);
//	glCreateBuffers(1, &this->verticesNormalsBuffer);
//	glCreateBuffers(1, &this->verticesTexCoordsBuffer);
//	glCreateBuffers(1, &this->verticesBoneIndicesBuffer);
//	glCreateBuffers(1, &this->verticesBoneWeightsBuffer);
//	glCreateBuffers(1, &this->verticesTangentBuffer);
//	glCreateBuffers(1, &this->verticesBitangentBuffer);

	return true;
}

bool AxOpenGLMesh::CreateIndexBuffer(int numIndices)
{
	this->DeleteIndexBuffer();

	this->indicesCount = numIndices;
	
	this->indices = new int[this->indicesCount];

	glGenBuffers(1, &this->indicesBuffer);

	return true;
}

bool AxOpenGLMesh::DeleteVertexBuffer()
{
	if (this->verticesPositions != 0)
	{
		delete[] this->verticesPositions;
		this->verticesPositions = 0;
	}

	if (this->verticesNormals != 0)
	{
		delete[] this->verticesNormals;
		this->verticesNormals = 0;
	}

	if (this->verticesTexCoords != 0)
	{
		delete[] this->verticesTexCoords;
		this->verticesTexCoords = 0;
	}

	if (this->verticesBoneIndices != 0)
	{
		delete[] this->verticesBoneIndices;
		this->verticesBoneIndices = 0;
	}

	if (this->verticesBoneWeights != 0)
	{
		delete[] this->verticesBoneWeights;
		this->verticesBoneWeights = 0;
	}

	if (this->verticesTangents != 0)
	{
		delete[] this->verticesTangents;
		this->verticesTangents = 0;
	}

	if (this->verticesBitangents != 0)
	{
		delete[] this->verticesBitangents;
		this->verticesBitangents = 0;
	}

	
	
	if (this->verticesPositionsBuffer != 0)
	{
		glDeleteBuffers(1, &this->verticesPositionsBuffer);
		this->verticesPositionsBuffer = 0;
	}
	
	if (this->verticesNormalsBuffer != 0)
	{
		glDeleteBuffers(1, &this->verticesNormalsBuffer);
		this->verticesNormalsBuffer = 0;
	}
	
	if (this->verticesTexCoordsBuffer != 0)
	{
		glDeleteBuffers(1, &this->verticesTexCoordsBuffer);
		this->verticesTexCoordsBuffer = 0;
	}
	
	if (this->verticesBoneIndicesBuffer != 0)
	{
		glDeleteBuffers(1, &this->verticesBoneIndicesBuffer);
		this->verticesBoneIndicesBuffer = 0;
	}
	
	if (this->verticesBoneWeightsBuffer != 0)
	{
		glDeleteBuffers(1, &this->verticesBoneWeightsBuffer);
		this->verticesBoneWeightsBuffer = 0;
	}
	
	if (this->verticesTangentBuffer != 0)
	{
		glDeleteBuffers(1, &this->verticesTangentBuffer);
		this->verticesTangentBuffer = 0;
	}
	
	if (this->verticesBitangentBuffer != 0)
	{
		glDeleteBuffers(1, &this->verticesBitangentBuffer);
		this->verticesBitangentBuffer = 0;
	}
	


	this->verticesCount = 0;

	return true;
}

bool AxOpenGLMesh::DeleteIndexBuffer()
{
	if (this->indices != 0)
	{
		delete[] this->indices;
		this->indices = 0;
	}
	
	if (this->indicesBuffer != 0)
	{
		glDeleteBuffers(1, &this->indicesBuffer);
		this->indicesBuffer = 0;
	}

	this->indicesCount = 0;

	return true;
}

bool AxOpenGLMesh::UpdateVertices(int offset, int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesPositionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertexPositionElementsCount * this->verticesCount * 4, this->verticesPositions, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesTexCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertexTexCoordsElementsCount * this->verticesCount * 4, this->verticesTexCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->verticesNormalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertexNormalElementsCount * this->verticesCount * 4, this->verticesNormals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->verticesBoneIndicesBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertexBoneIndicesElementsCount * this->verticesCount * 4, this->verticesBoneIndices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesBoneWeightsBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertexBoneWeightsElementsCount * this->verticesCount * 4, this->verticesBoneWeights, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->verticesTangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertexTangentElementsCount * this->verticesCount * 4, this->verticesTangents, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->verticesBitangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertexBitangentElementsCount * this->verticesCount * 4, this->verticesBitangents, GL_STATIC_DRAW);

	return true;
}

bool AxOpenGLMesh::UpdateIndices(int offset, int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->indicesBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->indicesCount * 4, this->indices, GL_STATIC_DRAW);
    
    return true;
}

int AxOpenGLMesh::GetVertexCount()
{
	return this->verticesCount;
}

int AxOpenGLMesh::GetIndexCount()
{
	return this->indicesCount;
}

int AxOpenGLMesh::GetPrimitiveCount()
{
	return this->indicesCount / 3;
}

void AxOpenGLMesh::GetVertexPosition(int index, AxVector3 &result)
{
	float *pos = (float*)((unsigned int)this->verticesPositions + index * this->vertexPositionElementsCount * 4);
	result.x = *pos++;
	result.y = *pos++;
	result.z = *pos;
}

void AxOpenGLMesh::SetVertexPosition(int index, AxVector3 &value)
{
	float *pos = (float*)((unsigned int)this->verticesPositions + index * this->vertexPositionElementsCount * 4);
	*pos++ = value.x;
	*pos++ = value.y;
	*pos = value.z;
}

void AxOpenGLMesh::GetVertexNormal(int index, AxVector3 &result)
{
	float *normal = (float*)((unsigned int)this->verticesNormals + index * this->vertexNormalElementsCount * 4);
	result.x = *normal++;
	result.y = *normal++;
	result.z = *normal;
}

void AxOpenGLMesh::SetVertexNormal(int index, AxVector3 &value)
{
	float *normal = (float*)((unsigned int)this->verticesNormals + index * this->vertexNormalElementsCount * 4);
	*normal++ = value.x;
	*normal++ = value.y;
	*normal = value.z;
}

void AxOpenGLMesh::GetVertexTexCoords(int index, AxVector2 &result)
{
	float *texCoords = (float*)((unsigned int)this->verticesTexCoords + index * this->vertexTexCoordsElementsCount * 4);
	result.x = *texCoords++;
	result.y = *texCoords++;
}

void AxOpenGLMesh::SetVertexTexCoords(int index, AxVector2 &value)
{
	float *texCoords = (float*)((unsigned int)this->verticesTexCoords + index * this->vertexTexCoordsElementsCount * 4);
	*texCoords++ = value.x;
	*texCoords++ = value.y;
}

void AxOpenGLMesh::GetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	float *bone = (float*)((unsigned int)this->verticesBoneIndices + index * this->vertexBoneIndicesElementsCount * 4);
	boneIndices.x = *bone++;
	boneIndices.y = *bone++;
	boneIndices.z = *bone++;
	boneIndices.w = *bone;
	
	bone = (float*)((unsigned int)this->verticesBoneWeights + index * this->vertexBoneWeightsElementsCount * 4);
	boneWeights.x = *bone++;
	boneWeights.y = *bone++;
	boneWeights.z = *bone++;
	boneWeights.w = *bone;
}

void AxOpenGLMesh::SetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights)
{
	float *bone = (float*)((unsigned int)this->verticesBoneIndices + index * this->vertexBoneIndicesElementsCount * 4);
	*bone++ = boneIndices.x;
	*bone++ = boneIndices.y;
	*bone++ = boneIndices.z;
	*bone = boneIndices.w;

	bone = (float*)((unsigned int)this->verticesBoneWeights + index * this->vertexBoneWeightsElementsCount * 4);
	*bone++ = boneWeights.x;
	*bone++ = boneWeights.y;
	*bone++ = boneWeights.z;
	*bone = boneWeights.w;
}

void AxOpenGLMesh::GetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
	float *tangents = (float*)((unsigned int)this->verticesTangents + index * this->vertexTangentElementsCount * 4);
	tangent.x = *tangents++;
	tangent.y = *tangents++;
	tangent.z = *tangents;
	
	tangents = (float*)((unsigned int)this->verticesBitangents + index * this->vertexBitangentElementsCount * 4);
	biTangent.x = *tangents++;
	biTangent.y = *tangents++;
	biTangent.z = *tangents;
}

void AxOpenGLMesh::SetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent)
{
	float *tangents = (float*)((unsigned int)this->verticesTangents + index * this->vertexTangentElementsCount * 4);
	*tangents++ = tangent.x;
	*tangents++ = tangent.y;
	*tangents = tangent.z;

	tangents = (float*)((unsigned int)this->verticesBitangents + index * this->vertexBitangentElementsCount * 4);
	*tangents++ = biTangent.x;
	*tangents++ = biTangent.y;
	*tangents = biTangent.z;
}

int AxOpenGLMesh::GetIndex(int index)
{
	return this->indices[index];
}

void AxOpenGLMesh::SetIndex(int index, int value)
{
	this->indices[index] = value;
}
