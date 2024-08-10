#include "AxWebSocketsExchangeServer.h"

#include <iostream>
#include <ctime>

AxWebSocketsExchangeServer::AxWebSocketsExchangeServer(void)
	: AxWebSocketsServer()
{
}


AxWebSocketsExchangeServer::~AxWebSocketsExchangeServer(void)
{
}

AxTcpIpConnection *AxWebSocketsExchangeServer::CreateConnection()
{
	return new AxWebSocketsExchangeConnection(this);
}

void AxWebSocketsExchangeServer::OnWebSocketCreate(AxWebSocketsConnection *webSocketConnection)
{
	std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
	std::cout << "[" <<now ->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "]\t" << webSocketConnection->remoteAddress.GetCharContents() <<  "\tconnected\r\n";
}

void AxWebSocketsExchangeServer::OnWebSocketClose(AxWebSocketsConnection *webSocketConnection)
{
	AxWebSocketsExchangeConnection *exchangeConnection = (AxWebSocketsExchangeConnection*)webSocketConnection;
	this->SetConnectionChannel(exchangeConnection, 0);
	this->connections.Remove(webSocketConnection);

	std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
	std::cout << "[" <<now ->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "]\t" << webSocketConnection->remoteAddress.GetCharContents() <<  "\tdisconnected\r\n";
}

void AxWebSocketsExchangeServer::OnWebSocketReceive(AxWebSocketsConnection *webSocketConnection, void *data, int dataSize)
{
	AxWebSocketsExchangeConnection *exchangeConnection = (AxWebSocketsExchangeConnection*)webSocketConnection;
	exchangeConnection->ProcessData(data, dataSize);
}

AxWebSocketsExchangeChannel *AxWebSocketsExchangeServer::ProvideChannel(AxString channelId)
{
	AxWebSocketsExchangeChannel *channel;

    if (!this->channels.GetValue(channelId, channel))
    {
        channel = new AxWebSocketsExchangeChannel();
        channel->id = channelId;

        this->channels.Add(channelId, channel);
    }

	return channel;
}

void AxWebSocketsExchangeServer::SetConnectionChannel(AxWebSocketsExchangeConnection *connection, AxWebSocketsExchangeChannel *channel)
{
    if (connection->channel != 0)
    {
		connection->channel->Remove(connection);
        if (connection->channel->count == 0)
			this->channels.Remove(connection->channel->id);
    }

    connection->channel = channel;

    if (connection->channel != 0)
		connection->channel->Add(connection);
}