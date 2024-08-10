#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxSerialPort.h"

class AXDLLCLASS AxSerialTransmission :
	public AxSerialPort
{
private:
	static const int chunkIdSize = 2;
	static const int chunkFooterSize = 5;
	static const int chunkMaxDataSize = 255;
	static const int chunkMaxTotalSize = chunkMaxDataSize + chunkFooterSize;
	static const unsigned char chunkId[chunkIdSize];

	int readBufferPosition, detectedFooterOffset;
	unsigned char readBuffer[chunkMaxTotalSize];
	unsigned char chunkData[chunkFooterSize - chunkIdSize];

public:
	AxSerialTransmission(AxString comPortName, int baudRate, int bufferSize = 65536);
	virtual ~AxSerialTransmission(void);

	virtual int SendChunk(void *data, int dataSize);

	virtual void OnReceive(void *data, int dataSize);

	virtual void OnReceiveChunk(void *data, int dataSize);
};

