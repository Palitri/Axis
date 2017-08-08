/**
 * Creates a new AxHierarchyStreamWriter
 * AxHierarchyStreamWriter utilizes the serialization of data in Axis' heirarchical serialization format
 * @param {AxStream} stream The stream in which to write
 * @constructor
 */
function AxHierarchyStreamWriter(stream)
{
    this.stream = stream;
    this.chunkStack = new AxStack();
}



/**
 * Begins a chunk and writes a chunk header
 * @param {Integer} chunkId Identificator for the chunk
 */
AxHierarchyStreamWriter.prototype.BeginChunk = function(chunkId)
{
    this.stream.WriteUInt32(chunkId);
    this.chunkStack.Push(this.stream.position);
    this.stream.WriteUInt64(0);
};

/**
 * Ends a chunk and finisheds the previously created header fro the chunk.
 * A corresponding call of BeginChunk must have taken place before calling EndChunk
 */
AxHierarchyStreamWriter.prototype.EndChunk = function()
{
    var currentPos = this.stream.position;
    this.stream.Seek(this.chunkStack.Pop());
    this.stream.WriteUInt64(currentPos - this.stream.position - 8);
    this.stream.Seek(currentPos);
};