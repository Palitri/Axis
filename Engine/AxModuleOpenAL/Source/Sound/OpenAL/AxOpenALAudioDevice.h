#pragma once

#include "..\..\AxisEngine.h"

#include "AxOpenALSoundEmitter.h"

#include <OpenAL\al.h>
#include <OpenAL\alc.h>

class AxOpenALAudioDevice :
	public AxAudioDevice
{
private:
	static int AxOpenALAudioDevice::StreamingThreadFunction(void *parameter);

	ALCdevice *device;
	ALCcontext *context;

	AxList<AxAudioDeviceSoundEmitter*> streamingSounds;
	AxMutex *streamingThreadMutex;
	AxThread *streamingThread;
	bool processStreaming;

public:
	AxOpenALAudioDevice(void);
	~AxOpenALAudioDevice(void);

	void AddStreamingSoundEmitter(AxAudioDeviceSoundEmitter *soundEmitter);
	void RemoveStreamingSoundEmitter(AxAudioDeviceSoundEmitter *soundEmitter);

	virtual AxAudioDeviceSound *CreateSound();
	virtual AxAudioDeviceSoundEmitter *CreateSoundEmitter();
	virtual AxAudioDeviceSoundReceptor *CreateSoundReceptor();
};

