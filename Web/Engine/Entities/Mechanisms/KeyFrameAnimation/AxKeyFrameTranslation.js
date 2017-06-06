/**
 * Creates a new KeyFrame object for translation
 * @param {!Number} time The time at which the keyframe is hit
 * @param {!AxVector3} position The vector representing the position at the time of the keyframe
 * @constructor
 */
function AxKeyFrameTranslation(time, position)
{
    AxKeyFrame.call(this, time);

    this.typeId = AxKeyFrameTranslation.classId;

    this.position = new AxVector3();

    if (!AxUtils.IsUndefinedOrNull(position))
        this.position = position;
}

AxKeyFrameTranslation.prototype = Object.create(AxKeyFrame.prototype);



AxKeyFrameTranslation.classId = 0x21111518;



AxKeyFrameTranslation.prototype.Dispose = function()
{
};

/**
 * Gets the resulting transform of the key frame
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 */
AxKeyFrameTranslation.prototype.Process = function(result)
{
    AxMatrix.CreateTranslation_2(result, this.position);
};

/**
 * Gets a transform, which is the result of the keyframe, blended with another one, with blending factor given by blend. Normally the another key frame is of the same object and blend is in the range between 0 and 1
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 * @param {AxKeyFrameTranslation} next The next keyframe towards which to blend
 * @param {Number} blend The blending factor ranging from 0 to 1, where 0 is the currnet keyframe and 1 is the next keyframe
 */
AxKeyFrameTranslation.prototype.Process_2 = function(result, next, blend)
{
    var v = new AxVector3();
    AxVector3.Lerp(v, this.position, next.position, blend);
    AxMatrix.CreateTranslation_2(result, v);
};

/**
 * Serialized the keyframe oject and writes the serialized data into the given stream
 * @param {AxStream} destination The stream where the serialized data should be written
 */
AxKeyFrameTranslation.prototype.Serialize = function(destination)
{
    AxKeyFrame.prototype.Serialize.call(this, destination);

    destination.WriteFloat32(this.position.x);
    destination.WriteFloat32(this.position.y);
    destination.WriteFloat32(this.position.z);
};

/**
 * Deserializes the keyframe object from the given stream
 * @param {AxStream} source The shtream which contains the serialized data
 * @return {Boolean} True if deserialization has succeeded
 */
AxKeyFrameTranslation.prototype.Deserialize = function(source)
{
    if (!AxKeyFrame.prototype.Deserialize.call(this, source))
        return false;

    this.position.x = source.ReadFloat32();
    this.position.y = source.ReadFloat32();
    this.position.z = source.ReadFloat32();

    return true;
};
