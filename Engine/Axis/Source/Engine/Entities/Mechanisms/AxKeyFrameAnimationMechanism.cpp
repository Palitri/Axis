//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxKeyframeAnimationMechanism.h"

#include "..\..\Axis.h"

#include "KeyFrameAnimation\AxKeyFrameRotationAxis.h"

AxKeyframeAnimationMechanism::AxKeyframeAnimationMechanism(Axis *context)
	: AxMechanism()
{
	this->typeId = AxKeyframeAnimationMechanism::classId;

	this->context = context;

	this->parameter.SetType(AxParameterType_None);

	this->properties.Add(new AxProperty("Frame", 0.0f));
    this->properties.Add(new AxProperty("Transform", 0, AxParameterType_ReferenceTransform));
}


AxKeyframeAnimationMechanism::~AxKeyframeAnimationMechanism(void)
{
	for (int i = 0; i < this->keyFramesTracks.count; i++)
		delete this->keyFramesTracks[i];
}

bool AxKeyframeAnimationMechanism::Process(float deltaTime)
{
	float time = *(float*)this->properties[AxKeyframeAnimationMechanism::propertyIndex_Frame]->GetEffectiveParameter()->value;
	AxTransform *transform = (AxTransform*)this->properties[AxKeyframeAnimationMechanism::propertyIndex_Transform]->GetEffectiveParameter()->value;

	if (this->keyFramesTracks.count == 0)
		return true;

	this->keyFramesTracks[0]->GetTransform(transform->transform, time);

	AxMatrix m;
	for (int i = 1; i < this->keyFramesTracks.count; i++)
	{
		this->keyFramesTracks[i]->GetTransform(m, time);
		AxMatrix::Multiply(transform->transform, transform->transform, m);
	}

    return true;
}

void AxKeyframeAnimationMechanism::BuildRotationAxisTrack(AxKeyFramesTrack *rotationAxisTrack)
{
	AxMatrix transform, rotationMatrix;

	AxMatrix::CreateIdentity(transform);
	int count = rotationAxisTrack->count;
    for (int i = 0; i < count; i++)
    {
		AxMatrix::CreateRotationAxis(rotationMatrix, ((AxKeyFrameRotationAxis*)rotationAxisTrack->Get(i))->axis, ((AxKeyFrameRotationAxis*)rotationAxisTrack->Get(i))->angle);
        AxMatrix::Multiply(transform, transform, rotationMatrix);
        AxMatrix::Copy(((AxKeyFrameRotationAxis*)rotationAxisTrack->Get(i))->previousTransform, transform);
    }
}

void AxKeyframeAnimationMechanism::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxMechanism::SerializeChunks(writer);

	writer.BeginChunk(AxKeyframeAnimationMechanism::SerializationId_KeyFrameTracks);

	for (int trackIndex = 0; trackIndex < this->keyFramesTracks.count; trackIndex++)
	{
		writer.BeginChunk(AxKeyframeAnimationMechanism::SerializationId_KeyFrameTrack);

		int keyFramesCount = this->keyFramesTracks[trackIndex]->count;
		if (keyFramesCount > 0)
		{
			int typeId = this->keyFramesTracks[trackIndex]->Get(0)->typeId;
			writer.BeginChunk(AxKeyframeAnimationMechanism::SerializationId_KeyFrameTrackFrames);
			writer.stream->WriteInt32(typeId);

			for (int keyFrameIndex = 0; keyFrameIndex < keyFramesCount; keyFrameIndex++)
			{
				if (this->keyFramesTracks[trackIndex]->Get(keyFrameIndex)->typeId != typeId)
				{
					writer.EndChunk();
					writer.BeginChunk(AxKeyframeAnimationMechanism::SerializationId_KeyFrameTrackFrames);
					writer.stream->WriteInt32(typeId);

					typeId = this->keyFramesTracks[trackIndex]->Get(keyFrameIndex)->typeId;
				}

				this->keyFramesTracks[trackIndex]->Get(keyFrameIndex)->Serialize(*writer.stream);
			}

			writer.EndChunk();
			// SerializationId_KeyFrameTrackFrames
		}

		writer.EndChunk();
		// SerializationId_KeyFrameTrack
	}

	writer.EndChunk();
	// SerializationId_KeyFrameTracks
}

bool AxKeyframeAnimationMechanism::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxMechanism::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxKeyframeAnimationMechanism::SerializationId_KeyFrameTracks:
		{
			break;
		}

		case AxKeyframeAnimationMechanism::SerializationId_KeyFrameTrack:
		{
			this->currentDeserializationTrack = this->keyFramesTracks.Add(new AxKeyFramesTrack());
			break;
		}

		case AxKeyframeAnimationMechanism::SerializationId_KeyFrameTrackFrames:
		{
			long long chunkEnd = reader.stream->position + reader.chunkSize;

			int typeId = reader.stream->ReadInt32();
		
			while (reader.stream->position < chunkEnd)
			{
				AxKeyFrame *keyFrame = (AxKeyFrame*)this->context->CreateEntity(typeId);
				if (keyFrame == 0)
				{
					reader.SkipChunk();
					break;
				}

				keyFrame->Deserialize(*reader.stream);
				this->currentDeserializationTrack->Add(keyFrame);
			}

			if (typeId == AxKeyFrameRotationAxis::classId)
				this->BuildRotationAxisTrack(this->currentDeserializationTrack);

			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
