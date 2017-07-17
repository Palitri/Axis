#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

#include "..\..\..\Sound\AxAudioDeviceSound.h"

#include "..\..\..\Media\Sound\AxSoundMedia.h"
#include "..\..\..\Tools\AxString.h"

class Axis;

class AXDLLCLASS AxSound :
	public AxResource
{
public:
	static const int classId = (AxResourceType_Sound << 16) | 0;

	static const unsigned int SerializationId_FileName	= 0x2111100a;

	Axis *context;

	AxStream *mediaSourceStream;
	AxSoundMedia *mediaSource;

	AxAudioDeviceSound *deviceSound;

	AxString fileName;

	AxSound(Axis *context);
	virtual ~AxSound(void);

	//// Loads the whole data into memory. The resulting sound is not streaming
	//void Load(void *data, int dataSize, const AxPCMFormat &format);
	// Loads the sound from a media and if the resulting sound is streaming, keeps the source media or if the resulting sound is not streaming, releases the source media. If the sound is streaming, the media is kept and the stream which it reads from must also be kept until the sound is destroyed.
	void Load(AxSoundMedia *sourceMedia);
	// Loads the sound from a stream and if the resulting sound is streaming, keeps the source stream or if the resulting sound is not streaming, releases the source stream.
	bool Load(AxStream *source);
	// Loads the sound from a file and if the resulting sound is streaming, keeps the file open or if the resulting sound is not streaming, closes the file.
	bool Load(AxString &fileName);

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};

