#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\AxString.h"

#include "..\..\Threading\AxThread.h"
#include "..\..\Threading\AxMutex.h"

class AXDLLCLASS AxSerialPort
{
private:
	static int ReadPortAsync(void *serialPort);

	void* comPortHandle;

	AxThread *readThread;
	AxMutex *readMutex;

	void ReadPort();

public:
	AxSerialPort(AxString comPortName, int baudRate, int bufferSize = 65536);
	virtual ~AxSerialPort(void);

	virtual int Send(void *data, int dataSize);

	// Occurs when the client has received data from the serial port
	virtual void OnReceive(void *data, int dataSize);
	// Occurs when there was an error with the port
	virtual void OnError();
};

