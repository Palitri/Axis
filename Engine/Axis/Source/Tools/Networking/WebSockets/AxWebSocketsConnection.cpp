#include "AxWebSocketsConnection.h"

#include "AxWebSocketUtils.h"

AxWebSocketsConnection::AxWebSocketsConnection(void)
	: AxTcpIpConnection()
{
	this->isWebSocket = false;
}


AxWebSocketsConnection::~AxWebSocketsConnection(void)
{
}

int AxWebSocketsConnection::SendToWebSocket(const void *data, int dataSize)
{
	void* dataEncoded;
	int dataEncodedSize = AxWebSocketUtils::EncodeWebSocketServerPacket(dataEncoded, data, dataSize);

	int result = AxTcpIpConnection::Send(dataEncoded, dataEncodedSize);

	delete[] dataEncoded;

	return result;
}
