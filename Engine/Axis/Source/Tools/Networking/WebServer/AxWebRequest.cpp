//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWebRequest.h"

#include "AxWebServer.h"
#include "..\TCPIP\AxTcpIpConnection.h"
#include "..\..\AxStrings.h"

AxWebRequest::AxWebRequest(AxWebServer *server, AxTcpIpConnection *connection, AxString requestString)
{
	// TODO: Parsing of request

	this->server = server;
	this->connection = connection;

	this->fullRequest = requestString;

	AxStrings requestLines;
	requestLines.SplitString(requestString, "\r\n");

	AxList<AxStrings*> tokens;
	AxStrings linesHeaders;
	for (int lineIndex = 0; lineIndex < requestLines.count; lineIndex++)
	{
		AxStrings *lineTokens = new AxStrings();
		lineTokens->SplitString(requestLines[lineIndex], " ");
		linesHeaders.Add((*lineTokens)[0].ToLower());
		tokens.Add(lineTokens);
	}

	
	this->methodName = (*tokens[0])[0];

	if (this->methodName.Equals("GET") || this->methodName.Equals("POST"))
	{
		this->supported = true;

		this->originalPath = (*tokens[0])[1].Trim('/', false, true).Replace("/", "\\").Replace("%20", " ").Remove("\\..");

		int hostNameIndex = linesHeaders.IndexOf(AxString("host:"), StringComparison_CaseInsensitive);
		if (hostNameIndex != -1)
			this->hostName = (*tokens[hostNameIndex])[1];

		int authorizationTypeIndex = linesHeaders.IndexOf(AxString("authorization:"), StringComparison_CaseInsensitive);
		if (authorizationTypeIndex != -1)
		{
			this->authenticationTypeName = (*tokens[authorizationTypeIndex])[1];
			AxStrings userAndPass;
			userAndPass.SplitString((*tokens[authorizationTypeIndex])[2].FromBase64(), AxString(":"));
			this->authenticationUserName = userAndPass[0];
			this->authenticationUserPassword = userAndPass[1];
		}

		this->sessionId = -1;
		this->receivedSessionId = false;
		int cookiesLineIndex = linesHeaders.IndexOf(AxString("cookie:"), StringComparison_CaseInsensitive);
		if (cookiesLineIndex != -1)
		{
			for (int cookieIndex = 1; cookieIndex < (*tokens[cookiesLineIndex]).count; cookieIndex++)
			{
				AxStrings cookie;
				cookie.SplitString((*tokens[cookiesLineIndex])[cookieIndex].Trim(';'), '=');
				if (cookie.count == 2)
				{
					if (cookie[0].Trim(' ').Equals("axsession"))
					{
						 cookie[1].Trim(' ').ToUInt(this->sessionId);
						 this->receivedSessionId = true;
					}
				}
			}
		}
	}
	else
		this->supported = false;

	for (int lineIndex = 0; lineIndex < tokens.count; lineIndex++)
		delete tokens[lineIndex];
}


AxWebRequest::~AxWebRequest(void)
{
}
