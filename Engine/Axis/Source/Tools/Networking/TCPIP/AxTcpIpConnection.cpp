//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTcpIpConnection.h"

#include "..\..\AxMem.h"
#include "..\..\AxString.h"

#include <winsock2.h>
#include <ws2tcpip.h>

AxTcpIpConnection::AxTcpIpConnection(void)
{
	this->Initialize();

	*(SOCKET*)this->id = INVALID_SOCKET;
	this->opened = false;
}

AxTcpIpConnection::AxTcpIpConnection(void *socketId)
{
	this->Initialize();

	this->Open(socketId);
}


AxTcpIpConnection::~AxTcpIpConnection(void)
{
	this->Close();

	delete this->id;

	if (this->wsaStarted)
		WSACleanup();
}

void AxTcpIpConnection::Initialize()
{
	this->id = new SOCKET;

    // WSA Startup can be called numerous times in the app, as long as WSACleanup is called the same number of times
	WSADATA wsaData;
	int error = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (error != 0) 
	{
		this->wsaStarted = false;
		int errorCode = error;
		this->Close();
    }
	else
		this->wsaStarted = true;
}

bool AxTcpIpConnection::RetrieveAddresses()
{
	SOCKADDR_IN addressInfo;
	int addressInfoSize = sizeof(addressInfo);

	if (getpeername(*(SOCKET*)this->id, (sockaddr*)&addressInfo, &addressInfoSize) != 0)
	{
		int errorCode = WSAGetLastError();
		return false;
	}
	this->remoteAddress = inet_ntoa(addressInfo.sin_addr);
	this->remotePort = addressInfo.sin_port;

	if (getsockname(*(SOCKET*)this->id, (sockaddr*)&addressInfo, &addressInfoSize) != 0)
	{
		int errorCode = WSAGetLastError();
		return false;
	}

	this->localAddress = inet_ntoa(addressInfo.sin_addr);
	this->localPort = addressInfo.sin_port;

	return true;
}

bool AxTcpIpConnection::Open(AxString &address, int port)
{
	if (!this->wsaStarted)
		return false;

	int error, errorCode;

	// Resolve the server address and port
	addrinfo *result = NULL;

	addrinfo hints;
	AxMem::Zero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	error = getaddrinfo(address.GetCharContents(), AxString(port).GetCharContents(), &hints, &result);
    if (error != 0) 
	{
		errorCode = error;
		this->Close();
		return false;
    }

	// Attempt to connect to an address until one succeeds
    do
	{
        // Create a SOCKET for connecting to server
        *(SOCKET*)this->id = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (*(SOCKET*)this->id == INVALID_SOCKET) 
		{
            errorCode = WSAGetLastError();
			this->Close();
            return false;
        }

        // Attempt connection
        error = connect(*(SOCKET*)this->id, result->ai_addr, (int)result->ai_addrlen);
        if (error == SOCKET_ERROR) 
		{
            closesocket(*(SOCKET*)this->id);
            *(SOCKET*)this->id = INVALID_SOCKET;
        }
		else
			break;

		result = result->ai_next;
    } 
	while (result != 0);

	freeaddrinfo(result);

	if (*(SOCKET*)this->id == INVALID_SOCKET) 
	{
		this->Close();
        return false;
    }

	this->RetrieveAddresses();

	this->opened = true;

	return true;
}

bool AxTcpIpConnection::Open(void *socketId)
{
	*(SOCKET*)this->id = *(SOCKET*)socketId;
	this->opened = this->RetrieveAddresses();

	return this->opened;
}


bool AxTcpIpConnection::Close()
{
	if (!this->wsaStarted)
		return false;

	bool result = true;
    
	if (*(SOCKET*)this->id != INVALID_SOCKET)
	{
		if (this->opened)
		{
			int error = shutdown(*(SOCKET*)this->id, SD_SEND);
			if (error == SOCKET_ERROR)
			{
				int errorCode = WSAGetLastError();
				result = false;
			}
		}

		closesocket(*(SOCKET*)this->id);
		*(SOCKET*)this->id = INVALID_SOCKET;
	}

	this->opened = false;

	return result;
}

int AxTcpIpConnection::Receive(void *buffer, int bufferLength)
{
	if (!this->wsaStarted)
		return -1;
	
	
	int bytesRead = recv(*(SOCKET*)this->id, (char*)buffer, bufferLength, 0);

	if (bytesRead > 0)
	{
		return bytesRead;
	}

	else if (bytesRead == 0)
	{
		// Connection is closing
		this->Close();
		return 0;
	}

	else
	{
		// Error
		int errorCode = WSAGetLastError();
		this->Close();

		return -1;
	}
}

int AxTcpIpConnection::Send(const void *data, int dataSize)
{
	if (!this->wsaStarted)
		return -1;
	
	int bytesSent = send(*(SOCKET*)this->id, (char*)data, dataSize, 0 );
	if (bytesSent == SOCKET_ERROR) 
	{
		int errorCode = WSAGetLastError();
		this->Close();

		return -1;
	}

	return bytesSent;
}