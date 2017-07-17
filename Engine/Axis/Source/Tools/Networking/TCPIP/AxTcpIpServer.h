#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxTcpIpListener.h"
#include "AxTcpIpConnection.h"

#include "..\..\AxList.h"
#include "..\..\Threading\AxThread.h"
#include "..\..\Threading\AxMutex.h"
#include "..\..\Threading\AxThreadsPool.h"

class AxTcpIpServer;
class AXDLLCLASS AxTcpIpServerConnection
{
public:
	AxTcpIpServer *server;
	AxTcpIpConnection *connection;

	AxTcpIpServerConnection(AxTcpIpServer *server, AxTcpIpConnection *connection);
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxTcpIpConnection*>;

// TODO: Add functionality to ThreadPool (or create separate class) where the number of threads can grow dynamically. This is required to make more, unfixed number of maximum simultaneous connections to the server

class AXDLLCLASS AxTcpIpServer
{
private:
	AxThreadsPool *threadsPool;
	AxMutex *synchronization;

	bool opened;

	static int Listen(void *tcpIpServer);
	static int ConnectionReceive(void *tcpIpConnection);

public:
	AxTcpIpListener *listener;
	AxList<AxTcpIpConnection*> connections;

	bool synchronized;

	AxTcpIpServer(void);
	virtual ~AxTcpIpServer(void);

	// TODO: Make these asynchronous, as well as Client's
	bool Open(int port = 80);
	void Close();

	virtual bool OnAcceptConnection(AxTcpIpConnection *connection);

	virtual void OnConnectionReceive(AxTcpIpConnection *connection, void *data, int dataSize);
	virtual void OnConnectionClose(AxTcpIpConnection *connection);
	virtual void OnConnectionError(AxTcpIpConnection *connection);
};

