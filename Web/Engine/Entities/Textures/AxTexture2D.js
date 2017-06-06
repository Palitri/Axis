/**
 * Creates a new 2D texture
 * @param {Axis} context The context to use for the texture
 * @constructor
 */
function AxTexture2D(context)
{
    AxResource.call(this);

    this.context = context;

    this.typeId = AxTexture2D.classId;

    this.resourceType = AxResourceType.Texture;

    this.deviceTexture = this.context.graphicsDevice.CreateTexture();

    this.width = 0;
    this.height = 0;

    this.properties.Add(new AxProperty(new AxString('Width'), this.width));
    this.properties.Add(new AxProperty(new AxString('Height'), this.height));
}

AxTexture2D.prototype = Object.create(AxResource.prototype);


AxTexture2D.classId = (AxResourceType.Texture << 16) | 0;

AxTexture2D.propertyIndex_Width     = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxTexture2D.propertyIndex_Height    = AxResource.propertyIndex_ChildPropertiesIndex + 1;

AxTexture2D.SerializationId_FileName	= 0x21111006;


/**
 * Gets fired when a texture gets loaded
 * @callback AxTexture2DLoadedCallback
 * @param {*} caller The texutre object, which was loaded
 * @param {*} context An object holding the context for the callback. It is the object passed to the Loading method
 * @param {Boolean} succeeded True if the loading was successful, false otherwise
 */

/**
 * Explicitly dispose of the resources allocated by the object
 */
AxTexture2D.prototype.Dispose = function()
{
};

/**
 * Loads a texture from raw-encoded data in the graphics device pixel format
 * @param {*} data A buffer which contains the data for the texture
 * @param {Integer} width Width of the texture
 * @param {Integer} height Height of the texture
 */
AxTexture2D.prototype.Load = function(data, width, height)
{
    this.width = width;
    this.height = height;

    this.properties.Get(AxTexture2D.propertyIndex_Width).GetEffectiveParameter().SetInt(width);
    this.properties.Get(AxTexture2D.propertyIndex_Height).GetEffectiveParameter().SetInt(height);

    this.deviceTexture.Update(data, width, height);
};
	
/**
 * Loads a texture from an image. The image data must be in the pixel format of the graphics device
 * @param {AxImage} sourceImage The image source for the texture
 */
AxTexture2D.prototype.Load_2 = function(sourceImage)
{
    this.Load(sourceImage.pixelData, sourceImage.width, sourceImage.height);
};

/**
 * Loads a texture from a stream which contains an image encoded in any of the supported formats
 * @param {AxStream} source A stream containing the source image, encoded in any of the supported formats
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxTexture2DLoadedCallback} callback A callback method which is to be called when the loading has finished
 */
AxTexture2D.prototype.Load_3 = function(source, callbackContext, callback)
{
    this.context.LoadImage(source, this,
        function(sender, context, image)
        {
            if (image !== null)
            {
                image.SetPixelFormat(context.context.graphicsDevice.GetPixelFormat());
                context.Load_2(image);
                if (!AxUtils.IsUndefinedOrNull(callback))
                    callback(context, callbackContext, true);
            }
            else
            {
                if (!AxUtils.IsUndefinedOrNull(callback))
                    callback(context, callbackContext, false);
            }

        });

    return true;
};

/**
 * Loads a texture from a given file
 * @param {AxString} fileName Name of the file to open and use as a texture
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxTexture2DLoadedCallback} callback A callback method which is to be called when the loading has finished
 */
AxTexture2D.prototype.Load_4 = function(fileName, callbackContext, callback)
{
    this.context.fileSystem.OpenFile(fileName, this,
        function(context, stream, path, fileSystem)
        {
            if (stream !== 0)
            {
                context.Load_3(stream, context,
                    function(sender, context, succeeded)
                    {
                        if (succeeded)
                            sender.fileName = path;

                        if (!AxUtils.IsUndefinedOrNull(callback))
                            callback(sender, callbackContext, succeeded);
                    });
            }
        });
};

/**
 * Reads the texture data and returns it as an image
 * @return {AxImage} The image representing the texture
 */
AxTexture2D.prototype.Read = function()
{
    var result = new AxImage();
    result.pixelFormat = this.context.graphicsDevice.GetPixelFormat();
    result.SetSize(this.properties[AxTexture2D.propertyIndex_Width].GetEffectiveValue(), this.properties[AxTexture2D.propertyIndex_Height].GetEffectiveValue(), 1);
    this.deviceTexture.GetData(result.pixelData);

    return result;
};

/**
 * Updates the texture according to its properties
 */
AxTexture2D.prototype.UpdateToProperties = function()
{
    var propertyWidth = this.properties.Get(AxTexture2D.propertyIndex_Width).GetEffectiveParameter().GetInt();
    var propertyHeight = this.properties.Get(AxTexture2D.propertyIndex_Height).GetEffectiveParameter().GetInt();
    if ((this.width !== propertyWidth) || (this.height !== propertyHeight))
    {
        this.width = propertyWidth;
        this.height = propertyHeight;

        this.deviceTexture.Update(null, this.width, this.height);
    }
};

/**
 * Writes chunks for all the data which is needed to serialize the texture. 
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxTexture2D.prototype.SerializeChunks = function(writer)
{
    AxResource.prototype.SerializeChunks.call(this, writer);

    writer.BeginChunk(AxTexture2D.SerializationId_FileName);
    writer.stream.WriteString(AxFileSystem.GetRelativeFilePath(this.context.serializationParameters.rootDir, this.fileName));
    writer.EndChunk();
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the texture's chunks
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxTexture2D.prototype.DeserializeChunk = function(reader)
{
    if (AxResource.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxTexture2D.SerializationId_FileName:
        {
            var fileName = reader.stream.ReadString();
            this.Load_4(AxFileSystem.MergePaths(this.context.serializationParameters.rootDir, fileName));
            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};
