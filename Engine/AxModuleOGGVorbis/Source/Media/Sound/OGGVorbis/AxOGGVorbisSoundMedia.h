#pragma once

#include "..\..\..\AxisEngine.h"

#include <Vorbis\vorbis\vorbisfile.h>

class AxOGGVorbisSoundMedia :
	public AxSoundMedia
{
private:
	static size_t ReadCallback(void *ptr, size_t size, size_t nmemb, void *datasource);
	static int SeekCallback(void *datasource, ogg_int64_t offset, int whence);
	static int CloseCallback(void *datasource);
	static long TellCallback(void *datasource);

	bool opened;

	OggVorbis_File oggVorbisFile;
	
	long long streamStartOffset, position;

	void Clear();

public:
	AxOGGVorbisSoundMedia(void);
	~AxOGGVorbisSoundMedia(void);

	virtual bool Load(AxStream &source);
	virtual bool Save(AxStream &destination, const char *format);

	virtual bool SetPosition(unsigned int position);
	virtual unsigned int ReadData(unsigned int bytesToRead);
};

