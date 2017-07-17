//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTcpIpClient.h"

int AxTcpIpClient::ConnectionReceive(void *tcpIpClient)
{
	AxTcpIpClient *client = (AxTcpIpClient*)tcpIpClient;

	const int bufferSize = 4096;
	char buffer[bufferSize];
	bool active = client->connection->opened;
	while (active)
	{
		int bytesReceived = client->connection->Receive(buffer, bufferSize);

		client->synchronization->Lock();

		if (bytesReceived > 0)
		{
			client->OnReceive(buffer, bytesReceived);
		}

		else if (bytesReceived == 0)
		{
			// Connection is closing. OnConnectionClose will be called after this loop
			active = false;
		}

		else
		{
			client->OnError();
			active = false;
		}

		client->synchronization->Unlock();
	}

	client->synchronization->Lock();

	client->OnDisconnected();

	client->synchronization->Unlock();

	return 0;
}

AxTcpIpClient::AxTcpIpClient(void)
{
	this->connected = false;
}


AxTcpIpClient::~AxTcpIpClient(void)
{
	this->Disconnect();
}

bool AxTcpIpClient::Connect(AxString address, int port)
{
	this->connection = new AxTcpIpConnection();
	this->connected = this->connection->Open(address, port);

	this->synchronization = new AxMutex();
	this->thread = new AxThread(AxTcpIpClient::ConnectionReceive, this);

	return this->connected;
}

void AxTcpIpClient::Disconnect()
{
	if (this->connected)
	{
		this->connection->Close();

		this->thread->Wait();

		delete this->thread;
		delete this->synchronization;

		delete this->connection;

		this->connected = false;
	}
}

void AxTcpIpClient::OnConnected()
{
}

void AxTcpIpClient::OnReceive(void *data, int dataSize)
{
}

void AxTcpIpClient::OnDisconnected()
{
}

void AxTcpIpClient::OnError()
{
}
