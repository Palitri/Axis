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
AxTexture2D.SerializationId_TextureData	= 0x21112006;


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
 * Loads a texture from either raw data, image, stream or file name
 * This methods combines methods Load_1(), Load_2(), Load_3() and Load_4
 * @param {*|AxImage|AxStream|AxString} arg1 In case of loading from raw data it is the raw data source. In case of loading from an image it is the image. In case of loading from a stream, it is the stream. In case of loading from a file, it is the file name.
 * @param {!Integer|*} arg2 In case of loading from raw data it is the raw data width. In case of loading from an image, this parameter is not used.. In case of loading from a stream, it is a callback context. In case of loading from a file, it is a callback context.
 * @param {!Integer|AxTexture2DLoadedCallback} arg3 In case of loading from raw data it is the raw data height. In case of loading from an image, this parameter is not used. In case of loading from a stream, it is a callback function. In case of loading from a file, it is a callback function.
 * @returns {undefined}
 */
AxTexture2D.prototype.Load = function(arg1, arg2, arg3)
{
    if (arg1 instanceof AxImage)
        this.Load_2(arg1);
    else if (arg1 instanceof AxStream)
        this.Load_3(arg1, arg2, arg3);
    else if (arg1 instanceof AxString)
        this.Load_4(arg1, arg2, arg3);
    else
        this.Load_1(arg1, arg2, arg3);
};

/**
 * Loads a texture from raw-encoded data in the graphics device pixel format
 * @param {*} data A buffer which contains the data for the texture
 * @param {Integer} width Width of the texture
 * @param {Integer} height Height of the texture
 */
AxTexture2D.prototype.Load_1 = function(data, width, height)
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
    for (var i = 0; i < sourceImage.pixelData.length; i++)
    {
//        if (i % 4 === 3)
//            sourceImage.pixelData[i] = 128;
//        if (i % 4 === 2)
//            sourceImage.pixelData[i] = 128;
    }
    
    this.Load_1(sourceImage.pixelData, sourceImage.width, sourceImage.height);
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
 * Converts the texture data to an image
 * @returns {AxImage} The image
 */
AxTexture2D.prototype.ToImage = function()
{
    var pixelFormat = this.context.graphicsDevice.GetPixelFormat();

    var result = new AxImage();
    result.SetPixelFormat(pixelFormat);
    result.SetSize(this.width, this.height, 1);
    this.deviceTexture.GetData(result.pixelData);

    return result;
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

    if (AxString.IsNullOrEmpty(this.fileName))
    {
        if (this.context.settings.properties.Get(AxSettings.propertyIndex_EmbedGeneratedTextures).GetBool())
        {
            var image = this.ToImage();
            var nativeImage = new AxNativeImage(image);

            writer.BeginChunk(AxTexture2D.SerializationId_TextureData);
            nativeImage.Save(writer.stream);
            writer.EndChunk();
        }
    }
    else
    {
        if (this.context.settings.properties.Get(AxSettings.propertyIndex_EmbedImportedTextures).GetBool())
        {
            var file = this.context.fileSystem.OpenFile(this.fileName);

            if (this.context.settings.properties.Get(AxSettings.propertyIndex_EmbeddedTexturesOriginalEncoding).GetBool())
            {
                writer.BeginChunk(AxTexture2D.SerializationId_TextureData);
                writer.stream.WriteStreamData(file);
                writer.EndChunk();
            }
            else
            {
                var image = this.context.LoadImageX(file);
                var nativeImage = new AxNativeImage(image);

                writer.BeginChunk(AxTexture2D.SerializationId_TextureData);
                nativeImage.Save(writer.stream);
                writer.EndChunk();
            }
        }
        else
        {
            writer.BeginChunk(AxTexture2D.SerializationId_FileName);
            writer.stream.WriteString(AxFileSystem.GetRelativeFilePath(this.context.serializationParameters.rootDir, this.fileName));
            writer.EndChunk();
        }
    }
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

        case AxTexture2D.SerializationId_TextureData:
        {
            var imageStream = new AxSubStream(reader.stream, reader.chunkSize);
            this.Load_3(imageStream, imageStream, 
                function(caller, context, succeeded)
                {
                });

            reader.stream.Seek(reader.chunkSize, StreamSeekMode.Relative);
            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};
