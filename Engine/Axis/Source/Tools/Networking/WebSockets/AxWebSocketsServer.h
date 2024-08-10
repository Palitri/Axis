#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\AxList.h"
#include "..\..\AxDictionary.h"

#include "..\WebServer\AxWebServer.h"
#include "AxWebSocketsConnection.h"

class AXDLLCLASS AxWebSocketsServer :
	public AxWebServer
{
public:
	AxWebSocketsServer(void);
	virtual ~AxWebSocketsServer(void);

	virtual AxTcpIpConnection *CreateConnection();

	virtual void OnConnectionReceive(AxTcpIpConnection *connection, void *data, int dataSize);
	
	virtual void OnWebSocketCreate(AxWebSocketsConnection *webSocketConnection);
	virtual void OnWebSocketClose(AxWebSocketsConnection *webSocketConnection);
	virtual void OnWebSocketReceive(AxWebSocketsConnection *webSocketConnection, void *data, int dataSize);
};

