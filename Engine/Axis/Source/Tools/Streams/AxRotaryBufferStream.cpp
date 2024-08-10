#include "AxRotaryBufferStream.h"

#include "..\AxMem.h"

#include "..\AxMath.h"


AxRotaryBufferStream::AxRotaryBufferStream(int bufferSize, AxRotaryBufferStreamWriteMode writeMode)
	: AxStream()
{
	this->buffer = AxMem::Get((int)bufferSize);
	this->bufferSize = bufferSize;

	this->writeMode = writeMode;
	this->writePosition = 0;
}


AxRotaryBufferStream::~AxRotaryBufferStream(void)
{
	AxMem::FreeAndNull(&this->buffer);
}


long long AxRotaryBufferStream::ReadData(void *data, long long size)
{
	long long minPositionAvailable = this->length - this->bufferSize;
	this->position = AxMath::Max(this->position, minPositionAvailable);

	long long sizeAvailable = AxMath::Min(size, this->length - this->position);
	long long bufferPosition = this->position % this->bufferSize;

	int bytesAvailableFromPos = this->bufferSize - bufferPosition;
	int bytesRemainingFromBeginning = sizeAvailable - bytesAvailableFromPos;
	if (bytesRemainingFromBeginning > 0)
	{
		AxMem::Copy(data, (void*)((unsigned int)this->buffer + (unsigned int)bufferPosition), bytesAvailableFromPos);
		AxMem::Copy((void*)((unsigned int)data + (unsigned int)bytesAvailableFromPos), this->buffer, bytesRemainingFromBeginning);
	}
	else
	{
		AxMem::Copy(data, (void*)((unsigned int)this->buffer + (unsigned int)bufferPosition), sizeAvailable);
	}

	this->position += sizeAvailable;

	return sizeAvailable;
};

long long AxRotaryBufferStream::WriteData(const void *data, long long size)
{
	long long position = this->writeMode == AxRotaryBufferStreamWriteMode_SeparatePosition ? this->writePosition : this->position;

	long long minPositionAvailable = this->length - this->bufferSize;
	position = AxMath::Max(position, minPositionAvailable);

	long long sizeAvailable = AxMath::Min(size, this->bufferSize);
	long long bufferPosition = position % this->bufferSize;

	int bytesAvailableFromPos = this->bufferSize - bufferPosition;
	int bytesRemainingFromBeginning = sizeAvailable - bytesAvailableFromPos;
	if (bytesRemainingFromBeginning > 0)
	{
		AxMem::Copy((void*)((unsigned int)this->buffer + (unsigned int)bufferPosition), data, bytesAvailableFromPos);
		AxMem::Copy(this->buffer, (void*)((unsigned int)data + (unsigned int)bytesAvailableFromPos), bytesRemainingFromBeginning);
	}
	else
	{
		AxMem::Copy((void*)((unsigned int)this->buffer + (unsigned int)bufferPosition), data, sizeAvailable);
	}

	position += sizeAvailable;
	this->length = AxMath::Max(this->length, position);

	if (this->writeMode == AxRotaryBufferStreamWriteMode_SeparatePosition)
		this->writePosition = position;
	else if (this->writeMode == AxRotaryBufferStreamWriteMode_SharedPosition)
		this->position = position;

	return sizeAvailable;
};
