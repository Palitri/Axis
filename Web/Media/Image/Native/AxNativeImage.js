/**
 * Creates an instance of AxImage
 * @param {!AxImage} source Source image to copy
 * @param {!AxPixelFormat} pixelFormat Pixel format for the new image
 * @constructor
 */
function AxNativeImage(source, pixelFormat)
{
    AxImage.call(this, source, pixelFormat);
}

AxNativeImage.prototype = Object.create(AxImage.prototype);


AxNativeImage.SerializationId_Main	= 0x4d495841; // AXIM
AxNativeImage.SerializationId_Data	= 0x00004d49;
AxNativeImage.SerializationId_Size	= 0x00014d49;
AxNativeImage.SerializationId_Format	= 0x00024d49;
AxNativeImage.SerializationId_Encoding	= 0x00034d49;

AxNativeImage.Encoding_Raw  = 0x0000;

AxNativeImage.formatName = 'axi';


/**
 * Loads the image from a given stream
 * If the particular implementation of AxImage does not support the format in which the source data is encoded, it should return false
 * Different implementations of AxImage can load differently encoded images
 * @param {AxStream} source Stream containing the image encoded in its particular format
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxImageLoadedCallback} callback A callback method which is to be called when the loading has finished
 */
AxNativeImage.prototype.Load = function(source, callbackContext, callback) 
{
    var reader = new AxHierarchyStreamReader(source);

    var mainChunkEnd = -1;

    while (reader.ReadChunkHeader())
    {
        switch (reader.chunkId)
        {
            case AxNativeImage.SerializationId_Main:
            {
                mainChunkEnd = reader.stream.position + reader.chunkSize;
                break;
            }

            case AxNativeImage.SerializationId_Size:
            {
                var width = reader.stream.ReadInt32();
                var height = reader.stream.ReadInt32();
                var depth = reader.stream.ReadInt32();

                this.SetSize(width, height, depth);

                break;
            }

            case AxNativeImage.SerializationId_Format:
            {
                this.pixelFormat = AxSerializationUtils.DeserializePixelFormat(source);

                this.SetPixelFormat(this.pixelFormat, false);

                break;
            }

            case AxNativeImage.SerializationId_Data:
            {
                var pixelDataSize = this.width * this.height * this.depth * this.pixelFormat.sizeInBytes;
                if (pixelDataSize > reader.chunkSize)
                    pixelDataSize = reader.chunkSize;

                if (pixelDataSize > 0)
                    reader.stream.ReadData(this.pixelData, pixelDataSize);

                break;
            }

            default:
            {
                if ((reader.chunkSize < 0) || (reader.stream.position + reader.chunkSize > mainChunkEnd))
                {
                    if (!AxUtils.IsUndefinedOrNull(callback))
                        callback(this, callbackContext, false);
                    
                    return false;
                }

                reader.SkipChunk();

                break;
            }
        }

        if (reader.stream.position >= mainChunkEnd)
            break;
    }

    if (!AxUtils.IsUndefinedOrNull(callback))
        callback(this, callbackContext, true);
    
    return true;
};

/**
 * Saves the image to the given stream in the given encoding format
 * If the particular implementation of AxImage does not support the requested encoding format, it should return false and shouldn't write anything to the stream
 * Different implementations of AxImage can save images in different encodings
 * @param {AxStream} destination Stream to which the image will be written in the requested format
 * @param {String} format Format in which to encode the image. For example, such as 'jpg', 'png' and etc. 
 * @return {Boolean} True if the image was successfully saved to the given stream in the given encoding format
 */
AxNativeImage.prototype.Save = function(destination, format)
{
    if (AxUtils.IsUndefinedOrNull(format))
    {
        var encodingCode = AxString(format).Trim('.');
        if ((encodingCode.length !== 0) && (!encodingCode.Equals(AxNativeImage.formatName)))
            return false;
    }

    var writer = new AxHierarchyStreamWriter(destination);

    writer.BeginChunk(AxNativeImage.SerializationId_Main);

    writer.BeginChunk(AxNativeImage.SerializationId_Size);
    writer.stream.WriteInt32(this.width);
    writer.stream.WriteInt32(this.height);
    writer.stream.WriteInt32(this.depth);
    writer.EndChunk();

    writer.BeginChunk(AxNativeImage.SerializationId_Format);
    AxSerializationUtils.SerializePixelFormat(destination, this.pixelFormat);
    writer.EndChunk();

    writer.BeginChunk(AxNativeImage.SerializationId_Data);
    writer.stream.WriteData(this.pixelData, this.width * this.height * this.depth * this.pixelFormat.sizeInBytes);
    writer.EndChunk();

    writer.EndChunk();

    return true;
};