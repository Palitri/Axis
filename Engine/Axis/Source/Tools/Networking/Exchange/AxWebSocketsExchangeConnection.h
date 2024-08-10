#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\AxString.h"

#include "..\WebSockets\AxWebSocketsServer.h"

class AxWebSocketsExchangeServer;
class AxWebSocketsExchangeChannel;

class AXDLLCLASS AxWebSocketsExchangeConnection :
	public AxWebSocketsConnection
{
public:
	static const int OpCode_Main = 0x41584350;    //AXCP
	static const unsigned char OpCode_SetChannel = 0x10;
	static const unsigned char OpCode_ClientId = 0x20;
	static const unsigned char OpCode_Data = 0x30;
	static const unsigned char OpCode_PingBack = 0x40;

	AxWebSocketsExchangeServer *server;
	AxWebSocketsExchangeChannel *channel;
	AxString id;

	AxWebSocketsExchangeConnection(AxWebSocketsExchangeServer *server);
	virtual ~AxWebSocketsExchangeConnection(void);

	void ProcessData(void *data, int dataSize);
	//void SetChannel(AxWebSocketsExchangeChannel *channel);
	//void Close();
};

