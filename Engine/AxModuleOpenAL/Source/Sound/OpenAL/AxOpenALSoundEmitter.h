#pragma once

#include "..\..\AxisEngine.h"

#include "AxOpenALSound.h"

#include <OpenAL\al.h>

class AxOpenALAudioDevice;

class AxOpenALSoundEmitter :
	public AxAudioDeviceSoundEmitter
{
private:
	static const int parameterAddress_Invalid		= -1;
	static const int parameterAddress_Sound			= 1;
	static const int parameterAddress_WorldMatrix	= 2;
	static const int parameterAddress_Velocity		= 3;
	static const int parameterAddress_Loop			= 4;
	static const int parameterAddress_Rate			= 5;
	static const int parameterAddress_Volume		= 6;
	static const int parameterAddress_Ambient		= 7;
	static const int parameterAddress_Play			= 8;

	ALuint handle;

	AxOpenALSound *soundBuffer;

	bool isLooping, isAmbient, isPlaying;

public:
	AxOpenALAudioDevice *context;

	AxOpenALSoundEmitter(void);
	~AxOpenALSoundEmitter(void);

	virtual void Play();
	virtual void Stop();

	virtual bool RefillStreamingBuffers();

	virtual int GetParameterAddress(AxString parameterName);
	virtual void SetParameter(int address, int index, void *value);
};

