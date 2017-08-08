#pragma once

#include "..\..\AxisEngine.h"

#include "AxDirect3D11GraphicsDevice.h"

class AxDirect3D11Mesh :
	public AxDeviceMesh
{
private:
	void *vertexData;
	int *indexData;

public:
	ID3D11InputLayout*      vertexLayout;
	ID3D11Buffer*           vertexBuffer;
	
	ID3D11Buffer*           indexBuffer;

	int positionOffset, normalOffset, texCoordsOffset, bonesOffset, tangentsOffset;
	int vertexSize;

	int vertexCount;
	int indexCount;
	int indexSize;

	AxDirect3D11GraphicsDevice *context;

	AxDirect3D11Mesh(void);
	~AxDirect3D11Mesh(void);

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

