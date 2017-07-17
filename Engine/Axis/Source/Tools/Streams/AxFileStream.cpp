//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFileStream.h"

#include "..\AxMem.h"

#include <fstream>

AxFileStream::AxFileStream(const AxString &fileName, FileStreamAccessMode accessMode)
	: AxStream()
{

	std::ios_base::openmode fileMode = accessMode == FileStreamAccessMode_Read ? std::ios::in : accessMode == FileStreamAccessMode_Write ? std::ios::out : accessMode == FileStreamAccessMode_ReadWrite ? std::ios::in | std::ios::out : 0;

	this->fileStream_fstream = new std::fstream;
	((std::fstream*)this->fileStream_fstream)->open(fileName.contents, fileMode | std::ios::binary | std::ios::ate);

	this->length = ((std::fstream*)this->fileStream_fstream)->tellg();
	
	this->Seek(0, StreamSeekMode_FromBeginning);
}


AxFileStream::~AxFileStream(void)
{
	((std::fstream*)this->fileStream_fstream)->close();
	delete ((std::fstream*)this->fileStream_fstream);
}

long long AxFileStream::ReadData(void *data, long long size)
{
	((std::fstream*)this->fileStream_fstream)->read((char*)data, size);

	long long bytesRead = ((std::fstream*)this->fileStream_fstream)->gcount();

	this->position += bytesRead;

	return bytesRead;
}

long long AxFileStream::WriteData(const void *data, long long size)
{
	((std::fstream*)this->fileStream_fstream)->write((char*)data, size);

	long long bytesWritten = size;

	this->position += bytesWritten;
	
	if (this->position > this->length)
		this->length = this->position;

	return bytesWritten;
}

bool AxFileStream::Seek(long long offset, StreamSeekMode seekType)
{
	if (AxStream::Seek(offset, seekType))
	{
		((std::fstream*)this->fileStream_fstream)->seekg(this->position);
		((std::fstream*)this->fileStream_fstream)->seekp(this->position);
	}

	return false;
}
