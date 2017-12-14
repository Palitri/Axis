#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

class Axis;

class AXDLLCLASS AxTimelineAnimationKey
{
public:
	float time, value;

	AxTimelineAnimationKey()
	{
		this->time = 0.0f;
		this->value = 0.0f;
	}

	AxTimelineAnimationKey(float time, float value)
	{
		this->time = time;
		this->value = value;
	}
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxTimelineAnimationKey>;

class AXDLLCLASS AxTimelineAnimationMechanism :
	public AxMechanism
{
private:
	static const unsigned int SerializationId_TimelineTrack		= 0x21111028;

	int lastKeyIndex;

public:
	static const int classId = AxMechanism::classId | 112;

	static const int propertyIndex_Frame	= AxResource::propertyIndex_ChildPropertiesIndex + 0;

	AxList<AxTimelineAnimationKey> keys;

	AxTimelineAnimationMechanism(void);
	virtual ~AxTimelineAnimationMechanism(void);

	bool Process(float deltaTime);

	// Serialize timeline data. See base class method for more info
	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	// Deserialize timeline data. See base class method for more info
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};

