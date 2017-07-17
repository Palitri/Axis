#pragma once

#include "..\..\AxGlobals.h"

class AXDLLCLASS AxPCMFormat
{
public:
	int sampleSizeInBytes, frequency, channelsCount;

	AxPCMFormat(void);
	AxPCMFormat(int sampleSizeInBytes, int frequency, int channelsCount);
	~AxPCMFormat();
};