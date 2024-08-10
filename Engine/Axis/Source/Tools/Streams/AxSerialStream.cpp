#include "AxSerialStream.h"

#include "..\Platform\AxPlatformUtils.h"

int AxSerialStream::ReadPortAsync(void *serialStream)
{
	AxSerialStream *stream = (AxSerialStream*)serialStream;

	while (stream->readThread->running)
		stream->ReadPort();

	return 0;
}

AxSerialStream::AxSerialStream(AxString comPortName, int baudRate, int bufferSize)
	: AxStream()
{
	this->comPortHandle = AxPlatformUtils::OpenComPort(comPortName, baudRate);

	this->buffer = new AxRotaryBufferStream(bufferSize, AxRotaryBufferStreamWriteMode_SeparatePosition);

	this->readMutex = new AxMutex();
	this->readThread = new AxThread(AxSerialStream::ReadPortAsync, this);
};

AxSerialStream::~AxSerialStream(void)
{
	this->readThread->Finish();

	AxPlatformUtils::CloseComPort(&this->comPortHandle);

	delete this->readThread;
	delete this->readMutex;
	delete this->buffer;
};

long long AxSerialStream::ReadData(void *data, long long size)
{
	this->readMutex->Lock();

	long long result = this->buffer->ReadData(data, size);;
	this->position = this->buffer->position;

	this->readMutex->Unlock();
	
	return result;
};

long long AxSerialStream::WriteData(const void *data, long long size)
{
	return AxPlatformUtils::WriteComPort(this->comPortHandle, (void*)data, size);
};

void AxSerialStream::ReadPort()
{
	const int comBufferSize = 1;
	unsigned char comBuffer[comBufferSize];
	int bytesRead = AxPlatformUtils::ReadComPort(this->comPortHandle, comBuffer, comBufferSize);
	if (bytesRead > 0)
	{
		this->readMutex->Lock();
			
		this->buffer->WriteData(comBuffer, bytesRead);
		this->length = this->buffer->length;
			
		this->readMutex->Unlock();
	}
};
