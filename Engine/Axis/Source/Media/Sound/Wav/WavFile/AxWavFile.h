#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\Streams\AxStream.h"

class AXDLLCLASS AxWavFile
{
private:
	static const unsigned int	chunk_RIFF	= 0x46464952;		// The main chunk. Denotes a RIFF (Resource Interchange File Format) container format
	static const unsigned int	chunk_fmt	= 0x20746d66;		// Subchunk of RIFF. Contains the PCM format parameters
	static const unsigned int	chunk_data	= 0x61746164;		// Subchunk of RIFF. Contains the PCM data.

	static const unsigned int	riffType_WAVE	= 0x45564157;

	static const unsigned int	audioFormat_PCM	= 0x00000001;

	AxStream *source;

	long long mainChunkEnd;

	bool Read();

public:
	unsigned short audioFormat;		// 1 - PCM. Other format encodings are not supported by this implementation
	unsigned short numChannels;		// 1 - mono, 2 - stereo
	unsigned int sampleRate;		// 22050, 44100, etc.
	unsigned int byteRate;			// == SampleRate * NumChannels * BitsPerSample / 8
	unsigned short blockAlign;		// == NumChannels * BitsPerSample / 8. The number of bytes for one sample
	unsigned short bitsPerSample;	// 8, 16, etc.

	long long dataStart, dataLength;
	
	AxWavFile(void);
	~AxWavFile(void);

	// Opens the wav file and reads its header information
	bool Initialize(AxStream *source);
	// Reads PCM data at given position from the wav file into a memory buffer
	int ReadData(void *destination, unsigned int position, unsigned int length);
};

