/**
 * Creates a new substream of an existing stream
 * The substream is a virtual stream, which uses an isolated portion of contents of another stream
 * @param {AxStream} mainStream The main stream for the substream
 * @param {!Integer} length The length of the substream. If omitted or -1, the whole remaining length between the given offset and the size of the main stream is assumed.
 * @param {!Integer} offset The offset location in the main stream, at which the substream starts. If omitted or -1, the current position of the main stream is assumed.
 * @param {!Boolean} writeBeyondLength Denotes whether to allow for writing data beyond the specified length of the substream. If omitted, true is assumed.
 * @constructor
 */
function AxSubStream(mainStream, length, offset, writeBeyondLength)
{
    if (AxUtils.IsUndefinedOrNull(offset))
        offset = -1;
    if (AxUtils.IsUndefinedOrNull(length))
        length = -1;
    if (AxUtils.IsUndefinedOrNull(writeBeyondLength))
        writeBeyondLength = true;
    
    AxStream.call(this);

    this.mainStream = mainStream;
    
    this.offset = offset == -1 ? mainStream.position : offset;
    this.length = length == -1 ? mainStream.length - offset : length;
    this.writeBeyondLength = writeBeyondLength;
}

AxSubStream.prototype = Object.create(AxStream.prototype);


/**
 * Reads data from the sub stream and returns the number of bytes which were successfully read.
 * The position of the original stream is not changed
 * @param {ArrayBuffer} data An array buffer of the appropriate size, into which the read data will be stored
 * @param {Integer} size Number of bytes to read
 * @return {Integer} The number of bytes which were successfully read
 */
AxSubStream.prototype.ReadData = function(data, size)
{
    var originalPosition = this.mainStream.position;

    size = AxMath.Min(size, this.position + this.length);

    this.mainStream.Seek(this.offset + this.position, StreamSeekMode.FromBeginning);
    var result = this.mainStream.ReadData(data, size);
    this.mainStream.Seek(originalPosition, StreamSeekMode.FromBeginning);

    this.position += result;

    return result;
};

/**
 * Writes data to the sub stream and returns the number of bytes which were successfully written
 * If writing at a position before the end of the stream, depending on the stream's write mode, new data can either overwite bytes ahead of the position or be inserted - pushing the existing data forward.
 * The position of the original stream is not changed
 * @param {ArrayBuffer} data An array buffer which holds the data to be written
 * @param {Integer} size Number of bytes to write
 * @return {Integer} The number of bytes which were successfully written
 */
AxSubStream.prototype.WriteData = function(data, size)
{
    var  originalPosition = this.mainStream.position;

    if (!this.writeBeyondLength)
        size = AxMath.Min(size, this.position + this.length);

    this.mainStream.Seek(this.offset + this.position, StreamSeekMode.FromBeginning);
    var result = this.mainStream.WriteData(data, size);
    this.mainStream.Seek(originalPosition, StreamSeekMode.FromBeginning);

    this.position += result;
    if (this.position > this.length)
            this.length = this.position;

    return result;
};
