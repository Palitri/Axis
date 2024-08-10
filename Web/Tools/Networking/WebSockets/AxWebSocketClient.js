function AxWebSocketClient(url)
{
    this.socket = null;
    this.connected = false;
    
    if (!AxUtils.IsUndefinedOrNull(url))
        this.Connect(url);
}


AxWebSocketClient.prototype.Connect = function(url)
{
    if (!AxWebSocketClient.IsSupported())
        return;
    
    this.Disconnect();
    
    this.socket = new WebSocket(url);
    this.socket.instance = this;
    
    this.socket.addEventListener("open", AxWebSocketClient.OnOpen);
    this.socket.addEventListener("message", AxWebSocketClient.OnMessage);
    this.socket.addEventListener("close", AxWebSocketClient.OnClose);
    this.socket.addEventListener("error", AxWebSocketClient.OnError);
    
    //window.addEventListener("beforeunload", AxTcpIpClient.OnBeforeWindowUnload);
};

AxWebSocketClient.prototype.Disconnect = function()
{
    if ((this.socket !== null) && this.connected)
        this.socket.close();
    
    this.socket = null;
    this.connected = false;
};

AxWebSocketClient.prototype.Send = function(data)
{
    if (this.socket === null)
        return;
    
    this.socket.send(data);
};

AxWebSocketClient.prototype.OnConnected = function() { };
AxWebSocketClient.prototype.OnReceive = function(data) { };
AxWebSocketClient.prototype.OnDisconnected = function() { };
AxWebSocketClient.prototype.OnError = function() { };



AxWebSocketClient.IsSupported = function()
{
    return window.WebSocket;
};

AxWebSocketClient.OnOpen = function(event)
{
    var instance = this.instance;
    
    instance.OnConnected();
};

AxWebSocketClient.OnMessage = function(event)
{
    var instance = this.instance;
    var data = event.data;
    
    var reader = new FileReader();
    reader.addEventListener("loadend", function() {
       var bytes = reader.result;
        instance.OnReceive(bytes);
    });
    reader.readAsArrayBuffer(data);
};

AxWebSocketClient.OnClose = function(event)
{
    var instance = this.instance;
    
    instance.OnDisconnect();
};

AxWebSocketClient.OnError = function(event)
{
    var instance = this.instance;
    
    instance.OnError();
};
