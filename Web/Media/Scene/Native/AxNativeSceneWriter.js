/**
 * Creates a axis native scene writer object.
 * @param {Axis} context Context to use for the writing. All serialization  will be performed using this context
 * @constructor
 */
function AxNativeSceneWriter(context)
{
    this.context = context;
}


/**
 * Writes the full contents of the context in Axis' native format
 * @param {AxStream} destination Stream in which to write
 */
AxNativeSceneWriter.prototype.Write = function(destination)
{
    var writer = new AxHierarchyStreamWriter(destination);
	
    writer.BeginChunk(AxNativeSceneId.Main);

    writer.BeginChunk(AxNativeSceneId.Header);

    writer.BeginChunk(AxNativeSceneId.Header_String);
    writer.stream.WriteString(AxString('Axis'));
    writer.EndChunk();

    writer.BeginChunk(AxNativeSceneId.Header_Ver);
    writer.stream.WriteUInt32(0x09000000);
    writer.EndChunk();

    writer.EndChunk();
    // Header

    writer.BeginChunk(AxNativeSceneId.Resources);

    for (var i = 0; i < this.context.resources.count; i++)
    {
        writer.BeginChunk(AxNativeSceneId.ResourceSerializationWrap);
        this.context.SerializeResource(destination, this.context.resources[i]);
        writer.EndChunk();
    }

    writer.EndChunk();
    // Resources

    writer.BeginChunk(AxNativeSceneId.RootBranch);
    writer.stream.WriteInt64(this.context.root.id);
    writer.EndChunk();


    writer.BeginChunk(AxNativeFileID.RuntimeParameters);
    writer.BeginChunk(AxNativeFileID.ResourceSerializationWrap);
    this.context.SerializeResource(destination, this.context.settings);
    writer.EndChunk();
    writer.EndChunk();
    // RuntimeParameters

    
    writer.EndChunk();
    // Main
};
