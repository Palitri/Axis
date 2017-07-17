#pragma once

#include "..\AxGlobals.h"

#include "..\Engine\Primitive\AxVector2.h"
#include "..\Engine\Primitive\AxVector3.h"
#include "..\Engine\Primitive\AxVector4.h"

class AXDLLCLASS AxDeviceMesh
{
public:
	AxDeviceMesh(void);
	virtual ~AxDeviceMesh(void);

	virtual void CopyFrom(AxDeviceMesh *source);

	virtual bool CreateVertexBuffer(int numVertices) = 0;
	virtual bool CreateIndexBuffer(int numIndices) = 0;

	virtual bool DeleteVertexBuffer() = 0;
	virtual bool DeleteIndexBuffer() = 0;

	virtual bool UpdateVertices(int offset, int count) = 0;
	virtual bool UpdateIndices(int offset, int count) = 0;

	virtual int GetVertexCount() = 0;
	virtual int GetIndexCount() = 0;
	virtual int GetPrimitiveCount() = 0;

	virtual void GetVertexPosition(int index, AxVector3 &result) = 0;
	virtual void SetVertexPosition(int index, AxVector3 &value) = 0;

	virtual void GetVertexNormal(int index, AxVector3 &result) = 0;
	virtual void SetVertexNormal(int index, AxVector3 &value) = 0;

	virtual void GetVertexTexCoords(int index, AxVector2 &result) = 0;
	virtual void SetVertexTexCoords(int index, AxVector2 &value) = 0;

	virtual void GetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights) = 0;
	virtual void SetVertexBones(int index, AxVector4 &boneIndices, AxVector4 &boneWeights) = 0;

	virtual void GetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent) = 0;
	virtual void SetVertexTangents(int index, AxVector3 &tangent, AxVector3 &biTangent) = 0;

	virtual int GetIndex(int index) = 0;
	virtual void SetIndex(int index, int value) = 0;
};

