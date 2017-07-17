//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSound.h"

#include "..\..\Axis.h"

AxSound::AxSound(Axis *context)
	: AxResource()
{
	this->context = context;

	this->typeId = AxSound::classId;
	this->resourceType = AxResourceType_Sound;

	this->deviceSound = this->context->audioDevice->CreateSound();

	this->mediaSource = 0;
	this->mediaSourceStream = 0;
}


AxSound::~AxSound(void)
{
	if (this->mediaSourceStream != 0)
		delete this->mediaSourceStream;
	if (this->mediaSource != 0)
		delete this->mediaSource;

	delete this->deviceSound;
}

void AxSound::Load(AxSoundMedia *sourceMedia)
{
	this->deviceSound->SetMediaSource(sourceMedia);

	if (this->deviceSound->IsStreaming())
	{
		this->mediaSourceStream = sourceMedia->sourceStream;
		this->mediaSource = sourceMedia;
	}
	else
	{
		delete sourceMedia->sourceStream;
		delete sourceMedia;

		this->mediaSourceStream = 0;
		this->mediaSource = 0;
	}
}

bool AxSound::Load(AxStream *source)
{
	AxSoundMedia *sourceMedia = this->context->CreateSoundMedia(*source);

	if (sourceMedia == 0)
		return false;

	this->Load(sourceMedia);

	return true;
}

bool AxSound::Load(AxString &fileName)
{
	AxStream *sourceStream = this->context->fileSystem->OpenFile(fileName);
	if (sourceStream == 0)
		return false;

	if (this->Load(sourceStream))
	{
		this->fileName = fileName;
		return true;
	}

	return false;
}

void AxSound::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxResource::SerializeChunks(writer);

	writer.BeginChunk(AxSound::SerializationId_FileName);
	writer.stream->WriteString(AxFileSystem::GetRelativeFilePath(this->context->serializationParameters.rootDir, this->fileName));
	writer.EndChunk();
}

bool AxSound::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxSound::SerializationId_FileName:
		{
			AxString fileName = reader.stream->ReadString();
			this->Load(AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, fileName));
			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
