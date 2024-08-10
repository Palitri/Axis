#include "AxWebSocketUtils.h"

#include "..\..\AxMath.h"
#include "..\..\AxMem.h"

const unsigned char AxWebSocketUtils::OpCode_ContinuationFrame = 0x0;
const unsigned char AxWebSocketUtils::OpCode_TextFrame = 0x1;
const unsigned char AxWebSocketUtils::OpCode_BinaryFrame = 0x2;
const unsigned char AxWebSocketUtils::OpCode_ConnectionClose = 0x8;
const unsigned char AxWebSocketUtils::OpCode_Ping1 = 0x9;
const unsigned char AxWebSocketUtils::OpCode_Ping2 = 0xA;

const unsigned char AxWebSocketUtils::FinalFragmentBit = 0x80;

const char *AxWebSocketUtils::httpEOL = "\r\n"; // HTTP/1.1 defines the end-of-line marker as the sequence CR LF
const char *AxWebSocketUtils::SecurityKeyResponseGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

AxWebSocketUtils::AxWebSocketUtils(void)
{
}


AxWebSocketUtils::~AxWebSocketUtils(void)
{
}


AxString AxWebSocketUtils::GenerateResponseSecurityKey(AxString requestSecurityKey)
{
	AxString responseKey = requestSecurityKey + AxWebSocketUtils::SecurityKeyResponseGuid;
	char responseSHA1[20];
	AxMath::SHA1(responseSHA1, responseKey.GetCharContents(), responseKey.length);

	return AxString::ToBase64(responseSHA1, 20);
}

bool AxWebSocketUtils::ParseHTTPHandshakeRequest(AxString httpRequest, AxString &requestSecurityKey)
{
    requestSecurityKey = "";
	if (httpRequest.IndexOf("GET") == -1)
		return false;

	int keyStart = httpRequest.IndexOf("Sec-WebSocket-Key:");
	if (keyStart == -1)
		return false;

	keyStart += 18;
	int keyEnd = httpRequest.IndexOf(AxWebSocketUtils::httpEOL, keyStart);
	if (keyEnd == -1)
		keyEnd = httpRequest.length;

	requestSecurityKey = httpRequest.SubString(keyStart, keyEnd - keyStart).Trim(' ');

	return !requestSecurityKey.IsEmpty();
}

AxString AxWebSocketUtils::GenerateHTTPHandshakeResponse(AxString requestSecurityKey)
{
    return AxString(AxString("HTTP/1.1 101 Switching Protocols") + AxWebSocketUtils::httpEOL +
        "Connection: Upgrade" + AxWebSocketUtils::httpEOL +
        "Upgrade: websocket" + AxWebSocketUtils::httpEOL +
        "Sec-WebSocket-Accept: " + AxWebSocketUtils::GenerateResponseSecurityKey(requestSecurityKey) + AxWebSocketUtils::httpEOL +
        AxWebSocketUtils::httpEOL);
}

bool AxWebSocketUtils::GenerateHTTPHandshakeResponse(AxString httpRequest, AxString &httpResponse)
{
    httpResponse = "";

    AxString requestSecurityKey;
    if (!AxWebSocketUtils::ParseHTTPHandshakeRequest(httpRequest, requestSecurityKey))
        return false;

    httpResponse = AxWebSocketUtils::GenerateHTTPHandshakeResponse(requestSecurityKey);
    return true;
}

int AxWebSocketUtils::DecodeWebSocketClientPacket(void *&result, const void *data, int dataLength)
{
    unsigned char* dataBytes = (unsigned char*)data;
	bool isFullMessage = (dataBytes[0] & 0x80) != 0;
    unsigned char opCode = dataBytes[0] & 0x0F;

    if (opCode != AxWebSocketUtils::OpCode_BinaryFrame)
        return 0;

    bool isMasked = (dataBytes[1] & 0x80) != 0;
    long long length = dataBytes[1] & 0x7F;
    int offset = 2;
    int lengthBytes = length == 126 ? 2 : length == 127 ? 8 : 0;
    if (lengthBytes != 0)
    {
        length = 0;
        for (int i = 0; i < 2; i++)
            length = (length << 8) | dataBytes[offset++];
    }

	unsigned char *resultBytes = new unsigned char[length];

    if (isMasked)
    {
        int maskOffset = offset;
        offset += 4;

        for (int i = 0; i < length; i++)
            resultBytes[i] = dataBytes[offset++] ^ dataBytes[maskOffset + i % 4];
    }
    else
    {
        for (int i = 0; i < length; i++)
            resultBytes[i] = dataBytes[offset++];
    }

	result = resultBytes;
    return length;
}

int AxWebSocketUtils::EncodeWebSocketServerPacket(void *&result, const void *data, int dataLength)
{
    int length = dataLength + 2;
	unsigned char *resultBytes = new unsigned char[length];
    resultBytes[0] = AxWebSocketUtils::FinalFragmentBit | AxWebSocketUtils::OpCode_BinaryFrame;
	resultBytes[1] = dataLength;
	AxMem::Copy(resultBytes + 2, data, dataLength);

	result = resultBytes;
    return length;
}