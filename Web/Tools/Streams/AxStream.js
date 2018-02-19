/**
 * Defines a mode for seeking through a stream
 */
var StreamSeekMode = Object.freeze(
{
    FromBeginning : 0,
    Relative : 1,
    FromEnd : 2
});


/**
 * Creates a stream object
 * This prototype is meant to be inherited by overriding the following two methods:
 * ReadData(data, size) where data is an ArrayBuffer, size is the amount of bytes to read from the stream into the ArrayBuffer and the return value is the amount of bytes actually read.
 * WriteData(data, size) where data is an ArrayBuffer, size is the amount of bytes to write to the stream from the ArrayBuffer and the return value is the amount of bytes actually written.
 * @constructor
 */
function AxStream()
{
    this.position = 0;
    this.length = 0;
    
    this.buffer = new ArrayBuffer(8);
    this.bufferUInt8 = new Uint8Array(this.buffer);
    this.bufferInt8 = new Int8Array(this.buffer);
    this.bufferUInt16 = new Uint16Array(this.buffer);
    this.bufferInt16 = new Int16Array(this.buffer);
    this.bufferUInt32 = new Uint32Array(this.buffer);
    this.bufferInt32 = new Int32Array(this.buffer);
    this.bufferFloat32 = new Float32Array(this.buffer);
    this.bufferFloat64 = new Float64Array(this.buffer);
}

/**
 * Explicitly dispose of the resources allocated by the stream
 */
AxStream.prototype.Dispose = function() { };

/**
 * Reads data from the memory stream into an ArrayBuffer and returns the number of bytes which were successfully read
 * @param {ArrayBuffer} destination An array buffer of the appropriate size, into which the read data will be stored
 * @param {Integer} size Number of bytes to read
 * @return {Integer} The number of bytes which were successfully read
 */
AxStream.prototype.ReadData = function(destination, size) { };

/**
 * Writes data to the memory stream from an ArrayBuffer and returns the number of bytes which were successfully written
 * If writing at a position before the end of the stream, depending on the stream's write mode, new data can either overwite bytes ahead of the position or be inserted - pushing the existing data forward.
 * @param {ArrayBuffer} source An array buffer which holds the data to be written
 * @param {Integer} size Number of bytes to write
 * @return {Integer} The number of bytes which were successfully written
 */
AxStream.prototype.WriteData = function(source, size) { };

/**
 * Seeks number of offset bytes, in a manner determined by a seek type
 * @param {Integer} offset The offset of seeking is a number of bytes
 * @param {StreamSeekMode} seekType A seeking mode determines how to perform the seek. If omitted, seeking is performed from the beginning of the stream
 * @return {Boolean} True if seek was successful, False other wise, if for example it was outside the bounds of the stream
 */
AxStream.prototype.Seek = function(offset, seekType)
{
    if (AxUtils.IsUndefined(seekType))
        seekType = StreamSeekMode.FromBeginning;
    
    var newPosition = seekType === StreamSeekMode.FromBeginning ? offset : seekType === StreamSeekMode.Relative ? this.position + offset : seekType === StreamSeekMode.FromEnd ? this.length - offset : -1;

    if ((newPosition >= 0) && (newPosition <= this.length))
    {
        this.position = newPosition;
        return true;
    }

    return false;
};

/**
 * Writes data from another stream
 * After the writing is performed, the source stream's location is changed, according to the size of the data which was written
 * @param {AxStream} source Stream which is the source of the data to write
 * @param {Integer} size Size in bytes of the data to be written
 * @param {Integer} maxBufferSize Maximum size of the buffer to use for the writing. Can be omitted.
 * @return {Integer} The number of bytes actually written
 */
AxStream.prototype.WriteStreamData = function(source, size, maxBufferSize)
{
    if (AxUtils.IsUndefined(maxBufferSize))
        maxBufferSize = 65536;
    
     if (size === -1)
        size = source.length - source.position;

    var chunkSize = AxMath.Min(size, maxBufferSize);
    var dataChunk = new ArrayBuffer(chunkSize);

    var bytesToWrite = size;
    while (bytesToWrite > 0)
    {
        var bytesToRead = AxMath.Min(bytesToWrite, chunkSize);
        var bytesRead = source.ReadData(dataChunk, bytesToRead);
        var bytesWritten = this.WriteData(dataChunk, bytesRead);

        bytesToWrite -= bytesWritten;

        if (bytesToRead !== bytesWritten)
            break;
    }

    return size - bytesToWrite;
};


/**
 * Reads a Boolean value from the stream
 * @return {Boolean} A Boolean value read from the stream
 */
AxStream.prototype.ReadBool = function()
{
    this.ReadData(this.buffer, 1);
    return this.bufferUInt8[0] !== 0;
};

/**
 * Writes a Boolean value to the stream
 * @param {Boolean} value The Boolean value to be written
*/
AxStream.prototype.WriteBool = function(value)
{
    this.bufferUInt8[0] = value ? 1 : 0;
    this.WriteData(this.buffer, 1);
};


/**
 * Reads a signed byte value from the stream
 * @return {Integer} A signed byte value read from the stream
 */
AxStream.prototype.ReadInt8 = function()
{
    this.ReadData(this.buffer, 1);
    return this.bufferInt8[0];
};

/**
 * Writes a signed byte value to the stream
 * @param {Integer} value The signed 8 bit integer value to be written
*/
AxStream.prototype.WriteInt8 = function(value)
{
    this.bufferInt8[0] = value;
    this.WriteData(this.buffer, 1);
};

/**
 * Reads a signed 16 bit integer value from the stream
 * @return {Integer} A signed 16 bit integer value read from the stream
 */
AxStream.prototype.ReadInt16 = function()
{
    this.ReadData(this.buffer, 2);
    return this.bufferInt16[0];
};

/**
 * Writes a signed 16 bit integer value to the stream
 * @param {Integer} value The signed 16 bit integer value to be written
*/
AxStream.prototype.WriteInt16 = function(value)
{
    this.bufferInt16[0] = value;
    this.WriteData(this.buffer, 2);
};

/**
 * Reads a signed 32 bit integer value from the stream
 * @return {Integer} A signed 32 bit integer value read from the stream
 */
AxStream.prototype.ReadInt32 = function()
{
    this.ReadData(this.buffer, 4);
    return this.bufferInt32[0];
};

/**
 * Writes a signed 32 bit integer value to the stream
 * @param {Integer} value The signed 32 bit integer value to be written
 */
AxStream.prototype.WriteInt32 = function(value)
{
    this.bufferInt32[0] = value;
    this.WriteData(this.buffer, 4);
};

/**
 * Reads a signed 64 bit integer value from the stream
 * @return {Integer} A signed 64 bit integer value read from the stream
 */
AxStream.prototype.ReadInt64 = function()
{
    this.ReadData(this.buffer, 8);
    return this.bufferInt32[1] << 32 | this.bufferInt32[0];
};

/**
 * Writes a signed 64 bit integer value to the stream
 * @param {Integer} value The signed 64 bit integer value to be written
 */
AxStream.prototype.WriteInt64 = function(value)
{
    this.bufferInt32[0] = value & 0xFFFFFFFF;
    this.bufferInt32[1] = value & 0xFFFFFFFF00000000;
    this.WriteData(this.buffer, 8);
};


/**
 * Reads a signed 32 bit integer value of variable size from the stream
 * In the 7-bit encoding, the last bit of each of the bytes of the value represent a flag which denotes whether this is the last byte of the value. This means that the value, if small enough, can be encoded in less than 4 bytes (or 32 bits)
 * A value which uses this encoding practically uses 7 of the total 8 bits of each of its bytes, because the last one is a flag.
 * This encoding potentially saves space for small values, while in the same time gives the ability to have large values as well.
 * @return {Integer} A signed 32 bit integer value read from the stream
 */
AxStream.prototype.Read7BitEncodedInt32 = function()
{
    var result = 0;
    var bits = new ArrayBuffer(1);
    var bitsArray = new Uint8Array(bits);
    do
    {
        this.ReadData(bits, 1);
        result = (result << 7) | (bitsArray[0] & 0x7f);
    } 
    while ((bitsArray & (1 << 7)) !== 0);

    return result;
};

/**
 * Writes a signed 32 bit integer value of variable size to the stream
 * In the 7-bit encoding, the last bit of each of the bytes of the value represent a flag which denotes whether this is the last byte of the value. This means that the value, if small enough, can be encoded in less than 4 bytes (or 32 bits)
 * A value which uses this encoding practically uses 7 of the total 8 bits of each of its bytes, because the last one is a flag.
 * This encoding potentially saves space for small values, while in the same time gives the ability to have large values as well.
 * @param {Integer} value The signed 32 bit integer value to be written
 */
AxStream.prototype.Write7BitEncodedInt32 = function(value)
{
    var writeValue = 0;
    var writeBytes = 0;

    for (;;)
    {
        writeValue = (writeValue << 8) | (value & 0x7f);
        value = value >> 7;
        writeBytes++;

        if (value !== 0)
            writeValue |= 0x80;
        else
            break;
    }

    var data = new ArrayBuffer(4);
    var dataArray = new Int32Buffer(data);
    dataArray[0] = writeValue;
    this.WriteData(data, writeBytes);
};

/**
 * Reads an unsigned byte value from the stream
 * @return {Integer} An unsigned byte value read from the stream
 */
AxStream.prototype.ReadUInt8 = function()
{
    this.ReadData(this.buffer, 1);
    return this.bufferUInt8[0];
};

/**
 * Writes an unsigned byte value to the stream
 * @param {Integer} value The unsigned 8 bit integer value to be written
 */
AxStream.prototype.WriteUInt8 = function(value)
{
    this.bufferUInt8[0] = value;
    this.WriteData(this.buffer, 1);
};

/**
 * Reads an unsigned 16 bit integer value from the stream
 * @return {Integer} An unsigned 16 bit integer value read from the stream
 */
AxStream.prototype.ReadUInt16 = function()
{
    this.ReadData(this.buffer, 2);
    return this.bufferUInt16[0];
};

/**
 * Writes an unsigned 16 bit integer value to the stream
 * @param {Integer} value The unsigned 16 bit integer value to be written
 */
AxStream.prototype.WriteUInt16 = function(value)
{
    this.bufferUInt16[0] = value;
    this.WriteData(this.buffer, 2);
};

/**
 * Reads an unsigned 32 bit integer value from the stream
 * @return {Integer} An unsigned 32 bit integer value read from the stream
 */
AxStream.prototype.ReadUInt32 = function()
{
    this.ReadData(this.buffer, 4);
    return this.bufferUInt32[0];
};

/**
 * Writes an unsigned 32 bit integer value to the stream
 * @param {Integer} value The unsigned 32 bit integer value to be written
 */
AxStream.prototype.WriteUInt32 = function(value)
{
    this.bufferUInt32[0] = value;
    this.WriteData(this.buffer, 4);
};

/**
 * Reads an unsigned 64 bit integer value from the stream
 * @return {Integer} An unsigned 64 bit integer value read from the stream
 */
AxStream.prototype.ReadUInt64 = function()
{
    this.ReadData(this.buffer, 8);
    return this.bufferUInt32[1] << 32 | this.bufferUInt32[0];
};

/**
 * Writes an unsigned 64 bit integer value to the stream
 * @param {Integer} value The unsigned 64 bit integer value to be written
 */
AxStream.prototype.WriteUInt64 = function(value)
{
    this.bufferUInt32[0] = value & 0xFFFFFFFF;
    this.bufferUInt32[1] = value & 0xFFFFFFFF00000000;
    this.WriteData(this.buffer, 8);
};


/**
 * Reads a 32 bit floating point value from the stream
 * @return {Number} A 32 bit floating point value read from the stream
 */
AxStream.prototype.ReadFloat32 = function()
{
    this.ReadData(this.buffer, 4);
    return this.bufferFloat32[0];
};

/**
 * Writes a 32 bit floating point value to the stream
 * @param {Number} value The 32 bit floating point value to be written
 */
AxStream.prototype.WriteFloat32 = function(value)
{
    this.bufferFloat32[0] = value;
    this.WriteData(this.buffer, 4);
};

/**
 * Reads a 64 bit floating point value from the stream
 * @return {Number} A 64 bit floating point value read from the stream
 */
AxStream.prototype.ReadFloat64 = function()
{
    this.ReadData(this.buffer, 8);
    return this.bufferFloat64[0];
};

/**
 * Writes a 64 bit floating point value to the stream
 * @param {Number} value The 64 bit floating point value to be written
 */
AxStream.prototype.WriteFloat64 = function(value)
{
    this.bufferFloat64[0] = value;
    this.WriteData(this.buffer, 8);
};


/**
 * Reads a string value from the stream
 * @return {AxString} A string value read from the stream
 */
AxStream.prototype.ReadString = function()
{
    var dataLength = this.Read7BitEncodedInt32();
    var charCodeData = new ArrayBuffer(dataLength);
    var charCodeDataArray = new Uint8Array(charCodeData);

    this.ReadData(charCodeData, dataLength);

    var result = new AxString();
    AxString.DecodeUtf8String(result, charCodeDataArray, dataLength, 0);

    return result;
};

/**
 * Writes a string value to the stream
 * @param {AxString|String} value The string value to be written
 */
AxStream.prototype.WriteString = function(value)
{
    var dataLength = AxString.GetStringAsUTF8Length(value);
    var data = new ArrayBuffer(dataLength);
    AxString.EncodeUtf8String(data, value);

    this.Write7BitEncodedInt32(dataLength);
    this.WriteData(data, dataLength);
};

/**
 * Reads an uninterrupted set of text characters from the stream.
 * Useful for reading a single word or number in a series of such
 * TODO: make parameter for the interruption characters (or strings)
 * @return {Number} An uninterrupted set of text characters read from the stream
 */
//AxString AxStream.prototype.ReadText = function()
// Follow more AxString methods.
// TODO: implement them