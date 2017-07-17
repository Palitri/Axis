//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMemoryStream.h"

#include "..\AxMem.h"
#include "..\AxMath.h"

AxMemoryStream::AxMemoryStream(int initialCapacity)
	: AxStream()
{
	this->SetWriteMode(StreamWriteMode_Overwrite);
	
	this->capacity = 0;
	this->data = 0;

	this->Resize(initialCapacity);
}

AxMemoryStream::AxMemoryStream(void *data, int size)
	: AxStream()
{
	this->length = size;

	this->SetWriteMode(StreamWriteMode_Overwrite);
	
	this->capacity = size;
	this->data = data;
}


AxMemoryStream::~AxMemoryStream(void)
{
	AxMem::FreeAndNull(&this->data);
}

long long AxMemoryStream::ReadData(void *data, long long size)
{
	long long bytesToCopy = AxMath::Min(this->length - this->position, size);
	AxMem::Copy(data, (void*)((unsigned int)this->data + (unsigned int)this->position), (unsigned int)bytesToCopy);

	this->position += bytesToCopy;

	return bytesToCopy;
}

long long AxMemoryStream::WriteData(const void *data, long long size)
{
	long long requiredCapacity = this->writeMode == StreamWriteMode_Overwrite ? this->position + size : this->length + size;
	long long newCapacity = this->capacity;
	while (requiredCapacity > newCapacity)
	{
		newCapacity *= 2;
	}
	if (newCapacity != this->capacity)
		this->Resize(newCapacity);

	if (this->writeMode == StreamWriteMode_Insert)
	{
		long long tailBytes = this->length - this->position;
		if (size > tailBytes)
		{
			AxMem::Copy(this->GetDataPointer((unsigned int)this->position + size), this->GetDataPointer(this->position), (unsigned int)tailBytes);
		}
		else
		{
			void *tailData = AxMem::Get((unsigned int)tailBytes);
			AxMem::Copy(tailData, this->GetDataPointer(this->position), (unsigned int)tailBytes);
			AxMem::Copy(this->GetDataPointer(this->position + size), tailData, (unsigned int)tailBytes);
			AxMem::FreeAndNull(&tailData);
		}

		this->length += size;
	}

	AxMem::Copy(this->GetDataPointer(this->position), data, (unsigned int)size);

	this->position += size;

	this->length = AxMath::Max(this->length, this->position);

	return size;
}

void AxMemoryStream::SetWriteMode(StreamWriteMode writeMode)
{
	this->writeMode = writeMode;
}

void AxMemoryStream::Resize(long long newCapacity)
{
	void *newData = AxMem::Get((int)newCapacity);
	
	if (this->length > 0)
		AxMem::Copy(newData, this->data, (unsigned int)AxMath::Min(this->length, newCapacity));
	if (this->data != 0)
		AxMem::FreeAndNull(&this->data);

	this->data = newData;
	this->capacity = newCapacity;

};

void *AxMemoryStream::GetDataPointer(long long offset)
{
	return (void*)((int)this->data + offset);
}
