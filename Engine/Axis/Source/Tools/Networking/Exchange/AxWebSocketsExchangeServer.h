#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\WebSockets\AxWebSocketsServer.h"

#include "AxWebSocketsExchangeChannel.h"
#include "AxWebSocketsExchangeConnection.h"

AXDLLTEMPLATEMEMBER AxDictionary<AxString, AxWebSocketsExchangeChannel*>;

class AXDLLCLASS AxWebSocketsExchangeServer :
	public AxWebSocketsServer
{
public:
	AxDictionary<AxString, AxWebSocketsExchangeChannel*> channels;

	AxWebSocketsExchangeServer(void);
	virtual ~AxWebSocketsExchangeServer(void);

	virtual AxTcpIpConnection *CreateConnection();

	virtual void OnWebSocketCreate(AxWebSocketsConnection *webSocketConnection);
	virtual void OnWebSocketClose(AxWebSocketsConnection *webSocketConnection);
	virtual void OnWebSocketReceive(AxWebSocketsConnection *webSocketConnection, void *data, int dataSize);

	AxWebSocketsExchangeChannel *ProvideChannel(AxString channelId);
	void SetConnectionChannel(AxWebSocketsExchangeConnection *connection, AxWebSocketsExchangeChannel *channel);

};

