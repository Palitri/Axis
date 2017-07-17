#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

#include "..\..\Primitive\AxBoundingVolume.h"
#include "..\..\Primitive\AxIntersectionInfo.h"

#include "..\..\..\Graphics\AxGraphicsDevice.h"

// TODO: Remove this-> See the "bones" member variable. Maybe use a new "AxSkeleton" resource
#include "..\..\..\Tools\AxList.h"
#include "..\Transform\AxTransform.h"

class Axis;

class AXDLLCLASS AxMesh
	: public AxResource
{
private:
	static const unsigned int SerializationId_Vertices	= 0x21111004;
	static const unsigned int SerializationId_Normals	= 0x21112004;
	static const unsigned int SerializationId_TexCoords	= 0x21113004;
	static const unsigned int SerializationId_Triangles	= 0x21114004;
	static const unsigned int SerializationId_Bones		= 0x21115004;

public:
	static const int classId = (AxResourceType_Mesh << 16) | 0;

	// TODO: Use false for screen quad and other special cases. Remove in future, replace by a 2D management system
	static const int propertyIndex_Cull = AxResource::propertyIndex_ChildPropertiesIndex + 0;

	Axis *context;

	AxDeviceMesh *deviceMesh;

	AxBoundingVolume bounds;

	bool serializeVertices, serializeNormals, serializeTexCoords, serializeBones, serializeTangents;

	bool active;

	AxMesh(Axis *context);
	virtual ~AxMesh(void);

    void ComputeNormals();
    void ComputeTangents();
    void ComputeBounds();
    bool Intersection(AxVector3 &rayPoint, AxVector3 &rayDir, AxIntersectionInfo &intersectionInfo);

	void CopyFrom(AxMesh &source);

	virtual bool Deserialize(AxStream &source);

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};
