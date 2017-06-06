/**
 * Creates a base mechanism object.
 * Used to serve as a base for other mechanism implementations.
 * The mechanism is an object, which is used as a variable value and can output various types of data, including but not limited to resources
 * @constructor
 */
function AxMechanism()
{
    AxResource.call(this);

    this.typeId = AxMechanism.classId;

    this.resourceType = AxResourceType.Mechanism;
    this.parameter = new AxParameter();

    this._updateState = false;
}

AxMechanism.prototype = Object.create(AxResource.prototype);



AxMechanism.SerializationId_Parameter   = 0x21111008;

AxMechanism.classId = (AxResourceType.Mechanism << 16) | 0;




AxMechanism.prototype.Dispose = function()
{
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxMechanism.prototype.Process = function(deltaTime)
{
    return false;
};

/**
 * Writes chunks for all the data which is needed to serialize the resource. 
 * Can be overridden by child resources to serialize their implementation-specific data
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxMechanism.prototype.SerializeChunks = function(writer)
{
    AxResource.prototype.SerializeChunks.call(this, writer);

    writer.BeginChunk(AxMechanism.SerializationId_Parameter);
    this.SerializeParameter(this.parameter, writer.stream);
    writer.EndChunk();
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the resource's chunks. Override to read chunks of resource-specific data
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxMechanism.prototype.DeserializeChunk = function(reader)
{
    if (AxResource.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxMechanism.SerializationId_Parameter:
        {
            this.DeserializeParameter(this.parameter, reader.stream);
            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};