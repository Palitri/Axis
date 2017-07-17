#pragma once

#include "..\..\AxGlobals.h"

#include "AxResourceType.h"

#include "AxEntity.h"

#include "..\Properties\AxProperty.h"

#include "..\..\Tools\AxString.h"
#include "..\..\Tools\AxList.h"

#include "..\Utilities\Serialization\AxHierarchyStreamWriter.h"
#include "..\Utilities\Serialization\AxHierarchyStreamReader.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxProperty*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxParameter*>;

class AXDLLCLASS AxResource :
	public AxEntity
{
private:
	static const unsigned int SerializationId_Resource		= 0x21110000;
	static const unsigned int SerializationId_ResourceId	= 0x21111000;
	static const unsigned int SerializationId_ResourceName	= 0x21112000;
	static const unsigned int SerializationId_Properties	= 0x21113000;
	static const unsigned int SerializationId_Property		= 0x21113100;

	AxList<AxParameter*> oldProperties;

public:
	static const int propertyIndex_Enabled = 0;
	static const int propertyIndex_ChildPropertiesIndex = 1;

	AxResourceType resourceType;
    long long id;
    AxString name;

	void *tag;

	AxList<AxProperty*> properties;

	AxResource();
	virtual ~AxResource(void);

	AxProperty *GetProperty(const AxString &name);


	// Writes a parameter's type and value into a stream
	void SerializeParameter(AxParameter &parameter, AxStream &destination);
	// Readss a parameter's type and value from a stream
	void DeserializeParameter(AxParameter &parameter, AxStream &source);

	// Calls SerializeChunks() method, wrapping its whole output in a SerializationId_Resource
	virtual void Serialize(AxStream &destination);
	// Reads the contents wrapped in a SerializationId_Resource and calls DeserializeChunk() method for each chunk of these chunks. Override to perform pre or post serialization routines
	virtual bool Deserialize(AxStream &source);
	
	// Writes chunks for all the data which is needed to serialize the resource. Override to write chunks of resource-specific data
	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	// Reads chunks of all the data which is needed to deserialize the resource. Override to read chunks of resource-specific data
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);

	bool NotifyPropertiesChange(bool checkNewProperties = true);

	virtual void OnPropertiesChanged();
	virtual void OnPropertyChanged(int propertyIndex);

};

