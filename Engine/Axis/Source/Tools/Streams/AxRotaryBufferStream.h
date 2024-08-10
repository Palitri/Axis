#pragma once

#include "..\..\AxGlobals.h"

#include "AxStream.h"

enum AxRotaryBufferStreamWriteMode
{
	AxRotaryBufferStreamWriteMode_SeparatePosition,
	AxRotaryBufferStreamWriteMode_SharedPosition
};

class AXDLLCLASS AxRotaryBufferStream :
	public AxStream
{
private:
	void *buffer;
	long long bufferSize;

	AxRotaryBufferStreamWriteMode writeMode;
	long long writePosition;

public:
	AxRotaryBufferStream(int bufferSize, AxRotaryBufferStreamWriteMode writeMode = AxRotaryBufferStreamWriteMode_SeparatePosition);
	virtual ~AxRotaryBufferStream(void);

	virtual long long ReadData(void *data, long long size);
	virtual long long WriteData(const void *data, long long size);
};

