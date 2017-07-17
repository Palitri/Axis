#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\AxList.h"

struct AxFileObjVector3
{
	float x, y, z;
};

struct AxFileObjVector4
{
	float x, y, z, w;
};

struct AxFileObjFaceVertex
{
	int vertexIndex;
	int normalIndex;
	int texCoordIndex;
};

struct AxFileObjFace
{
	AxList<AxFileObjFaceVertex> vertices;
	unsigned int smoothingGroups;
};

struct AxFileObjBufferedFace
{
	AxList<int> bufferedVertexIndices;
	unsigned int smoothingGroups;
};


enum AxFileObjTextureMapType
{
	AxFileObjTextureMapType_Default,
	AxFileObjTextureMapType_Sphere,
	AxFileObjTextureMapType_CubeTop,
	AxFileObjTextureMapType_CubeBottom,
	AxFileObjTextureMapType_CubeFront,
	AxFileObjTextureMapType_CubeBack,
	AxFileObjTextureMapType_CubeLeft,
	AxFileObjTextureMapType_CubeRight
};

enum AxFileObjTextureMapChannel
{
	AxFileObjTextureMapChannel_Default,
	AxFileObjTextureMapChannel_R,
	AxFileObjTextureMapChannel_G,
	AxFileObjTextureMapChannel_B,
	AxFileObjTextureMapChannel_Matte,
	AxFileObjTextureMapChannel_Luminance,
	AxFileObjTextureMapChannel_Depth
};