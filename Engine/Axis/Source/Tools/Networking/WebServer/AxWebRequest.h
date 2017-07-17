#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\AxString.h"

class AxWebServer;
class AxTcpIpConnection;

class AXDLLCLASS AxWebRequest
{
public:
	bool supported;
	AxWebServer *server;
	AxTcpIpConnection *connection;
	AxString fullRequest, path, originalPath, methodName, hostName, 
		authenticationTypeName, authenticationUserName, authenticationUserPassword;
	bool receivedSessionId;
	unsigned int sessionId;
	void *session;

	AxWebRequest(AxWebServer *server, AxTcpIpConnection *connection, AxString requestString);
	~AxWebRequest(void);
};

