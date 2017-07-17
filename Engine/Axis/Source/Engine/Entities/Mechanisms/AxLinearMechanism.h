#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

class AXDLLCLASS AxLinearMechanism
	: public AxMechanism
{
private:
	static const unsigned int SerializationId_Phase	= 0x21111038;

	float phase;

public:
	static const int classId = AxMechanism::classId | 103;

	static const int propertyIndex_Min			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Max			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Speed		= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Timed		= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_Loop			= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_Oscillate	= AxResource::propertyIndex_ChildPropertiesIndex + 5;

	AxLinearMechanism(float min, float max, float speed, bool timed, bool loop, bool oscillate);
	virtual ~AxLinearMechanism(void);

	bool Process(float deltaTime);

	// Serialize the phase value
	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	// Deserialize the phase value
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);

};

