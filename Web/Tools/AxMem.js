/**
 * Creates an instane of AxMem
 * AxMem provides tools for memory management
 * @constructor
 */
function AxMem()
{
}



// Static methods



/**
 * Returns a new ArrayBuffer of the designed size
 * @param {Number} size The size to allocate
 * @return {ArrayBuffer} A new ArrayBuffer of the designed size
 */
AxMem.Get = function(size)
{
    return new ArrayBuffer(size);
};

/**
 * Returns a new ArrayBuffer of the designed size
 * @param {ArrayBuffer} destination The ArrayBuffer to be zeroed
 * @param {Number} destinationOffset The location in bytes where memory will be zeroed
 * @param {Number} size The size of the memory to be zeroed
 */
AxMem.Zero = function(destination, destinationOffset, size)
{
    var destinationBytes = new Uint8Array(destination);
    
    for (var i = 0; i < size; i++)
        destinationBytes[destinationOffset + i] = 0;
};

/**
 * Fills a block of a certain size of an ArrayBuffer, by repeatedly writing a given value, which is also located in an ArrayBuffer
 * @param {ArrayBuffer} destination The ArrayBuffer which to fill
 * @param {Number} destinationOffset The offset location at which to fill
 * @param {Number} fillSize The size in bytes of the block to fill
 * @param {ArrayBuffer} value The ArrayBuffer holding the value which is to be filled
 * @param {Number} valueOffset The offset location where the value which is to be filled is
 * @param {Number} valueSize The size of the fill value
 */
AxMem.Fill = function(destination, destinationOffset, fillSize, value, valueOffset, valueSize)
{
    var destinationBytes = new Uint8Array(destination);
    var valueBytes = new Uint8Array(value);
    
    for (var i = 0; i < fillSize; i++)
        destinationBytes[destinationOffset + i] = valueBytes[valueOffset + i % valueSize];
};

AxMem.Copy = function(destination, destinationOffset, source, sourceOffset, size)
{
    var destBytes = new Uint8Array(destination);
    var sourceBytes = new Uint8Array(source);
    
    for (var i = 0; i < size; i++)
        destBytes[destinationOffset + i] = sourceBytes[sourceOffset + i];
};

AxMem.Compare = function(block1, block1Offset, block2, block2Offset, size)
{
    var block1Bytes = new Uint8Array(block1);
    var block2Bytes = new Uint8Array(block2);
    
    for (var i = 0; i < size; i++)
    {
        var difference = block1Bytes[block1Offset + i] - block2Bytes[block2Offset + i];
        if (difference !== 0)
        {
            return difference > 0 ? 1 : 0;
        }
    }
    
    return 0;
};