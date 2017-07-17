#pragma once

#include "..\AxGlobals.h"

#include "AxAudioDeviceSound.h"
#include "AxAudioDeviceSoundEmitter.h"
#include "AxAudioDeviceSoundReceptor.h"

class AXDLLCLASS AxAudioDevice
{
public:
	AxAudioDevice(void);
	virtual ~AxAudioDevice(void);

	virtual AxAudioDeviceSound *CreateSound() = 0;
	virtual AxAudioDeviceSoundEmitter *CreateSoundEmitter() = 0;
	virtual AxAudioDeviceSoundReceptor *CreateSoundReceptor() = 0;
};

