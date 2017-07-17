//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTcpIpServer.h"

AxTcpIpServerConnection::AxTcpIpServerConnection(AxTcpIpServer *server, AxTcpIpConnection *connection)
{
	this->server = server;
	this->connection = connection;
}

int AxTcpIpServer::Listen(void *tcpIpServer)
{
	AxTcpIpServer *server = (AxTcpIpServer*)tcpIpServer;

	while (server->opened)
	{
		AxTcpIpConnection *connection = server->listener->AcceptConnection();

		if (connection == 0)
			return 0;

		bool synchronized = server->synchronized;
		if (synchronized)
			server->synchronization->Lock();

		if (server->OnAcceptConnection(connection))
		{
			server->connections.Add(connection);
			server->threadsPool->QueueTask(AxTcpIpServer::ConnectionReceive, new AxTcpIpServerConnection(server, connection));
		}
		else
		{
			delete connection;
		}

		if (synchronized)
			server->synchronization->Unlock();
	}


	return 0;
}

int AxTcpIpServer::ConnectionReceive(void *tcpIpServerConnection)
{
	AxTcpIpServerConnection *serverConnection = (AxTcpIpServerConnection*)tcpIpServerConnection;

	const int bufferSize = 4096;
	char buffer[bufferSize];
	bool active = serverConnection->connection->opened;
	while (active)
	{
		int bytesReceived = serverConnection->connection->Receive(buffer, bufferSize);

		bool synchronized = serverConnection->server->synchronized;
		if (synchronized)
			serverConnection->server->synchronization->Lock();

		if (bytesReceived > 0)
		{
			serverConnection->server->OnConnectionReceive(serverConnection->connection, buffer, bytesReceived);
		}

		else if (bytesReceived == 0)
		{
			// Connection is closing. OnConnectionClose will be called after this loop
			active = false;
		}

		else
		{
			serverConnection->server->OnConnectionError(serverConnection->connection);
			active = false;
		}

		if (synchronized)
			serverConnection->server->synchronization->Unlock();
	}

	bool synchronized = serverConnection->server->synchronized;
	if (synchronized)
		serverConnection->server->synchronization->Lock();

	serverConnection->server->OnConnectionClose(serverConnection->connection);

	serverConnection->server->connections.Remove(serverConnection->connection);
	serverConnection->connection->Close();
	delete serverConnection->connection;

	if (synchronized)
		serverConnection->server->synchronization->Unlock();

	delete serverConnection;


	return 0;
}

AxTcpIpServer::AxTcpIpServer(void)
{
	this->synchronized = false;

	this->synchronization = 0;
	this->threadsPool = 0;
	
	this->listener = 0;

	this->opened = false;
}

AxTcpIpServer::~AxTcpIpServer(void)
{
	this->Close();
}

bool AxTcpIpServer::Open(int port)
{
	this->synchronization = new AxMutex();
	this->threadsPool = new AxThreadsPool(4);
	
	this->listener = new AxTcpIpListener();
	this->opened = this->listener->Open(port);

	this->threadsPool->QueueTask(AxTcpIpServer::Listen, this);

	return this->opened;
}

void AxTcpIpServer::Close()
{
	if (this->opened)
	{
		// Close all active connections
		for (int i = 0; i < this->connections.count; i++)
			this->connections[i]->Close();

		// Close listener
		this->listener->Close();

		// Wait for all to finalize
		this->threadsPool->WaitAll();
		
		delete this->listener;
		delete this->threadsPool;
		delete this->synchronization;

		this->opened = false;
	}
}

bool AxTcpIpServer::OnAcceptConnection(AxTcpIpConnection *connection)
{
	return true;
}

void AxTcpIpServer::OnConnectionReceive(AxTcpIpConnection *connection, void *data, int dataSize)
{
}

void AxTcpIpServer::OnConnectionClose(AxTcpIpConnection *connection)
{
}

void AxTcpIpServer::OnConnectionError(AxTcpIpConnection *connection)
{
}
