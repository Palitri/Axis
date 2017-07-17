//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOGGVorbisSoundMedia.h"


size_t AxOGGVorbisSoundMedia::ReadCallback(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	AxOGGVorbisSoundMedia *media = (AxOGGVorbisSoundMedia*)datasource;

	return (size_t)media->sourceStream->ReadData(ptr, size * nmemb);
}

int AxOGGVorbisSoundMedia::SeekCallback(void *datasource, ogg_int64_t offset, int whence)
{
	AxOGGVorbisSoundMedia *media = (AxOGGVorbisSoundMedia*)datasource;

	if (media->sourceStream->Seek(offset, whence == SEEK_SET ? StreamSeekMode_FromBeginning : whence == SEEK_CUR ? StreamSeekMode_Relative : StreamSeekMode_FromEnd))
		return 0;
	else
		return -1;
}

int AxOGGVorbisSoundMedia::CloseCallback(void *datasource)
{
	return 0;
}

long AxOGGVorbisSoundMedia::TellCallback(void *datasource)
{
	AxOGGVorbisSoundMedia *media = (AxOGGVorbisSoundMedia*)datasource;
	return (long)(media->sourceStream->position - media->streamStartOffset);
}

AxOGGVorbisSoundMedia::AxOGGVorbisSoundMedia(void)
{
	this->opened = false;
}


AxOGGVorbisSoundMedia::~AxOGGVorbisSoundMedia(void)
{
	this->Clear();
}

bool AxOGGVorbisSoundMedia::Load(AxStream &source)
{
	if (!AxSoundMedia::Load(source))
		return false;

	this->Clear();
	
	this->streamStartOffset = source.position;
	
	ov_callbacks callbacks;
	callbacks.read_func = AxOGGVorbisSoundMedia::ReadCallback;
	callbacks.seek_func = AxOGGVorbisSoundMedia::SeekCallback;
	callbacks.close_func = AxOGGVorbisSoundMedia::CloseCallback;
	callbacks.tell_func = AxOGGVorbisSoundMedia::TellCallback;

	if (ov_open_callbacks(this, &this->oggVorbisFile, 0, 0, callbacks) < 0)
		return false;
	
	// OGG samples are always 2-byte sized (16bit)
	vorbis_info *vorbisInfo = ov_info(&this->oggVorbisFile, -1);
	this->pcmFormat = AxPCMFormat(2, vorbisInfo->rate, vorbisInfo->channels);

	this->totalMediaSamplesCount = (unsigned int)ov_pcm_total(&this->oggVorbisFile, -1);

	this->position = 0;

	this->opened = true;

	return true;
}

void AxOGGVorbisSoundMedia::Clear()
{
	if (this->opened)
	{
		ov_clear(&this->oggVorbisFile);
		
		this->opened = false;
	}
}

bool AxOGGVorbisSoundMedia::Save(AxStream &destination, const char *format)
{
	return false;
}

bool AxOGGVorbisSoundMedia::SetPosition(unsigned int position)
{
	if (!this->opened)
		return false;

	long long channelsSampleSize = (this->pcmFormat.sampleSizeInBytes * this->pcmFormat.channelsCount);
	long long currentSampleIndex = this->position / channelsSampleSize;
	long long seekSampleIndex = position / channelsSampleSize;

	if (currentSampleIndex != seekSampleIndex)
	{
		if (ov_pcm_seek(&this->oggVorbisFile, seekSampleIndex) == 0)
		{
			this->position = seekSampleIndex * channelsSampleSize;
		}
		else
			return false;
	}

	return true;
}

unsigned int AxOGGVorbisSoundMedia::ReadData(unsigned int bytesToRead)
{
	if (!this->opened)
		return 0;

	if (bytesToRead > this->dataSize)
		bytesToRead = this->dataSize;

	int bitStream;
	long bytesRead;
	this->dataBytesRead = 0;
	char *destination = (char*)this->data;
	do
	{
		bytesRead = ov_read(&this->oggVorbisFile, destination, bytesToRead, 0, this->pcmFormat.sampleSizeInBytes, 1, &bitStream);

		destination += bytesRead;
		bytesToRead -= bytesRead;
		this->dataBytesRead += bytesRead;
	}
	while ((bytesRead > 0) && (bytesToRead > 0));

	this->position += this->dataBytesRead;

	return this->dataBytesRead;
}

