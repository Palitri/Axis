/**
 * Represents a timeline key, consisting of a time and a value parameter.
 * @param {type} time The key's position on the timeline
 * @param {type} value The value at the key's position on the timeline
 * @constructor
 */
function AxTimelineAnimationKey(time, value)
{
    if (AxUtils.IsUndefinedOrNull(time))
        time = 0.0;
    
    if (AxUtils.IsUndefinedOrNull(value))
        value = 0.0;
    
    this.time = time;
    this.value = value;
};

/**
 * Creates a new timeline animation mechanism.
 * This mechanism is used to produce values, interpolating between key points, which consist of a time and value
 * @constructor
 */
function AxTimelineAnimationMechanism()
{
    AxMechanism.call(this);

    this.typeId = AxTimelineAnimationMechanism.classId;

	this.parameter.SetFloat(0.0);

	this.properties.Add(new AxProperty(new AxString("Frame"), 0.0));
    
    this.keys = new AxList();
}

AxTimelineAnimationMechanism.prototype = Object.create(AxMechanism.prototype);

AxTimelineAnimationMechanism.classId = AxMechanism.classId | 112;

AxTimelineAnimationMechanism.propertyIndex_Frame    = AxResource.propertyIndex_ChildPropertiesIndex + 0;

AxTimelineAnimationMechanism.SerializationId_TimelineTrack  = 0x21111028;




AxTimelineAnimationMechanism.prototype.Dispose = function()
{
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxTimelineAnimationMechanism.prototype.Process = function(deltaTime)
{
	var time = this.properties.Get(AxTimelineAnimationMechanism.propertyIndex_Frame).GetEffectiveValue();

	var count = this.keys.count;
	
	// No keyframes
	if (count === 0)
	{
		this.parameter.value = 0.0;
		return true;
	}

	// Only one keyframe
	else if (count === 1)
	{
		this.parameter.value = this.keys.Get(0).value;
		return true;
	}

	// Time outside of left boundary
	else if (time <= this.keys.Get(0).time)
	{
		this.parameter.value = this.keys.Get(0).value;
		return true;
	}

	// Time outside of right boundary
	else if (time >= this.keys.Get(count - 1).time)
	{
		this.parameter.value = this.keys.Get(count - 1).value;
		return true;
	}

	// Search - a minimum of 2 key frames are required
	else
	{
		var left = 0;
		var right = count - 2;

		var index;
		var startTime, endTime;

		do
		{
			index = left + AxMath.Floor((right - left) / 2);
			startTime = this.keys.Get(index).time;
			endTime = this.keys.Get(index + 1).time;

			if (time > endTime)
				left = index + 1;
			else if (time < startTime)
				right = index - 1;
			else
				break;
		}
		while (left <= right);

		var blend = (time - startTime) / (endTime - startTime);

		var startValue = this.keys.Get(index).value;
		var endValue = this.keys.Get(index + 1).value;
        
        this.parameter.value = startValue + (endValue - startValue) * blend;

		return true;
	}

	return false;
};


/**
 * Writes chunks for all the data which is needed to serialize the resource. 
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxTimelineAnimationMechanism.prototype.SerializeChunks = function(writer)
{
    AxMechanism.prototype.SerializeChunks.call(this, writer);

	if (this.keys.count > 0)
	{
		writer.BeginChunk(AxTimelineAnimationMechanism.SerializationId_TimelineTrack);
		writer.stream.WriteInt32(this.keys.count);

		for (var keyIndex = 0; keyIndex < this.keys.count; keyIndex++)
		{
			writer.stream.WriteFloat32(this.keys.Get(keyIndex).time);
			writer.stream.WriteFloat32(this.keys.Get(keyIndex).value);
		}

		writer.EndChunk();
	}
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the resource's chunks. Override to read chunks of resource-specific data
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxTimelineAnimationMechanism.prototype.DeserializeChunk = function(reader)
{
    if (AxMechanism.prototype.DeserializeChunk.call(this, reader))
        return true;

	switch (reader.chunkId)
	{
		case AxTimelineAnimationMechanism.SerializationId_TimelineTrack:
		{
			var numKeys = reader.stream.ReadInt32();
			this.keys = new AxList();
			for (var keyIndex = 0; keyIndex < numKeys; keyIndex++)
			{
				var key = new AxTimelineAnimationKey();
                key.time = reader.stream.ReadFloat32();
				key.value = reader.stream.ReadFloat32();
                this.keys.Add(key);
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
