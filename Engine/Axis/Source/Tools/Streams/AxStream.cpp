//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxStream.h"

#include "..\AxMath.h"
#include "..\AxMem.h"

StreamException::StreamException(const char *message)
{
	this->message = AxString::CopyNew(message);
}

StreamException::~StreamException(void)
{
	AxString::Dispose(&this->message);
}

const char *StreamException::what() const throw()
{
	return this->message;
}



AxStream::AxStream(void)
{
	this->position = 0;
	this->length = 0;
}

AxStream::~AxStream(void)
{
}

bool AxStream::Seek(long long offset, StreamSeekMode seekType)
{
	long long newPosition = seekType == StreamSeekMode_FromBeginning ? offset : seekType == StreamSeekMode_Relative ? this->position + offset : seekType == StreamSeekMode_FromEnd ? this->length - offset : -1;

	if ((newPosition >= 0) && (newPosition <= this->length))
	{
		this->position = newPosition;
		return true;
	}

	return false;
}

long long AxStream::Peek(void *data, long long size)
{
	long long originalPosition = this->position;

	long long result = this->ReadData(data, size);

	this->Seek(originalPosition, StreamSeekMode_FromBeginning);

	return result;
}

long long AxStream::WriteStreamData(AxStream &source, long long size, int maxBufferSize)
{
	if (size == -1)
		size = source.length - source.position;

	long long chunkSize = AxMath::Min(size, (long long)maxBufferSize);
	void *dataChunk = AxMem::Get((int)chunkSize);
	
	long long bytesToWrite = size;
	while (bytesToWrite > 0)
	{
		long long bytesToRead = AxMath::Min(bytesToWrite, chunkSize);
		long long bytesRead = source.ReadData(dataChunk, bytesToRead);
		long long bytesWritten = this->WriteData(dataChunk, bytesRead);

		bytesToWrite -= bytesWritten;

		if (bytesToRead != bytesWritten)
			break;
	}
	
	AxMem::FreeAndNull(&dataChunk);

	return size - bytesToWrite;
}

bool AxStream::ReadBool()
{
	bool result;
	this->ReadData(&result, 1);
	return result;
}

void AxStream::WriteBool(bool value)
{
	this->WriteData(&value, 1);
}


char AxStream::ReadInt8()
{
	char result;
	this->ReadData(&result, 1);
	return result;
}

void AxStream::WriteInt8(char value)
{
	this->WriteData(&value, 1);
}

short AxStream::ReadInt16()
{
	short result;
	this->ReadData(&result, 2);
	return result;
}

void AxStream::WriteInt16(short value)
{
	this->WriteData(&value, 2);
}

int AxStream::ReadInt32()
{
	int result;
	this->ReadData(&result, 4);
	return result;
}

void AxStream::WriteInt32(int value)
{
	this->WriteData(&value, 4);
}

long long AxStream::ReadInt64()
{
	long long result;
	this->ReadData(&result, 8);
	return result;
}

void AxStream::WriteInt64(long long value)
{
	this->WriteData(&value, 8);
}


int AxStream::Read7BitEncodedInt32()
{
	int result = 0;
	char bits;
	do
	{
		this->ReadData(&bits, 1);
		result = (result << 7) | (bits & 0x7f);
	} 
	while ((bits & (1 << 7)) != 0);
		
	return result;
}

void AxStream::Write7BitEncodedInt32(int value)
{
	int writeValue = 0, writeBytes = 0;
 
	for (;;)
	{
		writeValue = (writeValue << 8) | (value & 0x7f);
		value = value >> 7;
		writeBytes++;

		if (value != 0)
			writeValue |= 0x80;
		else
			break;
	}

	this->WriteData(&writeValue, writeBytes);
}

unsigned char AxStream::ReadUInt8()
{
	unsigned char result;
	this->ReadData(&result, 1);
	return result;
}

void AxStream::WriteUInt8(unsigned char value)
{
	this->WriteData(&value, 1);
}

unsigned short AxStream::ReadUInt16()
{
	unsigned short result;
	this->ReadData(&result, 2);
	return result;
}

void AxStream::WriteUInt16(unsigned short value)
{
	this->WriteData(&value, 2);
}

unsigned int AxStream::ReadUInt32()
{
	unsigned int result;
	this->ReadData(&result, 4);
	return result;
}

void AxStream::WriteUInt32(unsigned int value)
{
	this->WriteData(&value, 4);
}

unsigned long long AxStream::ReadUInt64()
{
	unsigned long long result;
	this->ReadData(&result, 8);
	return result;
}

void AxStream::WriteUInt64(unsigned long long value)
{
	this->WriteData(&value, 8);
}


float AxStream::ReadFloat32()
{
	float result;
	this->ReadData(&result, 4);
	return result;
}

void AxStream::WriteFloat32(float value)
{
	this->WriteData(&value, 4);
}

double AxStream::ReadFloat64()
{
	double result;
	this->ReadData(&result, 8);
	return result;
}

void AxStream::WriteFloat64(double value)
{
	this->WriteData(&value, 8);
}


AxString AxStream::ReadString()
{
	int dataLength = this->Read7BitEncodedInt32();
	char *data = new char[dataLength];
	this->ReadData(data, dataLength);

	AxString result;
	AxString::DecodeUtf8String(result, data, dataLength);

	delete[] data;

	return AxString(result);
}

void AxStream::WriteString(AxString &value)
{
	int dataLength = AxString::GetStringAsUTF8Length(value);
	char *data = new char[dataLength];
	AxString::EncodeUtf8String(data, value);

	this->Write7BitEncodedInt32(dataLength);
	this->WriteData(data, dataLength);

	delete[] data;
}

AxString AxStream::ReadText()
{
	AxString result;
	while (this->position < this->length)
	{
		char c = this->ReadUInt8();
		if ((c != ' ') && (c != '\t') && (c != '\r') && (c != '\n'))
			result += c;
		else
			if (result.length > 0)
				break;
	}

	return AxString(result);
}

void AxStream::WriteText(AxString text)
{
	this->WriteData(text.GetCharContents(), text.length);
}

AxString AxStream::ReadUText()
{
	AxString result;
	while (this->position < this->length)
	{
		wchar_t c = this->ReadUInt16();
		if ((c != ' ') && (c != '\t') && (c != '\r') && (c != '\n'))
			result += c;
		else
			if (result.length > 0)
				break;
	}

	return AxString(result);
}

void AxStream::WriteUText(AxString text)
{
	this->WriteData(text.contents, text.length * 2);
}

AxString AxStream::ReadTextLine()
{
	AxString result;
	while (this->position < this->length)
	{
		char c = this->ReadUInt8();
		if ((c != '\r') && (c != '\n'))
			result += c;
		else
			if (result.length > 0)
				break;
	}

	return AxString(result);
}

void AxStream::WriteTextLine(AxString text)
{
	this->WriteData(text.GetCharContents(), text.length);
	this->WriteUInt8('\r');
	this->WriteUInt8('\n');
}

AxString AxStream::ReadUTextLine()
{
	AxString result;
	while (this->position < this->length)
	{
		wchar_t c = this->ReadUInt8();
		if ((c != '\r') && (c != '\n'))
			result += c;
		else
			if (result.length > 0)
				break;
	}

	return AxString(result);
}

void AxStream::WriteUTextLine(AxString text)
{
	this->WriteData(text.contents, text.length * 2);
	this->WriteUInt16('\r');
	this->WriteUInt16('\n');
}

AxString AxStream::ReadTextFull()
{
	int textLength = (int)(this->length - this->position);
	char *text = new char[textLength];
	this->ReadData(text, textLength);

	AxString result(text, textLength);
	delete[] text;

	return AxString(result);
}

AxString AxStream::ReadUTextFull()
{
	int textLength = (int)((this->length - this->position) / 2);

	AxString result;
	result.SetLength(textLength);
	this->ReadData(result.contents, textLength * 2);

	return AxString(result);
}
