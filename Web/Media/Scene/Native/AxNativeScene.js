/**
 * Cretes a native scene media
 * @constructor
 */
function AxNativeScene()
{
    AxScene.call(this);
}

AxNativeScene.prototype = Object.create(AxScene.prototype);

AxNativeScene.formatName = 'axs';


/**
 * Loads a media's content, decoding it from its native format into raw data
 * @param {AxStream} source A Stream holding a media's content in its native format. The stream must be positioned at the start of the content. For example, if it represents a file of the media's native type, it's position should normally be zero
 * @return {Boolean} True if succeeds, false otherwise
 */
AxNativeScene.prototype.Load = function(source)
{
    if (AxUtils.IsUndefinedOrNull(this.context))
        return false;

    var reader = new AxNativeFileReader(this.context);

    return reader.Read(source);
};

/**
 * Saves the media, encoding it into the given format
 * @param {AxStream} destination Stream where the encoded data will be written, starting at the stream's current position
 * @param {String} format The format, in which the media must be encoded into. This is useful for medas, which support multiple formats. For example, to encode an image in png format, it should be "png", or to encode a sound in wav format, it should be "wav" and so on
 * @return {Bool} True if succeeds, false otherwise
 */
AxNativeScene.prototype.Save = function(destination, format)
{
    if (AxUtils.IsUndefinedOrNull(this.context))
        return false;

    if (!AxUtils.IsUndefinedOrNull(format))
    {
        var encodingCode = new AxString(format).Trim('.');
        if ((encodingCode.length !== 0) && (!encodingCode.Equals(AxNativeScene.formatName)))
            return false;
    }

    var writer = new AxNativeFileWriter(this.context);
    writer.Write(destination);

    return true;
};
