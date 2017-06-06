/**
 * Creates a new KeyFrame object
 * This is the base for the various keyframes for translation, rotation and etc.
 * @param {!Number} time The time at which the keyframe is hit
 * @constructor
 */
function AxKeyFrame(time)
{
    AxEntity.call(this);

    this.time = 0.0;
    
    if (!AxUtils.IsUndefinedOrNull(time))
        this.time = time;
}

AxKeyFrame.prototype = Object.create(AxEntity.prototype);


AxKeyFrame.prototype.Dispose = function()
{
};

/**
 * Gets the resulting transform of the key frame
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 */
AxKeyFrame.prototype.Process = function(result) { };

/**
 * Gets a transform, which is the result of the keyframe, blended with another one, with blending factor given by blend. Normally the another key frame is of the same object and blend is in the range between 0 and 1
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 * @param {AxKeyFrame} next The next keyframe towards which to blend
 * @param {Number} blend The blending factor ranging from 0 to 1, where 0 is the currnet keyframe and 1 is the next keyframe
 */
AxKeyFrame.prototype.Process_2 = function(result, next, blend) { };

/**
 * Serialized the keyframe oject and writes the serialized data into the given stream
 * @param {AxStream} destination The stream where the serialized data should be written
 */
AxKeyFrame.prototype.Serialize = function(destination)
{
    destination.WriteFloat32(this.time);
};

/**
 * Deserializes the keyframe object from the given stream
 * @param {AxStream} source The shtream which contains the serialized data
 * @return {Boolean} True if deserialization has succeeded
 */
AxKeyFrame.prototype.Deserialize = function(source)
{
    this.time = source.ReadFloat32();

    return true;
};