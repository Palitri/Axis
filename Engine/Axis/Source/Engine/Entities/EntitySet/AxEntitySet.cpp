//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxEntitySet.h"

AxEntitySet::AxEntitySet(void)
	: AxResource()
{
	this->typeId = AxEntitySet::classId;
    this->resourceType = AxResourceType_EntitySet;

    this->properties.Add(new AxProperty(AxString("Multi Pass"), false));
    this->properties.Add(new AxProperty(AxString("Branch"), true));
}


AxEntitySet::~AxEntitySet(void)
{
}

bool AxEntitySet::HierarchyContains(AxResource* resource)
{
	for (int i = 0; i < this->references.count; i++)
	{
		AxResource *reference = this->references[i];

		if (reference == resource)
			return true;

		if (reference->typeId == AxResourceType_EntitySet)
			if (((AxEntitySet*)reference)->HierarchyContains(resource))
				return true;
	}

	return false;
}

void AxEntitySet::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxResource::SerializeChunks(writer);

	writer.BeginChunk(AxEntitySet::SerializationId_References);
	writer.stream->WriteInt32(this->references.count);
	for (int i = 0; i < this->references.count; i++)
		writer.stream->WriteInt64(this->references[i]->id);
	writer.EndChunk();
}

bool AxEntitySet::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxEntitySet::SerializationId_References:
		{
			int numReferences = reader.stream->ReadInt32();
			for (int i = 0; i < numReferences; i++)
			{
				long long id = reader.stream->ReadInt64();

				AxResource *link = new AxResource();
				link->resourceType = AxResourceType_None;
				link->id = id;
				this->references.Add(link);
			}
			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
