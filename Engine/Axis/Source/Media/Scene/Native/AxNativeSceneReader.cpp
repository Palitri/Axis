//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNativeSceneReader.h"

#include "AxNativeSceneID.h"

#include "..\..\..\Engine\Entities\EntitySet\AxEntitySet.h"

#include "..\..\..\Tools\Streams\AxFileStream.h"

#include "..\..\..\Engine\Axis.h"


AxNativeSceneReader::AxNativeSceneReader(Axis *context)
{
    this->context = context;
}

AxNativeSceneReader::~AxNativeSceneReader(void)
{
}

bool AxNativeSceneReader::Read(AxStream &source)
{
	AxHierarchyStreamReader reader(source);

	long long mainChunkEnd = -1;

    while (reader.ReadChunkHeader())
	{
		switch (reader.chunkId)
		{
			case AxNativeSceneID::Main:
				{
					mainChunkEnd = reader.stream->position + reader.chunkSize;
					break;
				}

			case AxNativeSceneID::Resources:
				{
					break;
				}

			case AxNativeSceneID::ResourceSerializationWrap:
				{
					AxResource *resource = this->context->DeserializeResource(reader.stream);
					if (resource != 0)
					{
						if (resource->typeId == AxSettings::classId)
						{
							delete this->context->settings;
							this->context->settings = (AxSettings*)resource;
						}
						else
						{
							long long originalId = resource->id;
							this->context->AddResource(resource);
							this->idMap.Add(originalId, resource->id);
						}
					}

					break;
				}

			case AxNativeSceneID::RootBranch:
				{
					long long rootId = reader.stream->ReadInt64();
					AxResource *rootResource = this->context->FindResourceX(this->GetMappedResourceId(rootId));

					if (rootResource != 0)
					{
						if (rootResource->resourceType == AxResourceType_EntitySet)
						{
							AxEntitySet *root = (AxEntitySet*)rootResource;

							if (this->context->root != 0)
							{
								for (int i = 0; i < root->references.count; i++)
									this->context->root->references.Add(root->references[i]);

								this->context->RemoveResource(root);
							}
							else
							{
								this->context->root = root;
							}
						}
					}

					break;
				}

			case AxNativeSceneID::RuntimeParameters:
				{
					break;
				}

			default:
			{
				if ((reader.chunkSize < 0) || (reader.stream->position + reader.chunkSize > mainChunkEnd))
					return false;

				reader.SkipChunk();

				break;
			}
		}

		if (reader.stream->position >= mainChunkEnd)
			break;
	}

	this->LinkResources();

	return true;
}

void AxNativeSceneReader::LinkResources()
{
	int resourceCount = this->context->resources.count;
    for (int resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++)
    {
		AxResource *resource = this->context->resources[resourceIndex];

        int numProps = ((AxResource*)resource)->properties.count;
        for (int prop = 0; prop < numProps; prop++)
        {
            AxProperty *parameter = ((AxResource*)resource)->properties[prop];
			if (parameter->IsReference() && (parameter->value != 0))
			{
				AxResource *reference = (AxResource*)parameter->value;
				if (reference->resourceType == AxResourceType_None)
				{
					AxResource *ref = this->context->FindResourceX(this->GetMappedResourceId(reference->id));
					parameter->value = ref;
					delete reference;
				}
			}
        }

		if (resource->resourceType == AxResourceType_EntitySet)
        {
            int numReferences = ((AxEntitySet*)resource)->references.count;
            for (int i = 0; i < numReferences; i++)
            {
				AxResource *reference = (AxResource*)((AxEntitySet*)resource)->references[i];
				if (reference->resourceType == AxResourceType_None)
				{
					AxResource *ref = this->context->FindResourceX(this->GetMappedResourceId(reference->id));
					((AxEntitySet*)resource)->references[i] = ref;
					delete reference;
				}
            }
        }
    }
}

long long AxNativeSceneReader::GetMappedResourceId(long long originalId)
{
	long long newId;
	if (this->idMap.GetValue(originalId, newId))
		return newId;
	else
		return -1;
}
