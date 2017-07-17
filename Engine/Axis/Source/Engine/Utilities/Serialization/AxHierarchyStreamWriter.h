#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxStack.h"
#include "..\..\..\Tools\Streams\AxStream.h"

class AXDLLCLASS AxHierarchyStreamWriter
{
private:
    AxStack<long long> chunkStack;

public:
    AxStream *stream;

	AxHierarchyStreamWriter(AxStream &stream);
	~AxHierarchyStreamWriter(void);
        
	void BeginChunk(unsigned int chunkId);
	void EndChunk();
};

