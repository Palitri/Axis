/**
 * Creates a axis native scene reader object.
 * @param {Axis} context Context to use for the reading. All deserialization will be performed using this context
 * @constructor
 */
function AxNativeFileReader(context)
{
    this.context = context;
    
    this.idMap = new AxDictionary();
}

/**
 * Reads native scene data in Axis' native scene format from a given stream
 * @param {AxStream} source Source stream to read data from
 * @return {Boolean} True if the deserialization was successfull
 */
AxNativeFileReader.prototype.Read = function(source)
{
    var reader = new AxHierarchyStreamReader(source);

    var mainChunkEnd = -1;

    while (reader.ReadChunkHeader())
    {
        switch (reader.chunkId)
        {
            case AxNativeSceneId.Main:
            {
                mainChunkEnd = reader.stream.position + reader.chunkSize;
                break;
            }

            case AxNativeSceneId.Resources:
            {
                break;
            }

            case AxNativeSceneId.ResourceSerializationWrap:
            {
                var resource = this.context.DeserializeResource(reader.stream);
                if (resource !== null)
                {
                    if (resource.typeId === AxSettings.classId)
                    {
                        this.context.settings = resource;                        
                    }
                    else
                    {
                        var originalId = resource.id;
                        this.context.AddResource(resource);
                        this.idMap.Add(originalId, resource.id);
                    }
                }

                break;
            }

            case AxNativeSceneId.RootBranch:
            {
                var rootId = reader.stream.ReadInt64();
                var rootResource = this.context.FindResource_1(this.GetMappedResourceId(rootId));

                if (rootResource !== null)
                {
                    if (rootResource.resourceType === AxResourceType.EntitySet)
                    {
                        root = rootResource;

                        if (this.context.root !== null)
                        {
                            for (var i = 0; i < root.references.count; i++)
                                this.context.root.references.Add(root.references.Get(i));

                            this.context.RemoveResource(root);
                        }
                        else
                        {
                            this.context.root = root;
                        }
                    }
                }

                break;
            }

            case AxNativeSceneId.RuntimeParameters:
            {
                break;
            }
            
            default:
            {
                if ((reader.chunkSize < 0) || (reader.stream.position + reader.chunkSize > mainChunkEnd))
                    return false;

                reader.SkipChunk();

                break;
            }
        }

        if (reader.stream.position >= mainChunkEnd)
            break;
    }

    this.LinkResources();

    return true;
};

/**
 * Private
 * Links temporary resource references made while deserializing and reconstructing the scene
 * This is a helper method and is meant to be called only in a specific context
 */
AxNativeFileReader.prototype.LinkResources = function()
{
    var resourceCount = this.context.resources.count;
    for (var resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++)
    {
        var resource = this.context.resources.Get(resourceIndex);

        var numProps = resource.properties.count;
        for (var prop = 0; prop < numProps; prop++)
        {
            var parameter = resource.properties.Get(prop);
            if (parameter.IsReference() && (parameter.value !== null))
            {
                var reference = parameter.value;
                if (reference.resourceType === AxResourceType.None)
                {
                    var ref = this.context.FindResource_1(this.GetMappedResourceId(reference.id));
                    parameter.value = ref;
                }
            }
        }

        if (resource.resourceType === AxResourceType.EntitySet)
        {
            var numReferences = resource.references.count;
            for (var i = 0; i < numReferences; i++)
            {
                var reference = resource.references.Get(i);
                if (reference.resourceType === AxResourceType.None)
                {
                    var ref = this.context.FindResource_1(this.GetMappedResourceId(reference.id));
                    resource.references.Set(i, ref);
                }
            }
        }
    }
};

/**
 * Private
 * Gets the ID of a temporary resource reference, which was made while deserializing and reconstructing the scene
 * This is a helper method and is meant to be called only in a specific context
 * @param {Integer} originalId description
 * @return {Integer} description
 */
AxNativeFileReader.prototype.GetMappedResourceId = function(originalId)
{
    var newId = new AxReferenceParameter();
    if (this.idMap.GetValue(originalId, newId))
        return newId.value;
    else
        return -1;
};
