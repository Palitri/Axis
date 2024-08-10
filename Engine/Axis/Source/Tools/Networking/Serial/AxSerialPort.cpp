#include "AxSerialPort.h"

#include "..\..\Platform\AxPlatformUtils.h"

int AxSerialPort::ReadPortAsync(void *serialPort)
{
	AxSerialPort *port = (AxSerialPort*)serialPort;

	while (port->readThread->running)
		port->ReadPort();

	return 0;
}

AxSerialPort::AxSerialPort(AxString comPortName, int baudRate, int bufferSize)
{
	this->comPortHandle = AxPlatformUtils::OpenComPort(comPortName, baudRate);

	this->readMutex = new AxMutex();
	this->readThread = new AxThread(AxSerialPort::ReadPortAsync, this);
}


AxSerialPort::~AxSerialPort(void)
{
	this->readThread->Finish();

	AxPlatformUtils::CloseComPort(&this->comPortHandle);

	delete this->readThread;
	delete this->readMutex;
}

int AxSerialPort::Send(void *data, int dataSize)
{
	this->readMutex->Lock();

	int bytesSent = AxPlatformUtils::WriteComPort(this->comPortHandle, data, dataSize);

	this->readMutex->Unlock();

	return bytesSent;
};


void AxSerialPort::ReadPort()
{
	const int comBufferSize = 1;
	unsigned char comBuffer[comBufferSize];
	int bytesRead = AxPlatformUtils::ReadComPort(this->comPortHandle, comBuffer, comBufferSize);
	if (bytesRead > 0)
	{
		this->readMutex->Lock();
			
		this->OnReceive(comBuffer, bytesRead);
			
		this->readMutex->Unlock();
	}
};

void AxSerialPort::OnReceive(void *data, int dataSize)
{
}

void AxSerialPort::OnError()
{
}
