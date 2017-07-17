//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenALAudioDevice.h"

#include "AxOpenALSound.h"
#include "AxOpenALSoundReceptor.h"

int AxOpenALAudioDevice::StreamingThreadFunction(void *parameter)
{
	AxOpenALAudioDevice *device = (AxOpenALAudioDevice*)parameter;

	while (device->processStreaming)
	{
		device->streamingThreadMutex->Lock();

		for (int i = 0; i < device->streamingSounds.count; i++)
		{
			device->streamingSounds[i]->RefillStreamingBuffers();
		}
		
		device->streamingThreadMutex->Unlock();

		AxPlatformUtils::SleepX((int)(AxOpenALSound::StreamingBufferDuration * 1000.0f / 3.0f));
	}

	return 0;
}

AxOpenALAudioDevice::AxOpenALAudioDevice(void)
{
	const ALCchar* deviceName = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);

	this->device = alcOpenDevice(0);
	if (this->device == 0)
		return;

	this->context = alcCreateContext(this->device, 0);
	if (this->context == 0)
		return;

	alcMakeContextCurrent(this->context);

	this->processStreaming = true;
	this->streamingThreadMutex = new AxMutex();
	this->streamingThread = new AxThread(AxOpenALAudioDevice::StreamingThreadFunction, this);
}


AxOpenALAudioDevice::~AxOpenALAudioDevice(void)
{
	this->processStreaming = false;
	delete this->streamingThread;
	delete this->streamingThreadMutex;

	alcMakeContextCurrent(0);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}

void AxOpenALAudioDevice::AddStreamingSoundEmitter(AxAudioDeviceSoundEmitter *soundEmitter)
{
	this->streamingThreadMutex->Lock();

	if (this->streamingSounds.IndexOf(soundEmitter) == -1)
		this->streamingSounds.Add(soundEmitter);

	this->streamingThreadMutex->Unlock();
}

void AxOpenALAudioDevice::RemoveStreamingSoundEmitter(AxAudioDeviceSoundEmitter *soundEmitter)
{
	this->streamingThreadMutex->Lock();

	this->streamingSounds.Remove(soundEmitter);

	this->streamingThreadMutex->Unlock();
}

AxAudioDeviceSound *AxOpenALAudioDevice::CreateSound()
{
	return new AxOpenALSound();
}

AxAudioDeviceSoundEmitter *AxOpenALAudioDevice::CreateSoundEmitter()
{
	AxOpenALSoundEmitter *result = new AxOpenALSoundEmitter();
	result->context = this;
	
	return result;
}

AxAudioDeviceSoundReceptor *AxOpenALAudioDevice::CreateSoundReceptor()
{
	return new AxOpenALSoundReceptor();
}
