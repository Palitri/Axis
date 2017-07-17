#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Engine\Utilities\Serialization\AxHierarchyStreamReader.h"

#include "..\..\..\Tools\AxDictionary.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxDictionary<long long, long long>;

class Axis;

class AXDLLCLASS AxNativeSceneReader
{
private:
	AxDictionary<long long, long long> idMap;

	long long GetMappedResourceId(long long originalId);
	void LinkResources();

public:
	Axis *context;

	AxNativeSceneReader(Axis *context);
	~AxNativeSceneReader(void);

	bool Read(AxStream &source);
};

