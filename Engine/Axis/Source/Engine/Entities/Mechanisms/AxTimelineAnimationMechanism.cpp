//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTimelineAnimationMechanism.h"

#include "..\..\..\Tools\AxMath.h"

AxTimelineAnimationMechanism::AxTimelineAnimationMechanism(void)
{
	this->typeId = AxTimelineAnimationMechanism::classId;

	this->parameter.SetType(AxParameterType_Float);

	this->properties.Add(new AxProperty("Frame", 0.0f));

	this->lastKeyIndex = 0;
}


AxTimelineAnimationMechanism::~AxTimelineAnimationMechanism(void)
{
}

bool AxTimelineAnimationMechanism::Process(float deltaTime)
{
	float time = *(float*)this->properties[AxTimelineAnimationMechanism::propertyIndex_Frame]->GetEffectiveValue();

	int count = this->keys.count;
	
	// No keyframes
	if (count == 0)
	{
		*(float*)this->parameter.value = 0.0f;
		return true;
	}

	// Only one keyframe
	else if (count == 1)
	{
		*(float*)this->parameter.value = this->keys[0].value;
		return true;
	}

	// Time outside of left boundary
	else if (time <= this->keys[0].time)
	{
		*(float*)this->parameter.value = this->keys[0].value;
		return true;
	}

	// Time outside of right boundary
	else if (time >= this->keys[count - 1].time)
	{
		*(float*)this->parameter.value = this->keys[count - 1].value;
		return true;
	}

	// Search - a minimum of 2 key frames are required
	else
	{
		int left = 0;
		int right = count - 2;

		int index;
		float startTime, endTime;

		do
		{
			index = left + (right - left) / 2;
			startTime = this->keys[index].time;
			endTime = this->keys[index + 1].time;

			if (time > endTime)
				left = index + 1;
			else if (time < startTime)
				right = index - 1;
			else
				break;
		}
		while (left <= right);

		float blend = (time - startTime) / (float)(endTime - startTime);

		*(float*)this->parameter.value = this->keys[index].value + (this->keys[index + 1].value - this->keys[index].value) * blend;

		return true;
	}

	return false;
}


void AxTimelineAnimationMechanism::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxMechanism::SerializeChunks(writer);

	if (this->keys.count > 0)
	{
		writer.BeginChunk(AxTimelineAnimationMechanism::SerializationId_TimelineTrack);
		writer.stream->WriteInt32(this->keys.count);

		for (int keyIndex = 0; keyIndex < this->keys.count; keyIndex++)
		{
			writer.stream->WriteFloat32(this->keys[keyIndex].time);
			writer.stream->WriteFloat32(this->keys[keyIndex].value);
		}

		writer.EndChunk();
	}
}

bool AxTimelineAnimationMechanism::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxMechanism::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxTimelineAnimationMechanism::SerializationId_TimelineTrack:
		{
			int numKeys = reader.stream->ReadInt32();
			this->keys.SetSize(numKeys);
			for (int keyIndex = 0; keyIndex < this->keys.count; keyIndex++)
			{
				this->keys[keyIndex].time = reader.stream->ReadFloat32();
				this->keys[keyIndex].value = reader.stream->ReadFloat32();
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
