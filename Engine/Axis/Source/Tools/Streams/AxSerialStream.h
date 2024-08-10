#pragma once

#include "..\..\AxGlobals.h"

#include "AxStream.h"

#include "AxRotaryBufferStream.h"
#include "..\Threading\AxThread.h"
#include "..\Threading\AxMutex.h"

class AXDLLCLASS AxSerialStream :
	public AxStream
{
private:
	void* comPortHandle;

	AxRotaryBufferStream *buffer;

	AxThread *readThread;
	AxMutex *readMutex;

	static int ReadPortAsync(void *parameter);

public:
	AxSerialStream(AxString comPortName, int baudRate, int bufferSize = 65536);
	virtual ~AxSerialStream(void);

	virtual long long ReadData(void *data, long long size);
	virtual long long WriteData(const void *data, long long size);

	void ReadPort();
};

