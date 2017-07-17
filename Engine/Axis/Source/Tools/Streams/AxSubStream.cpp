//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSubStream.h"

#include "..\AxMath.h"

AxSubStream::AxSubStream(AxStream &mainStream, long long length, long long offset, bool writeBeyondLength)
{
	this->mainStream = &mainStream;
	this->offset = offset == -1 ? mainStream.position : offset;
	this->length = length == -1 ? mainStream.length - offset : length;
	this->writeBeyondLength = writeBeyondLength;
}


AxSubStream::~AxSubStream(void)
{
}

long long AxSubStream::ReadData(void *data, long long size)
{
	long long originalPosition = this->mainStream->position;

	size = AxMath::Min(size, this->position + this->length);

	this->mainStream->Seek(this->offset + this->position, StreamSeekMode_FromBeginning);
	long long result = this->mainStream->ReadData(data, size);
	this->mainStream->Seek(originalPosition, StreamSeekMode_FromBeginning);

	this->position += result;

	return result;
}

long long AxSubStream::WriteData(const void *data, long long size)
{
	long long originalPosition = this->mainStream->position;

	if (!this->writeBeyondLength)
		size = AxMath::Min(size, this->position + this->length);

	this->mainStream->Seek(this->offset + this->position, StreamSeekMode_FromBeginning);
	long long result = this->mainStream->WriteData(data, size);
	this->mainStream->Seek(originalPosition, StreamSeekMode_FromBeginning);

	this->position += result;
	if (this->position > this->length)
		this->length = this->position;

	return result;
}
