#pragma once

#include "..\..\Tools\Streams\AxStream.h"

class AXDLLCLASS AxEntity
{
public:
	// typeId can be assigned to a static member of a AxClassId class, which in its constructor can assign a unique AxClassId.id. This way we could have, for example AxMesh.typeId = AxMesh::classId.id. However it would be a bit obscure, because even if this will ensure the uniqueness of the ids, it wont ensure that the same class will always have the same id assigned, which is vital for serialization.
	int typeId;

	AxEntity(void);
	virtual ~AxEntity(void);

	// Serializes the object into the stream
	virtual void Serialize(AxStream &destination) = 0;
	// Deserializes the object from the stream
	virtual bool Deserialize(AxStream &source) = 0;
};

