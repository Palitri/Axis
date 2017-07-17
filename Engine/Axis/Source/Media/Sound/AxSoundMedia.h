#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxMedia.h"

#include "..\..\Engine\Primitive\AxPCMFormat.h"

class AXDLLCLASS AxSoundMedia :
	public AxMedia
{
public:
	AxPCMFormat pcmFormat;
	unsigned int totalMediaSamplesCount;
	// Data is kept in this class rather than passed in ReadData(), in order to be able to be able to change its pcm format, similarly as the data in AxImage can change its pixel format
	unsigned int dataSize, dataBytesRead;
	void *data;

	AxStream *sourceStream;

	AxSoundMedia(void);
	virtual ~AxSoundMedia(void);

	virtual bool Load(AxStream &source);

	// Sets the position from which data will be read
	virtual bool SetPosition(unsigned int position) = 0;
	// Reads data from the current position and moves the current position
	virtual unsigned int ReadData(unsigned int bytesToRead) = 0;

	
	// Reads data from the current position and if the end of data is reached, automatically continues the reading from the beginning. Typically used for loading data into streaming audio
	unsigned int ReadDataLooping(unsigned int bytesToRead);

	// Sets the size of the maximum data that can be read
	void SetSize(unsigned int dataSize);

	// Returns the total duration of the whole sound, in seconds
	float GetTotalDuration();

};

