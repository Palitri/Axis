//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxLinearMechanism.h"

#include "..\..\..\Tools\AxMath.h"


AxLinearMechanism::AxLinearMechanism(float min, float max, float speed, bool timed, bool loop, bool oscillate)
	: AxMechanism()
{
	this->typeId = AxLinearMechanism::classId;

	this->parameter.SetFloat(min);

	this->properties.Add(new AxProperty("Min", min));
    this->properties.Add(new AxProperty("Max", max));
    this->properties.Add(new AxProperty("Speed", speed));
    this->properties.Add(new AxProperty("Timed", timed));
    this->properties.Add(new AxProperty("Loop", loop));
    this->properties.Add(new AxProperty("Oscilate", oscillate));

	this->phase = min;
}


AxLinearMechanism::~AxLinearMechanism(void)
{
}

bool AxLinearMechanism::Process(float deltaTime)
{
	float min = *(float*)this->properties[AxLinearMechanism::propertyIndex_Min]->GetEffectiveParameter()->value;
	float max = *(float*)this->properties[AxLinearMechanism::propertyIndex_Max]->GetEffectiveParameter()->value;
	float speed = *(float*)this->properties[AxLinearMechanism::propertyIndex_Speed]->GetEffectiveParameter()->value;
	bool timed = *(bool*)this->properties[AxLinearMechanism::propertyIndex_Timed]->GetEffectiveParameter()->value;
    bool loop = *(bool*)this->properties[AxLinearMechanism::propertyIndex_Loop]->GetEffectiveParameter()->value;
	bool oscillate = *(bool*)this->properties[AxLinearMechanism::propertyIndex_Oscillate]->GetEffectiveParameter()->value;

	float interval = max - min;
	float deltaPhase = timed ? (deltaTime / speed) : (deltaTime * speed / interval);
	float phaseMax = oscillate ? 2.0f : 1.0f;

	this->phase += deltaPhase;

	bool result = true;
	if (this->phase > phaseMax)
	{
		if (loop)
			this->phase -= (int)this->phase;
		else
		{
			this->phase = phaseMax;
			result = false;
		}
	}

	else if (this->phase < 0.0f)
	{
		if (loop)
			this->phase += phaseMax - (int)this->phase;
		else
		{
			this->phase = 0.0f;
			result = false;
		}
	}

	float oscillationPhase = 1.0f - AxMath::Abs(this->phase - 1.0f);
	*(float*)this->parameter.value = min + (max - min) * oscillationPhase;

    return result;
}

void AxLinearMechanism::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	writer.BeginChunk(AxLinearMechanism::SerializationId_Phase);
	writer.stream->WriteFloat32(this->phase);
	writer.EndChunk();

	AxMechanism::SerializeChunks(writer);
}

bool AxLinearMechanism::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxMechanism::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxLinearMechanism::SerializationId_Phase:
		{
			this->phase = reader.stream->ReadFloat32();
			break;
		}
	}

	return true;
}