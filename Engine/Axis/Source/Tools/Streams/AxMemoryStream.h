#pragma once

#include "..\..\AxGlobals.h"

#include "AxStream.h"

enum StreamWriteMode
{
	StreamWriteMode_Overwrite,
	StreamWriteMode_Insert
};

class AXDLLCLASS AxMemoryStream :
	public AxStream
{
public:
	StreamWriteMode writeMode;

	void *data;

	long long capacity;

	AxMemoryStream(int initialCapacity = 2048);
	AxMemoryStream(void *data, int size);
	~AxMemoryStream(void);

	virtual long long ReadData(void *data, long long size);
	virtual long long WriteData(const void *data, long long size);

	virtual void SetWriteMode(StreamWriteMode writeMode);

	void Resize(long long newCapacity);
	void *GetDataPointer(long long offset);

};

