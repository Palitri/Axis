/**
 * Creates a new AxHierarchyStreamReader
 * AxHierarchyStreamReader utilizes the deserialization of streams built in Axis' heirarchical serialization format
 * @param {AxStream} stream The stream from which to read
 * @constructor
 */
function AxHierarchyStreamReader(stream)
{
    this.stream = stream;
    
    this.chunkStack = new AxStack();

    this.chunkId = 0;
    this.chunkSize = 0;
}

/**
 * Reads a chunk header and stores the header information in the chunkId and chunkSize member variables
 * @return {Boolean} True if a chunk header was read successfully
 */
AxHierarchyStreamReader.prototype.ReadChunkHeader = function()
{
    while (this.chunkStack.count > 0)
        if (this.chunkStack.Peek() <= this.stream.position)
			this.stream.Seek(this.chunkStack.Pop());
        else
            break;

    if (this.stream.length - this.stream.position < 4 + 8)
        return false;

	this.chunkId = this.stream.ReadUInt32();
	this.chunkSize = this.stream.ReadUInt64();
    this.chunkStack.Push(this.stream.position + this.chunkSize);

    return true;
};

/**
 * Skips the current chunk
 */
AxHierarchyStreamReader.prototype.SkipChunk = function()
{
    while (this.chunkStack.count > 0)
        if (this.chunkStack.Peek() < this.stream.position)
            this.stream.Seek(this.chunkStack.Pop());
        else
            break;

    if (this.chunkStack.count > 0)
        this.stream.Seek(this.chunkStack.Pop());
};
