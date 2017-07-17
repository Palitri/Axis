/**
 * Creates a new linear mechanism object
 * Linear mechanism interpolates between two boundaries and can loop continuously
 * For example a looping linear mechanism can produce a sawtooth function, while a looping and oscillating will produce a triangular function
 * @param {Number} min The left boundary for the interpolation
 * @param {Number} max The right boundary for the interpolation
 * @param {Number} speed The speed in units per second at which the interpolation is performed or the time in seconds for which the full interpolation is performed. The "timed" parameter determines whether it's a speed or time based.
 * @param {Boolean} timed Determines whether the speed value is measured in units per second or in total seconds for the interpolation to run from start to end
 * @param {Boolean} loop Determines whether the mechanism should loop
 * @param {Boolean} oscillate Determines whether the mechanism should oscillate to and fro and boundaries, producing a triangular function, rather than a sawtooth function
 * @constructor
 */
function AxLinearMechanism(min, max, speed, timed, loop, oscillate)
{
    AxMechanism.call(this);

    this.typeId = AxLinearMechanism.classId;

    this.parameter.SetFloat(min);

    this.properties.Add(new AxProperty(new AxString("Min"), min));
    this.properties.Add(new AxProperty(new AxString("Max"), max));
    this.properties.Add(new AxProperty(new AxString("Speed"), speed));
    this.properties.Add(new AxProperty(new AxString("Timed"), timed));
    this.properties.Add(new AxProperty(new AxString("Loop"), loop));
    this.properties.Add(new AxProperty(new AxString("Oscilate"), oscillate));

    this.phase = min;
}

AxLinearMechanism.prototype = Object.create(AxMechanism.prototype);


AxLinearMechanism.classId = AxMechanism.classId | 103;

AxLinearMechanism.propertyIndex_Min         = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxLinearMechanism.propertyIndex_Max         = AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxLinearMechanism.propertyIndex_Speed       = AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxLinearMechanism.propertyIndex_Timed       = AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxLinearMechanism.propertyIndex_Loop        = AxResource.propertyIndex_ChildPropertiesIndex + 4;
AxLinearMechanism.propertyIndex_Oscillate   = AxResource.propertyIndex_ChildPropertiesIndex + 5;


AxLinearMechanism.SerializationId_Phase	= 0x21111038;




AxLinearMechanism.prototype.Dispose = function()
{
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxLinearMechanism.prototype.Process = function(deltaTime)
{
    var min = this.properties.Get(AxLinearMechanism.propertyIndex_Min).GetEffectiveParameter().value;
    var max = this.properties.Get(AxLinearMechanism.propertyIndex_Max).GetEffectiveParameter().value;
    var speed = this.properties.Get(AxLinearMechanism.propertyIndex_Speed).GetEffectiveParameter().value;
    var timed = this.properties.Get(AxLinearMechanism.propertyIndex_Timed).GetEffectiveParameter().value;
    var loop = this.properties.Get(AxLinearMechanism.propertyIndex_Loop).GetEffectiveParameter().value;
    var oscillate = this.properties.Get(AxLinearMechanism.propertyIndex_Oscillate).GetEffectiveParameter().value;

    var interval = max - min;
    var deltaPhase = timed ? (deltaTime / speed) : (deltaTime * speed / interval);
    var phaseMax = oscillate ? 2.0 : 1.0;

    this.phase += deltaPhase;

    var result = true;
    if (this.phase > phaseMax)
    {
        if (loop)
            this.phase -= AxMath.Trunc(this.phase);
        else
        {
            this.phase = phaseMax;
            result = false;
        }
    }

    else if (this.phase < 0.0)
    {
        if (loop)
            this.phase += phaseMax - AxMath.Trunc(this.phase);
        else
        {
            this.phase = 0.0;
            result = false;
        }
    }

    var oscillationPhase = 1.0 - AxMath.Abs(this.phase - 1.0);
    this.parameter.value = min + (max - min) * oscillationPhase;

    return result;
};

/**
 * Writes chunks for all the data which is needed to serialize the resource.
 * Can be overridden by child resources to serialize their implementation-specific data
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxLinearMechanism.prototype.SerializeChunks = function(writer)
{
    writer.BeginChunk(AxLinearMechanism.SerializationId_Phase);
    writer.stream.WriteFloat32(this.phase);
    writer.EndChunk();

    AxMechanism.prototype.SerializeChunks.call(this, writer);
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the resource's chunks. Override to read chunks of resource-specific data
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxLinearMechanism.prototype.DeserializeChunk = function(reader)
{
    if (AxMechanism.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxLinearMechanism.SerializationId_Phase:
        {
            this.phase = reader.stream.ReadFloat32();
            break;
        }
    }
};