#pragma once

#include "..\AxGlobals.h"

#include "..\Media\Sound\AxSoundMedia.h"

class AXDLLCLASS AxAudioDeviceSound
{
public:
	AxAudioDeviceSound(void);
	virtual ~AxAudioDeviceSound(void);

	// Returns true if the sound is streaming its media source or false if the sound is fully loaded from the media source into memory and the media source is not required any more
	virtual bool IsStreaming() = 0;
	// Sets a sound media as a source of the sound. The media source might be required to play the sound, if it is streaming, otherwise the media source is not futherly required. To check if the sound is streaming, use the IsStreaming() method;
	virtual void SetMediaSource(AxSoundMedia *mediaSource) = 0;
	// Loads the whole data into memory. The resulting sound is not streaming
	//virtual void Load(void *data, int dataSize, AxPCMFormat *format);
};

