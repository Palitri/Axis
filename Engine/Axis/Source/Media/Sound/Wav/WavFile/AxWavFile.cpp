//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWavFile.h"

#include "..\..\..\..\Tools\AxMath.h"

AxWavFile::AxWavFile(void)
{
	this->mainChunkEnd = -1;
	this->dataStart = -1;

	this->source = 0;
}


AxWavFile::~AxWavFile(void)
{
}

bool AxWavFile::Read()
{
	unsigned int chunkId = this->source->ReadUInt32();
	unsigned int chunkSize = this->source->ReadUInt32();
	long long chunkStart = this->source->position;

	switch (chunkId)
	{
		case AxWavFile::chunk_RIFF:
		{
			this->mainChunkEnd = this->source->position + chunkSize;
			
			unsigned int riffType = this->source->ReadUInt32();
			if (riffType != AxWavFile::riffType_WAVE)
				return false;

			break;
		}

		case AxWavFile::chunk_fmt:
		{
			this->audioFormat = this->source->ReadUInt16();
			this->numChannels = this->source->ReadUInt16();
			this->sampleRate = this->source->ReadUInt32();
			this->byteRate = this->source->ReadUInt32();
			this->blockAlign = this->source->ReadUInt16();
			this->bitsPerSample = this->source->ReadUInt16();

			this->source->Seek(chunkStart + chunkSize, StreamSeekMode_FromBeginning);

			break;
		}

		case AxWavFile::chunk_data:
		{
			this->dataStart = this->source->position;
			this->dataLength = chunkSize;

			return true;
		}

		default:
		{
			if ((chunkSize < 0) || (this->source->position + chunkSize > this->mainChunkEnd))
				return false;

			this->source->Seek(chunkSize, StreamSeekMode_Relative);

			break;
		}
	}

	if (this->mainChunkEnd < 0)
		return false;

	if (this->source->position + 8 < this->mainChunkEnd)
        return this->Read();

	return true;
}

bool AxWavFile::Initialize(AxStream *source)
{
	this->source = source;

	return this->Read();
}

int AxWavFile::ReadData(void *destination, unsigned int position, unsigned int length)
{
	if (this->source == 0)
		return -1;

	if (this->dataStart == -1)
		return -1;

	long long pos = this->dataStart + position;
	if (pos >= this->source->length)
		return 0;

	long long readLength = AxMath::Min(length, this->dataLength - position);

	this->source->Seek(pos);
	readLength = this->source->ReadData(destination, readLength);

	return (int)readLength;
}
