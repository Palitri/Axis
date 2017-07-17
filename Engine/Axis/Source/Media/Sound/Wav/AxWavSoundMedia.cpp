//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWavSoundMedia.h"


AxWavSoundMedia::AxWavSoundMedia(void)
{
}


AxWavSoundMedia::~AxWavSoundMedia(void)
{
}

bool AxWavSoundMedia::Load(AxStream &source)
{
	if (!AxSoundMedia::Load(source))
		return false;

	if (this->wavFile.Initialize(this->sourceStream))
	{
		this->pcmFormat = AxPCMFormat(this->wavFile.bitsPerSample / 8, this->wavFile.sampleRate, this->wavFile.numChannels);
		this->totalMediaSamplesCount = (unsigned int)(this->wavFile.dataLength / (this->pcmFormat.sampleSizeInBytes * this->pcmFormat.channelsCount));

		this->position = 0;

		return true;
	}

	return false;
}

bool AxWavSoundMedia::Save(AxStream &destination, const char *format) 
{ 
	return false;
}

bool AxWavSoundMedia::SetPosition(unsigned int position)
{
	if ((position >= 0) && (position < this->wavFile.dataLength))
	{
		this->position = position;
		return true;
	}

	return false;
}

unsigned int AxWavSoundMedia::ReadData(unsigned int bytesToRead)
{
	if (bytesToRead > this->dataSize)
		bytesToRead = this->dataSize;

	int readBytes = this->wavFile.ReadData(this->data, this->position, bytesToRead);
	this->position += readBytes;

	return readBytes;
}
