/**
 * Creates a new KeyFrame object for scaling
 * @param {!Number} time The time at which the keyframe is hit
 * @param {!AxVector3} scaling The vector representing the scaling on each axis at the time of the keyframe
 * @constructor
 */
function AxKeyFrameScaling(time, scaling)
{
    AxKeyFrame.call(this, time);

    this.typeId = AxKeyFrameScaling.classId;

    this.scaling = new AxVector3();

    if (!AxUtils.IsUndefinedOrNull(scaling))
        this.scaling = scaling;
}

AxKeyFrameScaling.prototype = Object.create(AxKeyFrame.prototype);



AxKeyFrameScaling.classId = 0x21111218;



AxKeyFrameScaling.prototype.Dispose = function()
{
};

/**
 * Gets the resulting transform of the key frame
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 */
AxKeyFrameScaling.prototype.Process = function(result)
{
    AxMatrix.CreateScaling_2(result, this.scaling);
};

/**
 * Gets a transform, which is the result of the keyframe, blended with another one, with blending factor given by blend. Normally the another key frame is of the same object and blend is in the range between 0 and 1
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 * @param {AxKeyFrameScaling} next The next keyframe towards which to blend
 * @param {Number} blend The blending factor ranging from 0 to 1, where 0 is the currnet keyframe and 1 is the next keyframe
 */
AxKeyFrameScaling.prototype.Process_2 = function(result, next, blend)
{
    var v = new AxVector3();
    AxVector3.Lerp(v, this.scaling, next.scaling, blend);
    AxMatrix.CreateScaling_2(result, v);
};

/**
 * Serialized the keyframe oject and writes the serialized data into the given stream
 * @param {AxStream} destination The stream where the serialized data should be written
 */
AxKeyFrameScaling.prototype.Serialize = function(destination)
{
    AxKeyFrame.prototype.Serialize.call(this, destination);

    destination.WriteFloat32(this.scaling.x);
    destination.WriteFloat32(this.scaling.y);
    destination.WriteFloat32(this.scaling.z);
};

/**
 * Deserializes the keyframe object from the given stream
 * @param {AxStream} source The shtream which contains the serialized data
 * @return {Boolean} True if deserialization has succeeded
 */
AxKeyFrameScaling.prototype.Deserialize = function(source)
{
    if (!AxKeyFrame.prototype.Deserialize.call(this, source))
        return false;

    this.scaling.x = source.ReadFloat32();
    this.scaling.y = source.ReadFloat32();
    this.scaling.z = source.ReadFloat32();

    return true;
};
