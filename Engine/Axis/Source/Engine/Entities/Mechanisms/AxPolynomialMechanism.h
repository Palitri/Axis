#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

class AXDLLCLASS AxPolynomialMechanism
	: public AxMechanism
{
private:
	static const unsigned int SerializationId_Degree	= 0x21111028;

	static int GetDegreePropertyIndex(int degree);

public:
	static const int classId = AxMechanism::classId | 102;

	static const int propertyIndex_X		= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Degree0	= AxResource::propertyIndex_ChildPropertiesIndex + 1;

	int degree;

	AxPolynomialMechanism(int degree);
	virtual ~AxPolynomialMechanism(void);

	bool Process(float deltaTime);

	void SetDegree(int degree);
	
	// Serialize the degree value
	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	// Deserialize the degree value
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);

};

