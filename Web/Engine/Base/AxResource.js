function AxResource()
{
    AxEntity.call(this);
    
    this.ResourceType = AxResourceType.None;
    this.id = -1;
    this.name = new AxString();

    this.tag = 0;
    
    this.properties = new AxList();
    
    this.properties.Add(new AxProperty(new AxString('Active'), true));
}

AxResource.prototype = Object.create(AxEntity.prototype);


AxResource.SerializationId_Resource	= 0x21110000;
AxResource.SerializationId_ResourceId	= 0x21111000;
AxResource.SerializationId_ResourceName	= 0x21112000;
AxResource.SerializationId_Properties	= 0x21113000;
AxResource.SerializationId_Property	= 0x21113100;

AxResource.propertyIndex_Enabled = 0;
AxResource.propertyIndex_ChildPropertiesIndex = 1;




/**
 * Gets the property with the given name. If the resource has no such property, returns null
 * @param {AxString} name Name of the property to get
 * @return {AxProperty} The property with the given name. If the resource has no such property, returns null
 */
AxResource.prototype.GetProperty = function(name)
{
    var count = this.properties.count;
    for (var i = 0; i < count; i++)
    {
        var property = this.properties.Get(i);
        if (property.name.Equals(name))
            return property;
    }

    return null;
};

/**
 * Serializes a parameter into a stream. In effect, it writes the parameter's type and value
 * @param {AxParameter} parameter The parameter to serialize
 * @param {AxStream} destination The stream in which the serialized parameter will be written
 */
AxResource.prototype.SerializeParameter = function(parameter, destination)
{
    destination.WriteInt32(parameter.type);
    
    if (parameter.IsReference())
    {
        var reference = parameter.value;
        if (reference !== 0)
            destination.WriteInt64(reference.id);
        else
            destination.WriteInt64(-1);
    }
    else
    {
        switch (parameter.type)
        {
            case AxParameterType.Int:
            {
                destination.WriteInt32(parameter.value);
                break;
            }

            case AxParameterType.Float:
            {
                destination.WriteFloat32(parameter.value);
                break;
            }

            case AxParameterType.Bool:
            {
                destination.WriteBool(parameter.value);
                break;
            }

            case AxParameterType.Color:
            {
                destination.WriteFloat32(parameter.value.x);
                destination.WriteFloat32(parameter.value.y);
                destination.WriteFloat32(parameter.value.z);
                destination.WriteFloat32(parameter.value.w);
                break;
            }
        }
    }
};

/**
 * Deserializes a parameter from a stream. In effect, it reads the parameter's type and value from a stream
 * @param {AxParameter} parameter The parameter which will be deserialized from the source data
 * @param {AxStream} source Stream with the serialized data
 */
AxResource.prototype.DeserializeParameter = function(parameter, source)
{
    var propertyType = source.ReadInt32();

    if (AxParameter.IsReference(propertyType))
    {
        var linkId = source.ReadInt64();

        var link = new AxResource();
        link.resourceType = AxResourceType.None;
        link.id = linkId;
        parameter.SetValue(link, propertyType);
    }
    else
    {
        switch (propertyType)
        {
            case AxParameterType.Int:
            {
                parameter.SetInt(source.ReadInt32());
                break;
            }

            case AxParameterType.Float:
            {
                parameter.SetFloat(source.ReadFloat32());
                break;
            }

            case AxParameterType.Bool:
            {
                parameter.SetBool(source.ReadBool());
                break;
            }

            case AxParameterType.Color:
            {
                var color = new AxVector4() ;
                color.x = source.ReadFloat32();
                color.y = source.ReadFloat32();
                color.z = source.ReadFloat32();
                color.w = source.ReadFloat32();
                parameter.SetColor(color);
                break;
            }
        }
    }
};

/**
 * Serializes the whole resource.
 * This method relies on SerializeChunks() to be properly overridden for the particular inherited resource type.
 * It calls SerializeChunks(), wrapping its whole output in a SerializationId_Resource
 * Can be overriden to perform pre or post serialization routines, but for child resources, serialization itself is meant to be customized through the SerializeChunks method
 * @param {AxStream} destination The stream into which to write the serialized data
 */
AxResource.prototype.Serialize = function(destination)
{
    var writer = new AxHierarchyStreamWriter(destination);

    writer.BeginChunk(AxResource.SerializationId_Resource);
    this.SerializeChunks(writer);
    writer.EndChunk();
};

/**
 * Deserialized a whole resource
 * This method relies on DeserializeChunk() to be properly overriden for the particular inherited resource type.
 * It reads the contents wrapped in a SerializationId_Resource and calls DeserializeChunk() method for each chunk encountered
 * Can be overriden to perform pre or post deserialization routines, but for child resources, deserialization itself is meant to be customized through the DeserializeChunk method
 * @param {AxStream} source The stream holding the serialized data
 * @return {Boolean} True if the deserialization was successfull
 */
AxResource.prototype.Deserialize = function(source)
{
    var reader = new AxHierarchyStreamReader(source);

    var mainChunkEnd = -1;

    while (reader.ReadChunkHeader())
    {
        switch (reader.chunkId)
        {
            case AxResource.SerializationId_Resource:
            {
                mainChunkEnd = reader.stream.position + reader.chunkSize;
                break;
            }

            default:
            {
                if ((reader.chunkSize < 0) || (reader.stream.position + reader.chunkSize > mainChunkEnd))
                    return false;

                if (!this.DeserializeChunk(reader))
                    reader.SkipChunk();

                break;
            }
        }

        if (reader.stream.position >= mainChunkEnd)
            return true;
    }

    return true;
};

/**
 * Writes chunks for all the data which is needed to serialize the resource. 
 * Can be overridden by child resources to serialize their implementation-specific data
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxResource.prototype.SerializeChunks = function(writer)
{
    writer.BeginChunk(AxResource.SerializationId_ResourceId);
    writer.stream.WriteInt64(this.id);
    writer.EndChunk();

    writer.BeginChunk(AxResource.SerializationId_ResourceName);
    writer.stream.WriteString(this.name);
    writer.EndChunk();

    writer.BeginChunk(AxResource.SerializationId_Properties);

    for (var i = 0; i < this.properties.count; i++)
    {
        writer.BeginChunk(AxResource.SerializationId_Property);

        writer.stream.WriteString(this.properties[i].name);

        this.SerializeParameter(this.properties[i], writer.stream);

        writer.EndChunk();
        // Property
    }

    writer.EndChunk();
    // Properties
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the resource's chunks. Override to read chunks of resource-specific data
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxResource.prototype.DeserializeChunk = function(reader)
{
    switch (reader.chunkId)
    {
        case AxResource.SerializationId_ResourceId:
        {
            this.id = reader.stream.ReadInt64();
            break;
        }

        case AxResource.SerializationId_ResourceName:
        {
            this.name = reader.stream.ReadString();
            break;
        }

        case AxResource.SerializationId_Properties:
        {
            break;
        }

        case AxResource.SerializationId_Property:
        {
            var propertyName = reader.stream.ReadString();
            var currentProperty = this.GetProperty(propertyName);
            if (currentProperty === null)
                currentProperty = this.properties.Add(new AxProperty(propertyName, AxParameterType.None, 0));

            this.DeserializeParameter(currentProperty, reader.stream);

            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};