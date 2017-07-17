#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxTcpIpConnection.h"

#include "..\..\Threading\AxThread.h"
#include "..\..\Threading\AxMutex.h"

class AXDLLCLASS AxTcpIpClient
{
private:
	static int ConnectionReceive(void *tcpIpClient);

	AxThread *thread;
	AxMutex *synchronization;

public:
	bool connected;
	AxTcpIpConnection *connection;

	AxTcpIpClient(void);
	~AxTcpIpClient(void);

	// TODO: Make these asynchronous, as well as Server's
	// Connects to a server. Returns true if succeeds or false otherwise
	bool Connect(AxString address, int port = 80);
	// Disconnects from a server
	void Disconnect();

	// Occurs when the client has successfully connected to a server
	virtual void OnConnected();

	// Occurs when the client has received data from the server
	virtual void OnReceive(void *data, int dataSize);
	// Occurs when the client has disconnected from the server
	virtual void OnDisconnected();
	// Occurs when there was an error with the connection
	virtual void OnError();
};

