//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMechanism.h"

#include "..\..\Base\AxResourceType.h"

AxMechanism::AxMechanism(void)
	: AxResource()
{
	this->typeId = AxMechanism::classId;

	this->resourceType = AxResourceType_Mechanism;
	//new this->parameter = new AxMechanismParameter(AxParameterType_None, 0);
}


AxMechanism::~AxMechanism(void)
{
}

bool AxMechanism::Process(float deltaTime)
{
	return false;
}

void AxMechanism::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxResource::SerializeChunks(writer);

	writer.BeginChunk(AxMechanism::SerializationId_Parameter);
	this->SerializeParameter(this->parameter, *writer.stream);
	writer.EndChunk();
}

bool AxMechanism::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxMechanism::SerializationId_Parameter:
		{
			this->DeserializeParameter(this->parameter, *reader.stream);
			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}

