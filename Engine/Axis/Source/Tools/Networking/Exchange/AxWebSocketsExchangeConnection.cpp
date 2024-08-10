#include "AxWebSocketsExchangeConnection.h"

#include "AxWebSocketsExchangeServer.h"
#include "AxWebSocketsExchangeChannel.h"

AxWebSocketsExchangeConnection::AxWebSocketsExchangeConnection(AxWebSocketsExchangeServer *server)
{
	this->server = server;
	
	this->channel = 0;
}


AxWebSocketsExchangeConnection::~AxWebSocketsExchangeConnection(void)
{
}

void AxWebSocketsExchangeConnection::ProcessData(void *data, int dataSize)
{
	AxMemoryStream *inputStream = new AxMemoryStream(data, dataSize);

	AxMemoryStream *outputStream = new AxMemoryStream();

	while (inputStream->position < inputStream->length - 4)
    {
		unsigned char chunkId = inputStream->ReadUInt8();
        switch (chunkId)
        {
            case AxWebSocketsExchangeConnection::OpCode_SetChannel:
            {
				AxString channelId = inputStream->ReadString();

				AxWebSocketsExchangeChannel *channel = this->server->ProvideChannel(channelId);
				this->server->SetConnectionChannel(this, channel);

                break;
            }

            case AxWebSocketsExchangeConnection::OpCode_ClientId:
            {
				this->id = inputStream->ReadString();

                break;
            }

            case AxWebSocketsExchangeConnection::OpCode_Data:
            {
                int length = inputStream->ReadInt32();
				char *clientData = new char[length];
				inputStream->ReadData(clientData, length);

                if (this->channel != 0)
                {
					for (int i = 0; i < this->channel->count; i++)
                    {
						AxWebSocketsExchangeConnection *connection = this->channel->Get(i);
                        if (connection != this)
                        {
							AxMemoryStream *outputStream = new AxMemoryStream();
							
							outputStream->WriteUInt8(AxWebSocketsExchangeConnection::OpCode_Data);
							outputStream->WriteUInt32(length);
							outputStream->WriteData(clientData, length);
							
							connection->SendToWebSocket(outputStream->data, outputStream->length);

							delete outputStream;
                        }
                    }
                }

				delete[] clientData;

                break;
            }
        }

		outputStream->WriteUInt8(AxWebSocketsExchangeConnection::OpCode_PingBack);
		outputStream->WriteUInt8(chunkId);
	}

	if (outputStream->length > 0)
		this->SendToWebSocket(outputStream->data, outputStream->length);

	delete outputStream;

	delete inputStream;
}

//void AxWebSocketsExchangeConnection::SetChannel(AxWebSocketChannel *channel)
//{
//    if (this->channel != 0)
//    {
//		this->channel->Remove(this->connection);
//        if (this->channel->count == 0)
//			this->server->channels->Remove(channel->id);
//    }
//
//    this->channel = channel;
//
//    if (this->channel != 0)
//		this->channel->Add(this->connection);
//}
//
//void AxWebSocketsExchangeConnection::Close()
//{
//    this->SetChannel(0);
//}
