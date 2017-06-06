/**
 * Creates a new KeyFrame object for Euler rotation around each of the X, Y and Z axes
 * @param {!Number} time The time at which the keyframe is hit
 * @param {!AxVector3} eulerAngles The Euler angles to rotate at in the time of the keyframe
 * @constructor
 */
function AxKeyFrameRotationEuler(time, eulerAngles)
{
    AxKeyFrame.call(this, time);

    this.typeId = AxKeyFrameRotationEuler.classId;

    this.eulerAngles = new AxVector3();

    if (!AxUtils.IsUndefinedOrNull(eulerAngles))
        this.eulerAngles = eulerAngles;
}

AxKeyFrameRotationEuler.prototype = Object.create(AxKeyFrame.prototype);

AxKeyFrameRotationEuler.classId = 0x21111318;


AxKeyFrameRotationEuler.prototype.Dispose = function()
{
};

/**
 * Gets the resulting transform of the key frame
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 */
AxKeyFrameRotationEuler.prototype.Process = function(result)
{
    var m = new AxMatrix();
    AxMatrix.CreateRotationX(result, this.eulerAngles.x);
    AxMatrix.CreateRotationY(m, this.eulerAngles.y);
    AxMatrix.Multiply(result, result, m);
    AxMatrix.CreateRotationZ(m, this.eulerAngles.z);
    AxMatrix.Multiply(result, result, m);
};

/**
 * Gets a transform, which is the result of the keyframe, blended with another one, with blending factor given by blend. Normally the another key frame is of the same object and blend is in the range between 0 and 1
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 * @param {AxKeyFrameRotationEuler} next The next keyframe towards which to blend
 * @param {Number} blend The blending factor ranging from 0 to 1, where 0 is the currnet keyframe and 1 is the next keyframe
 */
AxKeyFrameRotationEuler.prototype.Process_2 = function(result, next, blend)
{
    var v = new AxVector3();
    AxVector3.Lerp(v, this.eulerAngles, next.eulerAngles, blend);

    var m = new AxMatrix();
    AxMatrix.CreateRotationX(result, v.x);
    AxMatrix.CreateRotationY(m, v.y);
    AxMatrix.Multiply(result, result, m);
    AxMatrix.CreateRotationZ(m, v.z);
    AxMatrix.Multiply(result, result, m);
};

/**
 * Serialized the keyframe oject and writes the serialized data into the given stream
 * @param {AxStream} destination The stream where the serialized data should be written
 */
AxKeyFrameRotationEuler.prototype.Serialize = function(destination)
{
    AxKeyFrame.prototype.Serialize.call(this, destination);

    destination.WriteFloat32(this.eulerAngles.x);
    destination.WriteFloat32(this.eulerAngles.y);
    destination.WriteFloat32(this.eulerAngles.z);
};

/**
 * Deserializes the keyframe object from the given stream
 * @param {AxStream} source The shtream which contains the serialized data
 * @return {Boolean} True if deserialization has succeeded
 */
AxKeyFrameRotationEuler.prototype.Deserialize = function(source)
{
    if (!AxKeyFrame.prototype.Deserialize.call(this, source))
        return false;

    this.eulerAngles.x = source.ReadFloat32();
    this.eulerAngles.y = source.ReadFloat32();
    this.eulerAngles.z = source.ReadFloat32();

    return true;
};
