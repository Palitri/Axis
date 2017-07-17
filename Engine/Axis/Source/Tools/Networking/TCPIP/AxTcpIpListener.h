#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxTcpIpConnection.h"

// Serves as a means to enable listening for incoming Tcp/Ip connections. 
// When the listener is opened, incoming connections can be accepted via the AcceptConnection method, which creates and returns an instance of AxTcpIpConnection which can be used to communicate with the remote client who requested the connection.
class AXDLLCLASS AxTcpIpListener
{
private:
    void *listeningSocket;
	bool wsaStarted;

	void Initialize();

public:
	bool opened;

	AxString address;
	int port;

	// Creates an unopened instance
	AxTcpIpListener(void);
	// Creates an instance and opens it on the specified port, enabling it to accept connections
	AxTcpIpListener(int port);
	~AxTcpIpListener(void);

	// Opens the listener on the specified port, enabling it to accept connections
	bool Open(int port);
	// Closes the listener, disabling it to accept connections
	void Close();

	// Waits until there is an incoming connection and returns a new instance of AxTcpIpConnection as a means to communicate with the remote client who requested the connection
	AxTcpIpConnection *AcceptConnection();
};
