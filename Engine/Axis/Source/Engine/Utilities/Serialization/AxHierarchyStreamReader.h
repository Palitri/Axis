#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxStack.h"
#include "..\..\..\Tools\Streams\AxStream.h"

class AXDLLCLASS AxHierarchyStreamReader
{
public:
    AxStack<long long> chunkStack;

	AxStream *stream;

	unsigned int chunkId;
	long long chunkSize;

    AxHierarchyStreamReader(AxStream &stream);
	~AxHierarchyStreamReader(void);

    bool ReadChunkHeader();
    void SkipChunk();
};

