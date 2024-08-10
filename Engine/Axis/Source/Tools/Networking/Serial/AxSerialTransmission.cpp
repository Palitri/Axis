#include "AxSerialTransmission.h"

#include "..\..\AxMath.h"
#include "..\..\AxMem.h"

const unsigned char AxSerialTransmission::chunkId[2] = { 0xE6, 0x71 };

AxSerialTransmission::AxSerialTransmission(AxString comPortName, int baudRate, int bufferSize)
	: AxSerialPort(comPortName, baudRate, bufferSize)
{
	this->readBufferPosition = 0;
	this->detectedFooterOffset = 0;
}


AxSerialTransmission::~AxSerialTransmission(void)
{
}

int AxSerialTransmission::SendChunk(void *data, int dataSize)
{
	unsigned char writeBuffer[AxSerialTransmission::chunkMaxTotalSize];

	int sendDataSize = AxMath::Min(dataSize, AxSerialTransmission::chunkMaxDataSize);

	AxMem::Copy(writeBuffer, data, sendDataSize);
	AxMem::Copy(writeBuffer + sendDataSize, AxSerialTransmission::chunkId, AxSerialTransmission::chunkIdSize);
	*((unsigned char*)(writeBuffer + sendDataSize + AxSerialTransmission::chunkIdSize)) = sendDataSize;
	*((unsigned short*)(writeBuffer + sendDataSize + AxSerialTransmission::chunkIdSize + 1)) = AxMath::CRC16(data, sendDataSize);

	return this->Send(writeBuffer, sendDataSize + AxSerialTransmission::chunkFooterSize);
}

void AxSerialTransmission::OnReceive(void *data, int dataSize)
{
	AxSerialPort::OnReceive(data, dataSize);

	for (int i = 0; i < dataSize; i++)
	{
		unsigned char dataByte = ((unsigned char*)data)[i];

		this->readBuffer[this->readBufferPosition] = dataByte;
		this->readBufferPosition++;

		if (this->detectedFooterOffset < AxSerialTransmission::chunkIdSize)
		{
			if (dataByte == AxSerialTransmission::chunkId[this->detectedFooterOffset])
				this->detectedFooterOffset++;
			else
				this->detectedFooterOffset = 0;
		}
		else if (this->detectedFooterOffset < AxSerialTransmission::chunkFooterSize)
		{
			this->chunkData[this->detectedFooterOffset - AxSerialTransmission::chunkIdSize] = dataByte;
			this->detectedFooterOffset++;

			if (this->detectedFooterOffset == AxSerialTransmission::chunkFooterSize)
			{
				this->detectedFooterOffset = 0;

				unsigned char payloadSize = this->chunkData[0];
				unsigned short payloadCrc16 = *(unsigned short*)(this->chunkData + 1);

				int payloadPosition = this->readBufferPosition - AxSerialTransmission::chunkFooterSize - payloadSize;
				if (payloadPosition >= 0)
				{
					unsigned short actualCrc16 = AxMath::CRC16(&this->readBuffer[payloadPosition], payloadSize);
					if (actualCrc16 == payloadCrc16)
					{
						this->OnReceiveChunk(&this->readBuffer[payloadPosition], payloadSize);

						this->readBufferPosition = 0;
					}
				}
				else
				{
					unsigned char chunkData[AxSerialTransmission::chunkMaxDataSize];
					AxMem::Copy(chunkData, this->readBuffer + AxSerialTransmission::chunkMaxTotalSize + payloadPosition, -payloadPosition);
					AxMem::Copy(chunkData - payloadPosition, this->readBuffer, payloadSize + payloadPosition);
					unsigned short actualCrc16 = AxMath::CRC16(chunkData, payloadSize);
					if (actualCrc16 == payloadCrc16)
					{
						this->OnReceiveChunk(chunkData, payloadSize);

						this->readBufferPosition = 0;
					}
				}
			}
		}

		 this->readBufferPosition %= AxSerialTransmission::chunkMaxTotalSize;
	}
}

void AxSerialTransmission::OnReceiveChunk(void *data, int dataSize)
{
}
