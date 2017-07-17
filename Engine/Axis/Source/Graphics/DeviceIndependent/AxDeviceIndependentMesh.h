#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxDeviceMesh.h"

class AXDLLCLASS AxDeviceIndependentMesh
	: public AxDeviceMesh
{
public:
	float *positions, *normals, *texCoords, *boneIndices, *boneWeights, *tangents, *biTangents;
	unsigned short *indices;
	int numIndices, numVertices;

	AxDeviceIndependentMesh(void);
	virtual ~AxDeviceIndependentMesh(void);

	bool CreateVertexBuffer(int numVertices);
	bool CreateIndexBuffer(int numIndices);
	
	bool DeleteVertexBuffer();
	bool DeleteIndexBuffer();

	bool UpdateVertices(int offset, int count);
	bool UpdateIndices(int offset, int count);

	int GetVertexCount();
	int GetIndexCount();
	int GetPrimitiveCount();

	void GetVertexPosition(int index, AxVector3 &result);
	void SetVertexPosition(int index, AxVector3 &value);

	void GetVertexNormal(int index, AxVector3 &result);
	void SetVertexNormal(int index, AxVector3 &value);

	void GetVertexTexCoords(int index, AxVector2 &result);
	void SetVertexTexCoords(int index, AxVector2 &value);

	void GetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights);
	void SetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights);

	void GetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent);
	void SetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent);

	int GetIndex(int index);
	void SetIndex(int index, int value);
};

