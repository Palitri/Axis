#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\TCPIP\AxTcpIpConnection.h"

class AXDLLCLASS AxWebSocketsConnection :
	public AxTcpIpConnection
{
public:
	bool isWebSocket;

	AxWebSocketsConnection(void);
	virtual ~AxWebSocketsConnection(void);

	virtual int SendToWebSocket(const void *data, int dataSize);
};

