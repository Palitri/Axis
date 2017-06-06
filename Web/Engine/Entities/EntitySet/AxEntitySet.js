/**
 * Creates a new entity set
 * @constructor
 */
function AxEntitySet()
{
    AxResource.call(this);
    
    this.typeId = AxEntitySet.classId;
    this.resourceType = AxResourceType.EntitySet;

    this.references = new AxList();

    this.properties.Add(new AxProperty(new AxString('Multi Pass'), false));
    this.properties.Add(new AxProperty(new AxString('Branch'), true));
}

AxEntitySet.prototype = Object.create(AxResource.prototype);


AxEntitySet.SerializationId_References = 0x21111005;

AxEntitySet.classId = (AxResourceType.EntitySet << 16) | 0;

AxEntitySet.propertyIndex_MultiPass = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxEntitySet.propertyIndex_Branch    = AxResource.propertyIndex_ChildPropertiesIndex + 1;



AxEntitySet.prototype.Dispose = function()
{
};

/**
 * Checks recursively whether the set contains a given resource.
 * @param {AxResource} resource The resource to be checked for
 * @return {Boolean} True if the set's references and subreferences contain the given resource
 */
AxEntitySet.prototype.HierarchyContains = function(resource)
{
    for (var i = 0; i < this.references.count; i++)
    {
        var reference = this.references.Get(i);

        if (reference === resource)
            return true;

        if (reference.typeId === AxResourceType.EntitySet)
            if (reference.HierarchyContains(resource))
                return true;
    }

    return false;
};

/**
 * Checks recursively whether the hierarchy in the set contains given resource.
 * @param {AxResource} resource The resource to check for in the set hierarchy
 * @return {Boolean} True if the set hierarchy contains the given resource
 */
AxEntitySet.prototype.HierarchyContains = function(resource)
{
    for (var i = 0; i < this.references.count; i++)
    {
        var reference = this.references.Get(i);

        if (reference === resource)
            return true;

        if (reference.typeId === AxResourceType.EntitySet)
            if (reference.HierarchyContains(resource))
                return true;
    }

    return false;
};


/**
 * Writes chunks for all the data which is needed to serialize the entity set.
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxEntitySet.prototype.SerializeChunks = function(writer)
{
    AxResource.prototype.SerializeChunks.call(this, writer);

    writer.BeginChunk(AxEntitySet.SerializationId_References);
    writer.stream.WriteInt32(this.references.count);
    for (var i = 0; i < this.references.count; i++)
        writer.stream.WriteInt64(this.references.Get(i).id);
    writer.EndChunk();
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the entity set's chunks
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxEntitySet.prototype.DeserializeChunk = function(reader)
{
    if (AxResource.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxEntitySet.SerializationId_References:
        {
            var numReferences = reader.stream.ReadInt32();
            for (var i = 0; i < numReferences; i++)
            {
                var id = reader.stream.ReadInt64();

                var link = new AxResource();
                link.resourceType = AxResourceType.None;
                link.id = id;
                this.references.Add(link);
            }
            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};
