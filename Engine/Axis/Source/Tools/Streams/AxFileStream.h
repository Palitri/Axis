#pragma once

#include "..\..\AxGlobals.h"

#include "AxStream.h"

#include "..\AxString.h"

enum FileStreamAccessMode
{
	FileStreamAccessMode_Read,
	FileStreamAccessMode_Write,
	FileStreamAccessMode_ReadWrite
};

class AXDLLCLASS AxFileStream :
	public AxStream
{
private:
	void *fileStream_fstream;

public:
	AxFileStream(const AxString &fileName, FileStreamAccessMode accessMode);
	~AxFileStream(void);

	virtual long long ReadData(void *data, long long size);
	virtual long long WriteData(const void *data, long long size);

	virtual bool Seek(long long offset, StreamSeekMode seekType = StreamSeekMode_FromBeginning);
};