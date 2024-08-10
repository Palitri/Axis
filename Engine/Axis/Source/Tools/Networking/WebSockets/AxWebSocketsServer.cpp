#include "AxWebSocketsServer.h"

#include "AxWebSocketUtils.h"


AxWebSocketsServer::AxWebSocketsServer(void)
{
	AxWebServer();
}


AxWebSocketsServer::~AxWebSocketsServer(void)
{
}


AxTcpIpConnection *AxWebSocketsServer::CreateConnection()
{
	return new AxWebSocketsConnection();
}

void AxWebSocketsServer::OnConnectionReceive(AxTcpIpConnection *connection, void *data, int dataSize)
{
	AxWebSocketsConnection *webSocketConnection = (AxWebSocketsConnection*)connection;

	if (webSocketConnection->isWebSocket)
	{
		void *decodedBuffer;
		int decodedBufferSize = AxWebSocketUtils::DecodeWebSocketClientPacket(decodedBuffer, data, dataSize);
		if (decodedBufferSize != 0)
			this->OnWebSocketReceive(webSocketConnection, decodedBuffer, decodedBufferSize);
		else
		{
			this->OnWebSocketClose(webSocketConnection);

			connection->Close();
		}

		//delete[] decodedBuffer; // TODO: FIX THIS!!! : AxWebSocketsExchangeChannel.ProcessData() frees the buffer via AxMemoryStream.
	}
	else
	{
		AxString httpRequest((char*)data, dataSize);
		AxString httpResponse;
		if (AxWebSocketUtils::GenerateHTTPHandshakeResponse(httpRequest, httpResponse))
		{
			
			connection->Send(httpResponse.GetCharContents(), httpResponse.length);
			webSocketConnection->isWebSocket = true;
			this->OnWebSocketCreate(webSocketConnection);
		}
		else
			AxWebServer::OnConnectionReceive(connection, data, dataSize);
	}
}

void AxWebSocketsServer::OnWebSocketCreate(AxWebSocketsConnection *webSocketConnection)
{
}

void AxWebSocketsServer::OnWebSocketClose(AxWebSocketsConnection *webSocketConnection)
{
}

void AxWebSocketsServer::OnWebSocketReceive(AxWebSocketsConnection *webSocketConnection, void *data, int dataSize)
{
}
