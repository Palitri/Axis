/**
 * Creates an instance of AxImage
 * @param {!AxImage} source Source image to copy
 * @param {!AxPixelFormat} pixelFormat Pixel format for the new image
 * @constructor
 */
function AxWebImage(source, pixelFormat)
{
    AxImage.call(this, source, pixelFormat);
}

AxWebImage.prototype = Object.create(AxImage.prototype);



// TODO: Restore AxImage.Load Synchronoucity, as it should be!!!
// Currently, in order to use browsers' native image decoding, Load is asychronous due to web image being asynchronous
// This causes changes in the following:
//-AxWebImage.prototype.Load
//-AxImage.prototype.Load
//-AxNativeImage.prototype.Load
//
//-AxTexture2D.prototype.Load_3
//-AxTexture2D.prototype.Load_4
//
//-Axis.prototype.LoadImage
//-Axis.prototype.LoadImage_2
//
//-Axis.prototype.AcquireTexture

/**
 * Loads the image from a given stream
 * If the particular implementation of AxImage does not support the format in which the source data is encoded, it should return false
 * Different implementations of AxImage can load differently encoded images
 * @param {AxStream} source Stream containing the image encoded in its particular format
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxImageLoadedCallback} callback A callback method which is to be called when the loading has finished
 */
AxWebImage.prototype.Load = function(source, callbackContext, callback) 
{ 
    try
    {
        var arrayBuffer = new ArrayBuffer(source.length);
        source.ReadData(arrayBuffer, source.length);
        
        var byteBuffer = new Uint8Array(arrayBuffer);

        var webImage = document.createElement('img'); //new Image();
        var sourceData = 'data:image;base64,' + AxWebImage.EncodeBase64(byteBuffer);
        
        webImage.callbackSender = this;
        webImage.onload = function()
        {
            var sender = this.callbackSender;
            
            var webCanvas = document.createElement('canvas');//new Canvas();
            webCanvas.width = webImage.width;
            webCanvas.height = webImage.height;
            webCanvas.getContext('2d').drawImage(webImage, 0, 0, webImage.width, webImage.height);

            var imageData = webCanvas.getContext('2d').getImageData(0, 0, webCanvas.width, webCanvas.height).data;
            sender.pixelData = new Uint8Array(imageData.length);
            var lineSize = webCanvas.width * 4;
            for (var lineIndex = 0; lineIndex < webCanvas.height; lineIndex++)
            {
                var lineOffset = (webCanvas.height - 1 - lineIndex) * lineSize;
                var lineData = imageData.slice(lineOffset, lineOffset + lineSize);
                sender.pixelData.set(lineData, lineIndex * lineSize);
            }
            //sender.pixelData = new Uin8Array();
            sender.pixelFormat = new AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat.ChannelIdRed, AxPixelFormat.ChannelIdGreen, AxPixelFormat.ChannelIdBlue, AxPixelFormat.ChannelIdAlpha);
            sender.width = webCanvas.width;
            sender.height = webCanvas.height;
            sender.depth = 1;
            
            if (!AxUtils.IsUndefinedOrNull(callback))
                callback(sender, callbackContext, true);
        };

        webImage.src = sourceData;

        return true;
    }
    catch (x)
    {
        callback(this, callbackContext, false);
        return false;
    }
};

AxWebImage.EncodeBase64 = function(input)
{
    // TODO: Optimize!
    var keyStr = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=';
    var output = '';
    var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
    var i = 0;

    while (i < input.length) 
    {
        chr1 = input[i++];
        chr2 = i < input.length ? input[i++] : Number.NaN;
        chr3 = i < input.length ? input[i++] : Number.NaN;

        enc1 = chr1 >> 2;
        enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;

        if (isNaN(chr2)) {
            enc3 = enc4 = 64;
        } else if (isNaN(chr3)) {
            enc4 = 64;
        }
        output += keyStr.charAt(enc1) + keyStr.charAt(enc2) +
                  keyStr.charAt(enc3) + keyStr.charAt(enc4);
    }
    return output;
};