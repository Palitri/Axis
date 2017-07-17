#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

class AXDLLCLASS AxMechanism
	: public AxResource
{
private:
	static const unsigned int SerializationId_Parameter	= 0x21111008;

public:
	static const int classId = (AxResourceType_Mechanism << 16) | 0;

	AxParameter parameter;

    bool _updateState;

	AxMechanism(void);
	virtual ~AxMechanism(void);

	virtual bool Process(float deltaTime);

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};
