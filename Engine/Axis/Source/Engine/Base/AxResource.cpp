//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxResource.h"

#include "..\..\Tools\AxMath.h"

AxResource::AxResource(void)
	: AxEntity()
{
	this->resourceType = AxResourceType_None;
    this->id = -1;

	this->tag = 0;

	this->properties.Add(new AxProperty(AxString("Active"), true));
}


AxResource::~AxResource(void)
{
    int count = this->properties.count;
    for (int i = 0; i < count; i++)
	{
		delete this->properties[i];
	}

    count = this->oldProperties.count;
	for (int i = 0; i < count; i++)
	{
		delete this->oldProperties[i];
	}
}

AxProperty *AxResource::GetProperty(const AxString &name)
{
    int count = this->properties.count;
    for (int i = 0; i < count; i++)
        if (this->properties[i]->name.Equals(name))
            return this->properties[i];

    return 0;
}

void AxResource::SerializeParameter(AxParameter &parameter, AxStream &destination)
{
	destination.WriteInt32(parameter.type);

	if (parameter.IsReference())
	{
		AxResource *reference = (AxResource*)parameter.value;
		if (reference != 0)
			destination.WriteInt64(reference->id);
		else
			destination.WriteInt64(-1);
	}
	else
	{
		switch (parameter.type)
		{
			case AxParameterType_Int:
			{
				destination.WriteInt32(*(int*)parameter.value);
				break;
			}

			case AxParameterType_Float:
			{
				destination.WriteFloat32(*(float*)parameter.value);
				break;
			}

			case AxParameterType_Bool:
			{
				destination.WriteBool(*(bool*)parameter.value);
				break;
			}

			case AxParameterType_Color:
			{
				destination.WriteFloat32(((AxVector4*)parameter.value)->x);
				destination.WriteFloat32(((AxVector4*)parameter.value)->y);
				destination.WriteFloat32(((AxVector4*)parameter.value)->z);
				destination.WriteFloat32(((AxVector4*)parameter.value)->w);
				break;
			}
		}
	}
}

void AxResource::DeserializeParameter(AxParameter &parameter, AxStream &source)
{
	AxParameterType propertyType = (AxParameterType)source.ReadInt32();

	if (AxParameter::IsReference(propertyType))
	{
		long long linkId = source.ReadInt64();

		AxResource *link = new AxResource();
		link->resourceType = AxResourceType_None;
		link->id = linkId;
		parameter.SetValue(link, propertyType);
	}
	else
	{
		switch (propertyType)
		{
			case AxParameterType_Int:
			{
				parameter.SetInt(source.ReadInt32());
				break;
			}

			case AxParameterType_Float:
			{
				parameter.SetFloat(source.ReadFloat32());
				break;
			}

			case AxParameterType_Bool:
			{
				parameter.SetBool(source.ReadBool());
				break;
			}

			case AxParameterType_Color:
			{
				AxVector4 color;
				color.x = source.ReadFloat32();
				color.y = source.ReadFloat32();
				color.z = source.ReadFloat32();
				color.w = source.ReadFloat32();
				parameter.SetColor(color);
				break;
			}
		}
	}
}

void AxResource::Serialize(AxStream &destination)
{
	AxHierarchyStreamWriter writer(destination);

	writer.BeginChunk(AxResource::SerializationId_Resource);
	this->SerializeChunks(writer);
	writer.EndChunk();
}

bool AxResource::Deserialize(AxStream &source)
{
	AxHierarchyStreamReader reader(source);

	long long mainChunkEnd = -1;

	while (reader.ReadChunkHeader())
	{
		switch (reader.chunkId)
		{
			case AxResource::SerializationId_Resource:
			{
				mainChunkEnd = reader.stream->position + reader.chunkSize;
				break;
			}

			default:
			{
				if ((reader.chunkSize < 0) || (reader.stream->position + reader.chunkSize > mainChunkEnd))
					return false;

				if (!this->DeserializeChunk(reader))
					reader.SkipChunk();

				break;
			}
		}

		if (reader.stream->position >= mainChunkEnd)
			return true;
	}

	return true;
}

void AxResource::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	writer.BeginChunk(AxResource::SerializationId_ResourceId);
	writer.stream->WriteInt64(this->id);
	writer.EndChunk();

	writer.BeginChunk(AxResource::SerializationId_ResourceName);
	writer.stream->WriteString(this->name);
	writer.EndChunk();

	writer.BeginChunk(AxResource::SerializationId_Properties);
	
	for (int i = 0; i < this->properties.count; i++)
	{
		writer.BeginChunk(AxResource::SerializationId_Property);

		writer.stream->WriteString(this->properties[i]->name);

		this->SerializeParameter(*(AxParameter*)this->properties[i], *writer.stream);
	
		writer.EndChunk();
		// Property
	}

	writer.EndChunk();
	// Properties
}

bool AxResource::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	switch (reader.chunkId)
	{
		case AxResource::SerializationId_ResourceId:
		{
			this->id = reader.stream->ReadInt64();
			break;
		}

		case AxResource::SerializationId_ResourceName:
		{
			this->name = reader.stream->ReadString();
			break;
		}

		case AxResource::SerializationId_Properties:
		{
			break;
		}

		case AxResource::SerializationId_Property:
		{
			AxString propertyName = reader.stream->ReadString();
			AxProperty *currentProperty = this->GetProperty(propertyName);
			if (currentProperty == 0)
				currentProperty = this->properties.Add(new AxProperty(propertyName, 0, AxParameterType_None));

			this->DeserializeParameter(*(AxParameter*)currentProperty, *reader.stream);

			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}

bool AxResource::NotifyPropertiesChange(bool checkNewProperties)
{
	bool propertiesChanged = false;
	int count = AxMath::Min(this->oldProperties.count, this->properties.count);
	for (int i = 0; i < count; i++)
	{
		AxParameter *param = this->properties[i]->GetEffectiveParameter();
		AxParameter *oldValue = this->oldProperties[i];
		if (!param->Equals(*oldValue))
		{
			propertiesChanged = true;
			this->OnPropertyChanged(i);
			oldValue->SetValue(param->value, param->type);
		}
	}

	if (checkNewProperties)
	{
		for (int i = this->oldProperties.count; i < this->properties.count; i++)
		{
			AxParameter *param = this->properties[i]->GetEffectiveParameter();
			this->oldProperties.Add(new AxParameter(param->value, param->type));
			propertiesChanged = true;
			this->OnPropertyChanged(i);
		}
	}

	if (propertiesChanged)
		this->OnPropertiesChanged();

	return propertiesChanged;
}

void AxResource::OnPropertiesChanged()
{
}

void AxResource::OnPropertyChanged(int propertyIndex)
{
}
