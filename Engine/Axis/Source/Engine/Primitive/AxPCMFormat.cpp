//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPCMFormat.h"

AxPCMFormat::AxPCMFormat(void)
{
}

AxPCMFormat::AxPCMFormat(int sampleSizeInBytes, int frequency, int channelsCount)
{
	this->sampleSizeInBytes = sampleSizeInBytes;
	this->frequency = frequency;
	this->channelsCount = channelsCount;
}

AxPCMFormat::~AxPCMFormat()
{
}
