var StreamWriteMode = Object.freeze(
{
    Overwrite : 0,
    Insert : 1
});

/**
 * @constructor
 * Creates a memory stream object
 */
function AxMemoryStream()
{
    AxStream.call(this);

    this.capacity = 0;
    this.data = null;
}

AxMemoryStream.prototype = Object.create(AxStream.prototype);

AxMemoryStream.prototype.AxMemoryStream = function(initialCapacity)
{
    this.SetWriteMode(StreamWriteMode.Overwrite);
    
    this.capacity = 0;
    this.data = null;
    
    this.Resize(initialCapacity);
};

AxMemoryStream.prototype.AxMemoryStream_2 = function(data, size)
{
    this.length = size;

    this.SetWriteMode(StreamWriteMode.Overwrite);

    this.capacity = size;
    this.data = data;
};

/**
 * Reads data from the memory stream and returns the number of bytes which were successfully read
 * @param {ArrayBuffer} destination An array buffer of the appropriate size, into which the read data will be stored
 * @param {Integer} size Number of bytes to read
 * @return {Integer} The number of bytes which were successfully read
 */
AxMemoryStream.prototype.ReadData = function(destination, size)
{
    var bytesToCopy = AxMath.Min(this.length - this.position, size);
    AxMem.Copy(destination, 0, this.data, this.position, bytesToCopy);
    
    this.position += bytesToCopy;

    return bytesToCopy;
};

/**
 * Writes data to the memory stream and returns the number of bytes which were successfully written
 * If writing at a position before the end of the stream, depending on the stream's write mode, new data can either overwite bytes ahead of the position or be inserted - pushing the existing data forward.
 * @param {ArrayBuffer} data An array buffer which holds the data to be written
 * @param {Integer} size Number of bytes to write
 * @return {Integer} The number of bytes which were successfully written
 */
AxMemoryStream.prototype.WriteData = function(data, size)
{
    var requiredCapacity = this.writeMode === StreamWriteMode.Overwrite ? this.position + size : this.length + size;
    var newCapacity = this.capacity;
    while (requiredCapacity > newCapacity)
    {
        newCapacity *= 2;
    }
    if (newCapacity !== this.capacity)
        this.Resize(newCapacity);

    if (this.writeMode === StreamWriteMode.Insert)
    {
        var tailBytes = this.length - this.position;
        if (size > tailBytes)
        {
            AxMem.Copy(this.data, this.position + size, this.data, this.position, tailBytes);
        }
        else
        {
            var tailData = AxMem.Get(tailBytes);
            AxMem.Copy(tailData, 0, this.data, this.position, tailBytes);
            AxMem.Copy(this.data, this.position + size, tailData, 0, tailBytes);
        }

        this.length += size;
    }

    AxMem.Copy(this.data, this.position, data, 0, size);

    this.position += size;

    this.length = AxMath.Max(this.length, this.position);

    return size;
};

/**
 * Sets the write mode for the stream
 * If writing at a position before the end of the stream, depending on the stream's write mode, new data can either overwite bytes ahead of the position or be inserted - pushing the ahead bytes forward.
 * @param {StreamWriteMode} writeMode The write mode for the stream
 */
AxMemoryStream.prototype.SetWriteMode = function(writeMode)
{
    this.writeMode = writeMode;
};

/**
 * Resizes the memory stream to the given capacity in bytes. Existing data is saved as possible.
 * @param {Number} newCapacity The new capacity after the resize
 */
AxMemoryStream.prototype.Resize = function(newCapacity)
{
    var newData = AxMem.Get(newCapacity);

    if (this.length > 0)
        AxMem.Copy(newData, 0, this.data, 0, AxMath.Min(this.length, newCapacity));

    this.data = newData;
    this.capacity = newCapacity;
};

/**
 * Gets the ArrayBuffer which holds the data of the stream
 * @return {ArrayBuffer} The ArrayBuffer which holds the data of the stream
 */
AxMemoryStream.prototype.GetDataPointer = function()
{
	return this.data;
};
