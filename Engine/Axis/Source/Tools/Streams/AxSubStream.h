#pragma once

#include "..\..\AxGlobals.h"

#include "AxStream.h"

class AXDLLCLASS AxSubStream :
	public AxStream
{
public:
	AxStream *mainStream;

	long long offset;
	bool writeBeyondLength;

	AxSubStream(AxStream &mainStream, long long length = -1, long long offset = -1, bool writeBeyondLength = true);
	~AxSubStream(void);

	virtual long long ReadData(void *data, long long size);
	virtual long long WriteData(const void *data, long long size);
};

