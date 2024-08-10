#pragma once

#include "..\AxGlobals.h"

class AXDLLCLASS AxMem
{
public:
	AxMem(void);
	~AxMem(void);

	static void* Get(unsigned int size);
	static void FreeAndNull(void **data);
	static void Zero(void* data, int size);
	static void Fill(void* data, int dataSize, const void *value, int valueSize);
	static void Copy(void* destination, const void* source, int size);
	static int Compare(const void* block1, const void* block2, int size);
	static void CopyBits(const void *src, int srcBitOffset, void *dest, int destBitOffset, int bitCount);
	static void ReverseByteOrder(void *data, int size);
	static void CopyReversedByteOrder(void* destination, const void* source, int size);
};

