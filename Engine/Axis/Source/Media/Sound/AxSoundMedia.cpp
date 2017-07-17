//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSoundMedia.h"


AxSoundMedia::AxSoundMedia(void)
{
	this->data = 0;
	this->dataSize = 0;

	this->sourceStream = 0;
}


AxSoundMedia::~AxSoundMedia(void)
{
	this->SetSize(0);
}

bool AxSoundMedia::Load(AxStream &source)
{
	this->sourceStream = &source;

	return true;
}

void AxSoundMedia::SetSize(unsigned int dataSize)
{
	if (this->data != 0)
	{
		delete[] this->data;
		this->data = 0;
		this->dataSize = 0;
	}

	if (dataSize > 0)
	{
		this->dataSize = dataSize;
		this->data = new char[this->dataSize];
	}
}

float AxSoundMedia::GetTotalDuration()
{
	return (float)this->totalMediaSamplesCount / (float)this->pcmFormat.frequency;
}

unsigned int AxSoundMedia::ReadDataLooping(unsigned int bytesToRead)
{
	unsigned int bytesRead = this->ReadData(bytesToRead);

	unsigned int remainingBytes = bytesToRead - bytesRead;
	if (remainingBytes != 0)
	{
		unsigned int totalDataSize = this->pcmFormat.sampleSizeInBytes * this->pcmFormat.channelsCount * this->totalMediaSamplesCount;
		unsigned int lastPos = totalDataSize - bytesRead;
		if (remainingBytes > lastPos)
			remainingBytes = lastPos;
		
		this->SetPosition(0);

		void* d = this->data;
		this->data = (void*)((unsigned int)this->data + bytesRead);
		bytesRead += this->ReadData(remainingBytes);
		this->data = d;
	}

	return bytesRead;
}
