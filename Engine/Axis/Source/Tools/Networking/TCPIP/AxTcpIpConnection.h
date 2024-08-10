#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\AxString.h"

// AxTcpIpConnection and AxTcpIpListener could be unified into a TcpIpSocket class, as they both have similarities, but there are properies and functionalities that are specefic for one and meaningless to the other. 
// For example a listener doesn't have remote address and port, send or receive methods, while a connection can't have a method for accepting connections. As well as they are created differently.

// Represents a Tcp/Ip connection, a socket used to send and receive data. This class is used as a universal end point of a Tcp/Ip connection and provides the means to communicate remotely.
class AXDLLCLASS AxTcpIpConnection
{
private:
	void *id;
	bool wsaStarted;

	void Initialize();

	bool RetrieveAddresses();
	
public:
	bool opened;

	AxString remoteAddress, localAddress;
	int remotePort, localPort;

	// Creates AxTcpIpConnection instance ready to open a connection to a remote listener
	AxTcpIpConnection(void);
	// Creates AxTcpIpConnection instance from a given socket id, which is already opened, for example by a listener. Useful for server-side connections.
	AxTcpIpConnection(void *socketId);
	virtual ~AxTcpIpConnection(void);

	// Opens a connection to a listener at the given address and port. Useful for client-side connections
	bool Open(AxString &address, int port);
	// Opens a connection from a given socket id, which is already opened, for example by a listener. Useful for server-side connections.
	bool Open(void *socketId);
	// Closes the connection
	bool Close();

	// Returns the number of bytes that are avalable to be read by the Receive method or -1 if there was an error.
	int ReceivedSize();
	// Returns the number of bytes received if reception was successful, 0 if connection is closing and -1 if there was an error.
	int Receive(void *buffer, int bufferLength);
	// Returns the number of bytes sent, if sending was successful or -1 if there was an error
	virtual int Send(const void *data, int dataSize);
};

