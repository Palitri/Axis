/**
 * Creates a new KeyFrame object for rotation around an arbitrary axis
 * @param {!Number} time The time at which the keyframe is hit
 * @param {!Number} angle The angle at which to rotate at the time of the keyframe
 * @param {!AxVector3} axis The axis around which to rotate
 * @constructor
 */
function AxKeyFrameRotationAxis(time, angle, axis)
{
    AxKeyFrame.call(this, time);

    this.typeId = AxKeyFrameRotationAxis.classId;
	
    this.previousTransform = new AxMatrix();
    this.axis = new AxVector3();
    this.angle = 0.0;
    
    if (!AxUtils.IsUndefinedOrNull(axis))
        this.axis = axis;

    if (!AxUtils.IsUndefinedOrNull(angle))
        this.angle = angle;
}

AxKeyFrameRotationAxis.prototype = Object.create(AxKeyFrame.prototype);



AxKeyFrameRotationAxis.classId = 0x21111418;



AxKeyFrameRotationAxis.prototype.Dispose = function()
{
};

/**
 * Gets the resulting transform of the key frame
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 */
AxKeyFrameRotationAxis.prototype.Process = function(result)
{
    AxMatrix.Copy(result, this.previousTransform);
};

/**
 * Gets a transform, which is the result of the keyframe, blended with another one, with blending factor given by blend. Normally the another key frame is of the same object and blend is in the range between 0 and 1
 * @param {AxMatrix} result The transform resulting from processing the keyframe
 * @param {AxKeyFrameRotationAxis} next The next keyframe towards which to blend
 * @param {Number} blend The blending factor ranging from 0 to 1, where 0 is the currnet keyframe and 1 is the next keyframe
 */
AxKeyFrameRotationAxis.prototype.Process_2 = function(result, next, blend)
{
    var rotationMatrix = new AxMatrix(AxMatrix);
    AxMatrix.CreateRotationAxis_2(rotationMatrix, next.axis, next.angle * blend);
    AxMatrix.Multiply(result, this.previousTransform, rotationMatrix);
};

/**
 * Serialized the keyframe oject and writes the serialized data into the given stream
 * @param {AxStream} destination The stream where the serialized data should be written
 */
AxKeyFrameRotationAxis.prototype.Serialize = function(destination)
{
    AxKeyFrame.prototype.Serialize.call(this, destination);

    destination.WriteFloat32(this.axis.x);
    destination.WriteFloat32(this.axis.y);
    destination.WriteFloat32(this.axis.z);

    destination.WriteFloat32(this.angle);
};

/**
 * Deserializes the keyframe object from the given stream
 * @param {AxStream} source The shtream which contains the serialized data
 * @return {Boolean} True if deserialization has succeeded
 */
AxKeyFrameRotationAxis.prototype.Deserialize = function(source)
{
    if (!AxKeyFrame.prototype.Deserialize.call(this, source))
        return false;

    this.axis.x = source.ReadFloat32();
    this.axis.y = source.ReadFloat32();
    this.axis.z = source.ReadFloat32();

    this.angle = source.ReadFloat32();

    return true;
};