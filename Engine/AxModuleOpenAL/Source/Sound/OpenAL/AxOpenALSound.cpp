//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenALSound.h"

const float AxOpenALSound::ResidentBufferMaxDuration = 10.0f;
const float AxOpenALSound::StreamingBufferDuration = 2.0f;


AxOpenALSound::AxOpenALSound(void)
{
	this->bufferIds = 0;
	this->buffersCount = 0;

	this->mediaSource = 0;
}


AxOpenALSound::~AxOpenALSound(void)
{
	this->Clear();
}

void AxOpenALSound::Clear()
{
	if (this->bufferIds != 0)
	{
		alDeleteBuffers(this->buffersCount, this->bufferIds);

		delete[] this->bufferIds;
		this->bufferIds = 0;
	}

	this->mediaSource = 0;
}

bool AxOpenALSound::IsStreaming()
{
	return this->isStreaming;
}

void AxOpenALSound::SetMediaSource(AxSoundMedia *mediaSource)
{
	this->Clear();

	this->isStreaming = mediaSource->GetTotalDuration() > AxOpenALSound::ResidentBufferMaxDuration;

	this->buffersCount = this->isStreaming ? 2 : 1;
	this->bufferIds = new ALuint[this->buffersCount];

	unsigned int buffersLength = this->isStreaming ? (unsigned int)(AxOpenALSound::StreamingBufferDuration * (float)(mediaSource->pcmFormat.frequency * mediaSource->pcmFormat.sampleSizeInBytes * mediaSource->pcmFormat.channelsCount)) : mediaSource->totalMediaSamplesCount * mediaSource->pcmFormat.sampleSizeInBytes * mediaSource->pcmFormat.channelsCount;

	this->alFormat = mediaSource->pcmFormat.sampleSizeInBytes == 1 ? (mediaSource->pcmFormat.channelsCount == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8) : (mediaSource->pcmFormat.channelsCount == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);

	alGenBuffers(this->buffersCount, this->bufferIds);
	
	mediaSource->SetSize(buffersLength);

	for (int i = 0; i < this->buffersCount; i++)
	{
		unsigned int readBytes = mediaSource->ReadData(mediaSource->dataSize);
		
		alBufferData(this->bufferIds[i], this->alFormat, mediaSource->data, readBytes, mediaSource->pcmFormat.frequency);
	}

	this->mediaSource = this->isStreaming ? mediaSource : 0;
}
