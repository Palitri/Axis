#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxSoundMedia.h"

#include "WavFile\AxWavFile.h"

class AXDLLCLASS AxWavSoundMedia :
	public AxSoundMedia
{
private:
	AxWavFile wavFile;
	unsigned int position;

public:
	AxWavSoundMedia(void);
	~AxWavSoundMedia(void);

	virtual bool Load(AxStream &source);
	virtual bool Save(AxStream &destination, const char *format);

	virtual bool SetPosition(unsigned int position);
	virtual unsigned int ReadData(unsigned int bytesToRead);
};

