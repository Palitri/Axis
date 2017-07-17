#pragma once

#include "..\..\AxisEngine.h"

#include <OpenAL\al.h>

class AxOpenALSound :
	public AxAudioDeviceSound
{
private:
	void Clear();

public:
	// The maximum duration in seconds, of a buffer which is not streaming, but residently loaded in memory. Sounds with longer duration will use streaming buffers.
	static const float ResidentBufferMaxDuration;
	// The duration in seconds, of a buffer used in a queue for streaming audio.
	static const float StreamingBufferDuration;

	int buffersCount;
	ALuint *bufferIds;

	ALenum alFormat;

	bool isStreaming;
	AxSoundMedia *mediaSource;

	AxOpenALSound(void);
	~AxOpenALSound(void);

	virtual bool IsStreaming();
	virtual void SetMediaSource(AxSoundMedia *mediaSource);
};

