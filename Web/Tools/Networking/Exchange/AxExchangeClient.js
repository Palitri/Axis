var ExchangeProtocol = Object.freeze(
{
    Main : 0x41584350,    //AXCP
    SetChannel : 0x10,
    ClientId : 0x20,
    Data : 0x30,
    PingBack : 0x40
});

function AxExchangeClient(url)
{
    this.webSocket = null;
    this.connected = false;
    
    this.canSend = false;
    
    this.outputStream = new AxMemoryStream();
    
    if (!AxUtils.IsUndefinedOrNull(url))
        this.Connect(url);
}

AxExchangeClient.IsSupported = function()
{
    return AxWebSocketClient.IsSupported();
};


AxExchangeClient.prototype.Connect = function(url)
{
    if (!AxExchangeClient.IsSupported())
        return false;
    
    this.webSocket = new AxWebSocketClient(url);
    
    var instance = this;
    
    this.webSocket.OnConnected = function()
    {
        instance.OnConnected();
        
        instance.canSend = true;
        instance.OnCanSend();
    };

    this.webSocket.OnReceive = function(data)
    {
        var dataStream = new AxMemoryStream(data, data.byteLength);
        
        var chunkId = dataStream.ReadUInt8();
        
        switch (chunkId)
        {
            case ExchangeProtocol.PingBack:
            {
                var pingBackChunk = dataStream.ReadUInt8();

                instance.canSend = true;
                instance.OnCanSend();

                break;
            }
            
            case ExchangeProtocol.Data:
            {
                var sender = null;
                var dataSize = dataStream.ReadUInt32();
                var receivedDataStream = new AxMemoryStream(dataSize);
                receivedDataStream.WriteStreamData(dataStream, dataSize);
                receivedDataStream.Seek(0);
                
                instance.OnReceive(sender, receivedDataStream);
                
                break;
            }
        }
    };

    this.webSocket.OnDisconnected = function()
    {
        instance.OnDisconnected();
    };
    
    this.webSocket.OnError = function()
    {
        instance.OnError();
    };
    
    return true;
};

AxExchangeClient.prototype.Send = function()
{
    if (!this.canSend)
        return false;
    
    this.canSend = false;

    this.outputStream.Seek(0);

    var buffer = new AxMemoryStream();
    
    buffer.WriteUInt8(ExchangeProtocol.Data);
    buffer.WriteUInt32(this.outputStream.length);
    buffer.WriteStreamData(this.outputStream, this.outputStream.length);

    var bufferArray = buffer.data.slice(0, buffer.length);
    this.webSocket.Send(bufferArray);
    
    this.outputStream.Seek(0);
    this.outputStream.length = 0;
    
    return true;
};

AxExchangeClient.prototype.SetChannel = function(channelId)
{
    var buffer = new AxMemoryStream();
    
    buffer.WriteUInt8(ExchangeProtocol.SetChannel);
    buffer.WriteString(channelId);
    
    var bufferArray = buffer.data.slice(0, buffer.length);
    this.webSocket.Send(bufferArray);
};

AxExchangeClient.prototype.OnConnected = function() { };
AxExchangeClient.prototype.OnReceive = function(sender, data) { };
AxExchangeClient.prototype.OnDisconnected = function() { };
AxExchangeClient.prototype.OnError = function() { };
AxExchangeClient.prototype.OnCanSend = function() { };