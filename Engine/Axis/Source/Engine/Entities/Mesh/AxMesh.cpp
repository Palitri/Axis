//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMesh.h"

#include "..\..\Utilities\AxMaths.h"
#include "..\..\Utilities\Normals\AxNormalsGenerator.h"
#include "..\..\Utilities\Tangents\AxTangentsGenerator.h"

#include "..\..\Utilities\Serialization\AxSerializationUtils.h"

#include "..\..\..\Tools\AxMath.h"

#include "..\..\Axis.h"


AxMesh::AxMesh(Axis *context)
	: AxResource()
{
	this->typeId = AxMesh::classId;
	this->resourceType = AxResourceType_Mesh;
    this->active = true;
        
    this->properties.Add(new AxProperty(AxString("Cull"), true));

	this->context = context;

	this->deviceMesh = this->context->graphicsDevice->CreateMesh();

	this->serializeVertices = true;
	this->serializeTexCoords = true;
	this->serializeBones = true;
	this->serializeNormals = false;
	this->serializeTangents = false;
}

AxMesh::~AxMesh(void)
{
	delete this->deviceMesh;
}

void AxMesh::ComputeNormals()
{
	AxNormalsGenerator(*this->deviceMesh);	

	this->deviceMesh->UpdateVertices(0, -1);
}

void AxMesh::ComputeTangents()
{
	AxTangentsGenerator(*this->deviceMesh);	

	this->deviceMesh->UpdateVertices(0, -1);
}

void AxMesh::ComputeBounds()
{
	AxVector3 v, min, max;

	min.Set(AxMath::FloatMax);
	max.Set(AxMath::FloatMin);
	int numVertices = this->deviceMesh->GetVertexCount();
	for (int i = 0; i < numVertices; i++)
    {
        this->deviceMesh->GetVertexPosition(i, v);
        AxVector3::Min(min, min, v);
        AxVector3::Max(max, max, v);
    }

    AxVector3::Add(this->bounds.center, min, max);
    AxVector3::Scale(this->bounds.center, this->bounds.center, 1.0f / 2.0f);

    AxVector3::Subtract(this->bounds.box, max, min);
    AxVector3::Scale(this->bounds.box, this->bounds.box, 1.0f / 2.0f);

    this->bounds.sphereRadius = 0.0f;
    for (int i = 0; i < numVertices; i++)
    {
        this->deviceMesh->GetVertexPosition(i, v);
		this->bounds.sphereRadius = AxMath::Max(this->bounds.sphereRadius, AxVector3::Distance(this->bounds.center, v));
    }
}

    
bool AxMesh::Intersection(AxVector3 &rayPoint, AxVector3 &rayDir, AxIntersectionInfo &intersectionInfo)
{
	intersectionInfo.hasIntersected = false;

    if (!AxMaths::RayToSphereIntersection(rayPoint, rayDir, this->bounds.center, this->bounds.sphereRadius))
        return false;

	AxVector3 v0, v1, v2;

	intersectionInfo.distance = AxMath::FloatMax;
	int numIndices = this->deviceMesh->GetIndexCount();
    for (int i = 0; i < numIndices; i += 3)
    {
        int index0 = this->deviceMesh->GetIndex(i + 2);
        int index1 = this->deviceMesh->GetIndex(i + 1);
        int index2 = this->deviceMesh->GetIndex(i);

		this->deviceMesh->GetVertexPosition(index0, v0);
        this->deviceMesh->GetVertexPosition(index1, v1);
        this->deviceMesh->GetVertexPosition(index2, v2);
            
		float d, u, v;
        if (AxMaths::RayToTriangleIntersection(rayPoint, rayDir,
											v0, v1, v2,
											d, u, v))
        {
            if ((d > 0) && (d < intersectionInfo.distance))
            {
				AxVertex delta1, delta2;
				AxVector3 n0, n1, n2;
				AxVector2 texCoord0, texCoord1, texCoord2;

                this->deviceMesh->GetVertexNormal(index0, n0);
                this->deviceMesh->GetVertexNormal(index1, n1);
                this->deviceMesh->GetVertexNormal(index2, n2);

                this->deviceMesh->GetVertexTexCoords(index0, texCoord0);
                this->deviceMesh->GetVertexTexCoords(index1, texCoord1);
                this->deviceMesh->GetVertexTexCoords(index2, texCoord2);

				AxVector3::Subtract(delta1.position, v1, v0);
				AxVector3::Subtract(delta1.normal, n1, n0);
				AxVector2::Subtract(delta1.texCoords, texCoord1, texCoord0);

				AxVector3::Subtract(delta2.position, v2, v0);
				AxVector3::Subtract(delta2.normal, n2, n0);
				AxVector2::Subtract(delta2.texCoords, texCoord2, texCoord0);


                intersectionInfo.point.position.x = v0.x + delta1.position.x * u + delta2.position.x * v;
                intersectionInfo.point.position.y = v0.y + delta1.position.y * u + delta2.position.y * v;
                intersectionInfo.point.position.z = v0.z + delta1.position.z * u + delta2.position.z * v;
                intersectionInfo.point.normal.x = n0.x + delta1.normal.x * u + delta2.normal.x * v;
                intersectionInfo.point.normal.y = n0.y + delta1.normal.y * u + delta2.normal.y * v;
                intersectionInfo.point.normal.z = n0.z + delta1.normal.z * u + delta2.normal.x * v;
                intersectionInfo.point.texCoords.x = texCoord0.x + delta1.texCoords.x * u + delta2.texCoords.x * v;
                intersectionInfo.point.texCoords.y = texCoord0.y + delta1.texCoords.y * u + delta2.texCoords.y * v;

                intersectionInfo.distance = d;
                intersectionInfo.primitiveIndex = i;
            }

            intersectionInfo.hasIntersected = true;
        }
    }

    return intersectionInfo.hasIntersected;
}

void AxMesh::CopyFrom(AxMesh &source)
{
	this->deviceMesh->CopyFrom(source.deviceMesh);
}

bool AxMesh::Deserialize(AxStream &source)
{
	this->serializeVertices = false;
	this->serializeNormals = false;
	this->serializeTexCoords = false;
	this->serializeTangents = false;
	this->serializeBones = false;

	if (!AxResource::Deserialize(source))
		return false;

	if (!this->serializeNormals)
		this->ComputeNormals();

	if (!this->serializeTangents)
		this->ComputeTangents();

	this->deviceMesh->UpdateIndices(0, -1);
	this->deviceMesh->UpdateVertices(0, -1);
	this->ComputeBounds();

	return true;
}



void AxMesh::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxResource::SerializeChunks(writer);

	bool embedMeshData = this->context->settings->properties[AxSettings::propertyIndex_EmbedGeneratedMeshes]->GetBool();

	int numVertices = this->deviceMesh->GetVertexCount();
	int numIndices = this->deviceMesh->GetIndexCount();
	AxVector3 v3;
	AxVector2 v2;
	AxVector4 v41, v42;

	if (this->serializeVertices || embedMeshData)
	{
		writer.BeginChunk(AxMesh::SerializationId_Vertices);
		writer.stream->WriteInt32(numVertices);
		for (int i = 0; i < numVertices; i++)
		{
			this->deviceMesh->GetVertexPosition(i, v3);
			AxSerializationUtils::SerializeVector3(*writer.stream, v3);
		}
		writer.EndChunk();
	}

	if (this->serializeNormals)
	{
		writer.BeginChunk(AxMesh::SerializationId_Normals);
		writer.stream->WriteInt32(numVertices);
		for (int i = 0; i < numVertices; i++)
		{
			this->deviceMesh->GetVertexNormal(i, v3);
			AxSerializationUtils::SerializeVector3(*writer.stream, v3);
		}
		writer.EndChunk();
	}

	if (this->serializeTexCoords || embedMeshData)
	{
		writer.BeginChunk(AxMesh::SerializationId_TexCoords);
		writer.stream->WriteInt32(numVertices);
		for (int i = 0; i < numVertices; i++)
		{
			this->deviceMesh->GetVertexTexCoords(i, v2);
			AxSerializationUtils::SerializeVector2(*writer.stream, v2);
		}
		writer.EndChunk();
	}

	if (this->serializeVertices || embedMeshData)
	{
		writer.BeginChunk(AxMesh::SerializationId_Triangles);
		writer.stream->WriteInt32(numIndices);
		for (int i = 0; i < numIndices; i++)
		{
			writer.stream->WriteUInt32(this->deviceMesh->GetIndex(i));
		}
		writer.EndChunk();
	}

	if (this->serializeBones)
	{
		writer.BeginChunk(AxMesh::SerializationId_Bones);
		writer.stream->WriteInt32(numVertices);
		for (int i = 0; i < numVertices; i++)
		{
			this->deviceMesh->GetVertexBones(i, v41, v42);
			AxSerializationUtils::SerializeVector4(*writer.stream, v41);
			AxSerializationUtils::SerializeVector4(*writer.stream, v42);
		}
		writer.EndChunk();
	}
}

bool AxMesh::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxMesh::SerializationId_Vertices:
		{
			int numVertices = reader.stream->ReadInt32();
			if (numVertices != this->deviceMesh->GetVertexCount())
				this->deviceMesh->CreateVertexBuffer(numVertices);

			for (int i = 0; i < numVertices; i++)
				this->deviceMesh->SetVertexPosition(i, AxSerializationUtils::DeserializeVector3(*reader.stream));

			this->serializeVertices = true;

			break;
		}

		case AxMesh::SerializationId_Normals:
		{
			int numVertices = reader.stream->ReadInt32();
			if (numVertices != this->deviceMesh->GetVertexCount())
				this->deviceMesh->CreateVertexBuffer(numVertices);

			for (int i = 0; i < numVertices; i++)
				this->deviceMesh->SetVertexNormal(i, AxSerializationUtils::DeserializeVector3(*reader.stream));

			this->serializeNormals = true;

			break;
		}

		case AxMesh::SerializationId_TexCoords:
		{
			int numVertices = reader.stream->ReadInt32();
			if (numVertices != this->deviceMesh->GetVertexCount())
				this->deviceMesh->CreateVertexBuffer(numVertices);

			for (int i = 0; i < numVertices; i++)
				this->deviceMesh->SetVertexTexCoords(i, AxSerializationUtils::DeserializeVector2(*reader.stream));

			this->serializeTexCoords = true;

			break;
		}

		case AxMesh::SerializationId_Triangles:
		{
			int numIndices = reader.stream->ReadInt32();
			this->deviceMesh->CreateIndexBuffer(numIndices);

			for (int i = 0; i < numIndices; i++)
				this->deviceMesh->SetIndex(i, reader.stream->ReadUInt32());

			break;
		}

		case AxMesh::SerializationId_Bones:
		{
			int numVertices = reader.stream->ReadInt32();
			if (numVertices != this->deviceMesh->GetVertexCount())
				this->deviceMesh->CreateVertexBuffer(numVertices);

			AxVector4 boneIndices, boneWeights;
			for (int i = 0; i < numVertices; i++)
			{
				boneIndices = AxSerializationUtils::DeserializeVector4(*reader.stream);
				boneWeights = AxSerializationUtils::DeserializeVector4(*reader.stream);
				this->deviceMesh->SetVertexBones(i, boneIndices, boneWeights);
			}

			this->serializeBones = true;

			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
