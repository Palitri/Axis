#pragma once

#include "..\..\AxisEngine.h"

#include "AxOpenGLGraphicsDevice.h"

class AxOpenGLMesh
	: public AxDeviceMesh
{
public:
	AxOpenGLGraphicsDevice *context;

    int verticesCount, indicesCount;

	int vertexPositionElementsCount, vertexNormalElementsCount, vertexTexCoordsElementsCount, vertexBoneIndicesElementsCount, vertexBoneWeightsElementsCount, vertexTangentElementsCount, vertexBitangentElementsCount;

	float *verticesPositions, *verticesNormals, *verticesTexCoords, *verticesBoneIndices, *verticesBoneWeights, *verticesTangents, *verticesBitangents;
	int *indices;

	GLuint verticesPositionsBuffer, verticesNormalsBuffer, verticesTexCoordsBuffer, verticesBoneIndicesBuffer, verticesBoneWeightsBuffer, verticesTangentBuffer, verticesBitangentBuffer;
	GLuint indicesBuffer;

	AxOpenGLMesh(void);
	~AxOpenGLMesh(void);

	virtual bool CreateVertexBuffer(int numVertices);
	virtual bool CreateIndexBuffer(int numIndices);

	virtual bool DeleteVertexBuffer();
	virtual bool DeleteIndexBuffer();

	virtual bool UpdateVertices(int offset, int count);
	virtual bool UpdateIndices(int offset, int count);

	virtual int GetVertexCount();
	virtual int GetIndexCount();
	virtual int GetPrimitiveCount();

	virtual void GetVertexPosition(int index, AxVector3 &result);
	virtual void SetVertexPosition(int index, AxVector3 &value);

	virtual void GetVertexNormal(int index, AxVector3 &result);
	virtual void SetVertexNormal(int index, AxVector3 &value);

	virtual void GetVertexTexCoords(int index, AxVector2 &result);
	virtual void SetVertexTexCoords(int index, AxVector2 &value);

	virtual void GetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights);
	virtual void SetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights);

	virtual void GetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent);
	virtual void SetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent);

	virtual int GetIndex(int index);
	virtual void SetIndex(int index, int value);
};

