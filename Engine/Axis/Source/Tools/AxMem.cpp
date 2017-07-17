//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMem.h"

#include <string>  

#include "AxMath.h"

AxMem::AxMem(void)
{
}

AxMem::~AxMem(void)
{
}

void* AxMem::Get(unsigned int size)
{
	return malloc(size);
}

void AxMem::FreeAndNull(void **data)
{
	if (*data == 0)
		return;

	free(*data);
	*data = 0;
}

void AxMem::Zero(void* destination, int size)
{
	memset(destination, 0, size);
}

void AxMem::Fill(void* destination, int fillSize, const void *value, int valueSize)
{
	int chunks = fillSize / valueSize;
	int remains = fillSize % valueSize;

	for (int i = chunks; i != 0; i--)
	{
		memcpy(destination, value, valueSize);
		destination = (void*)((int)destination + valueSize);
	}

	if (remains > 0)
		memcpy(destination, value, remains);
}

void AxMem::Copy(void* destination, const void* source, int size)
{
	memcpy(destination, source, size);
}

int AxMem::Compare(const void* block1, const void* block2, int size)
{
	return memcmp(block1, block2, size);
}

void AxMem::CopyBits(const void *src, int srcBitOffset, void *dest, int destBitOffset, int bitCount)
{
	// TODO: Can be optimized to process larger (16, 32 bit) chunks while bitCount is large enough
	while (bitCount > 0)
	{
		if (srcBitOffset >= 8)
		{
			src = (void*)((unsigned int)src + (srcBitOffset >> 3));
			srcBitOffset &= 7;
		}

		if (destBitOffset >= 8)
		{
			dest = (void*)((unsigned int)dest + (destBitOffset >> 3));
			destBitOffset &= 7;
		}

		int bitsWritten = AxMath::Min(8 - AxMath::Max(srcBitOffset, destBitOffset), bitCount);

		unsigned char srcChunk = *(unsigned char*)src;
		srcChunk <<= srcBitOffset;
		srcChunk >>= destBitOffset;
		unsigned char mask = (~0);
		mask <<= 8 - bitsWritten;
		mask >>= destBitOffset;
		*(unsigned char*)dest &= (~mask);
		*(unsigned char*)dest |= (srcChunk & mask);


		srcBitOffset += bitsWritten;
		destBitOffset += bitsWritten;

		bitCount -= bitsWritten;
	}
}