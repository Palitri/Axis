function AxKeyframeAnimationMechanism(context)
{
    AxMechanism.call(this);
    
    this.typeId = AxKeyframeAnimationMechanism.classId;


    this.context = context;
    this.keyFramesTracks = new AxList();
    this.currentDeserializationTrack = null;

    this.parameter.SetType(AxParameterType.None);

    this.properties.Add(new AxProperty(new AxString('Frame'), 0.0));
    this.properties.Add(new AxProperty(new AxString('Transform'), null, AxParameterType.ReferenceTransform));
}

AxKeyframeAnimationMechanism.prototype = Object.create(AxMechanism.prototype);



AxKeyframeAnimationMechanism.classId = AxMechanism.classId | 107;

AxKeyframeAnimationMechanism.propertyIndex_Frame        = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxKeyframeAnimationMechanism.propertyIndex_Transform    = AxResource.propertyIndex_ChildPropertiesIndex + 1;



AxKeyframeAnimationMechanism.SerializationId_KeyFrameTracks		= 0x21111018;
AxKeyframeAnimationMechanism.SerializationId_KeyFrameTrack		= 0x21111118;
AxKeyframeAnimationMechanism.SerializationId_KeyFrameTrackFrames	= 0x21111128;



AxKeyframeAnimationMechanism.prototype.Dispose = function()
{
    for (var i = 0; i < this.keyFramesTracks.count; i++)
        this.keyFramesTracks.Set(i, null);
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxKeyframeAnimationMechanism.prototype.Process = function(deltaTime)
{
    var time = this.properties.Get(AxKeyframeAnimationMechanism.propertyIndex_Frame).GetEffectiveParameter().value;
    var transform = this.properties.Get(AxKeyframeAnimationMechanism.propertyIndex_Transform).GetEffectiveParameter().value;
    
    if (this.keyFramesTracks.count === 0)
    {
        return true;
    }

    this.keyFramesTracks.Get(0).GetTransform(transform.transform, time);

    var m = new AxMatrix();
    for (var i = 0; i < this.keyFramesTracks.count; i++)
    {
        this.keyFramesTracks.Get(i).GetTransform(m, time);
        AxMatrix.Multiply(transform.transform, transform.transform, m);
    }

    return true;
};

/**
 * Private
 * Because each of the rotation axis track's keyframes is based upon the accumulated transformation of the previous ones, this method precalculates these accumulated transformations for each track, so that when acquiring a transformation at a specific time, the precalculated values can be used, instead of calculating multiple transformations each time
 * @param {AxKeyFramesTrack} rotationAxisTrack A AxKeyFramesTrack of AxKeyFrameRotationAxis objects, for which to precalculate the transforms for each keyframe
 */
AxKeyframeAnimationMechanism.prototype.BuildRotationAxisTrack = function(rotationAxisTrack)
{
    var transform = new AxMatrix();
    var rotationMatrix = new AxMatrix();

    AxMatrix.CreateIdentity(transform);
    var count = rotationAxisTrack.count;
    for (var i = 0; i < count; i++)
    {
        AxMatrix.CreateRotationAxis_2(rotationMatrix, rotationAxisTrack.Get(i).axis, rotationAxisTrack.Get(i).angle);
        AxMatrix.Multiply(transform, transform, rotationMatrix);
        AxMatrix.Copy(rotationAxisTrack.Get(i).previousTransform, transform);
    }
};

/**
 * Writes chunks for all the data which is needed to serialize the resource. 
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxKeyframeAnimationMechanism.prototype.SerializeChunks = function(writer)
{
    AxMechanism.prototype.SerializeChunks.call(this, writer);

    writer.BeginChunk(AxKeyframeAnimationMechanism.SerializationId_KeyFrameTracks);

    for (var trackIndex = 0; trackIndex < this.keyFramesTracks.count; trackIndex++)
    {
        writer.BeginChunk(AxKeyframeAnimationMechanism.SerializationId_KeyFrameTrack);

        var keyFramesCount = this.keyFramesTracks.Get(trackIndex).count;
        if (keyFramesCount > 0)
        {
            var typeId = this.keyFramesTracks.Get(trackIndex).Get(0).typeId;
            writer.BeginChunk(AxKeyframeAnimationMechanism.SerializationId_KeyFrameTrackFrames);
            writer.stream.WriteInt32(typeId);

            for (var keyFrameIndex = 0; keyFrameIndex < keyFramesCount; keyFrameIndex++)
            {
                if (this.keyFramesTracks.Get(trackIndex).Get(keyFrameIndex).typeId !== typeId)
                {
                    writer.EndChunk();
                    writer.BeginChunk(AxKeyframeAnimationMechanism.SerializationId_KeyFrameTrackFrames);
                    writer.stream.WriteInt32(typeId);

                    typeId = this.keyFramesTracks.Get(trackIndex).Get(keyFrameIndex).typeId;
                }

                this.keyFramesTracks.Get(trackIndex).Get(keyFrameIndex).Serialize(writer.stream);
            }

            writer.EndChunk();
            // SerializationId_KeyFrameTrackFrames
        }

        writer.EndChunk();
        // SerializationId_KeyFrameTrack
    }

    writer.EndChunk();
    // SerializationId_KeyFrameTracks
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the resource's chunks. Override to read chunks of resource-specific data
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxKeyframeAnimationMechanism.prototype.DeserializeChunk = function(reader)
{
    if (AxMechanism.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxKeyframeAnimationMechanism.SerializationId_KeyFrameTracks:
        {
            break;
        }

        case AxKeyframeAnimationMechanism.SerializationId_KeyFrameTrack:
        {
            this.currentDeserializationTrack = this.keyFramesTracks.Add(new AxKeyFramesTrack());
            break;
        }

        case AxKeyframeAnimationMechanism.SerializationId_KeyFrameTrackFrames:
        {
            var chunkEnd = reader.stream.position + reader.chunkSize;

            var typeId = reader.stream.ReadInt32();

            while (reader.stream.position < chunkEnd)
            {
                var keyFrame = this.context.CreateEntity(typeId);
                if (keyFrame === null)
                {
                    reader.SkipChunk();
                    break;
                }

                keyFrame.Deserialize(reader.stream);
                this.currentDeserializationTrack.Add(keyFrame);
            }

            if (typeId === AxKeyFrameRotationAxis.classId)
                this.BuildRotationAxisTrack(this.currentDeserializationTrack);

            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};
