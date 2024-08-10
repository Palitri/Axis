#pragma once

#include "..\..\..\AxGlobals.h"

#include  "..\..\AxString.h"

class AXDLLCLASS AxWebSocketUtils
{
private:
	static const unsigned char OpCode_ContinuationFrame;
	static const unsigned char OpCode_TextFrame;
	static const unsigned char OpCode_BinaryFrame;
	static const unsigned char OpCode_ConnectionClose;
	static const unsigned char OpCode_Ping1;
	static const unsigned char OpCode_Ping2;

	static const unsigned char FinalFragmentBit;

	static const char *httpEOL;
	static const char *SecurityKeyResponseGuid;

public:
	AxWebSocketUtils(void);
	virtual ~AxWebSocketUtils(void);

	static AxString GenerateResponseSecurityKey(AxString requestSecurityKey);
	static bool ParseHTTPHandshakeRequest(AxString httpRequest, AxString &requestSecurityKey);
	static AxString GenerateHTTPHandshakeResponse(AxString requestSecurityKey);
	static bool GenerateHTTPHandshakeResponse(AxString httpRequest, AxString &httpResponse);
	static int DecodeWebSocketClientPacket(void *&result, const void *data, int dataLength);
	static int EncodeWebSocketServerPacket(void *&result, const void *data, int dataLength);
};
