#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxResource*>;

class AXDLLCLASS AxEntitySet
	: public AxResource
{
private:
	static const unsigned int SerializationId_References = 0x21111005;

public:
	static const int classId = (AxResourceType_EntitySet << 16) | 0;

	static const int propertyIndex_MultiPass	= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Branch		= AxResource::propertyIndex_ChildPropertiesIndex + 1;

	AxList<AxResource*> references;

	AxEntitySet(void);
	virtual ~AxEntitySet(void);

	bool HierarchyContains(AxResource* resource);

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};

