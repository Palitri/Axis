/**
 * @constructor
 */
function AxSerializationUtils()
{
}



// Static methods



/**
 * Serializes the given vector and writes the serialized data into the given stream
 * @param {AxStream} destinationStream Stream in which to write the serialized data
 * @param {AxVector2} vector The vector which is to be serialized
 */
AxSerializationUtils.SerializeVector2 = function(destinationStream, vector)
{
    destinationStream.WriteFloat32(vector.x);
    destinationStream.WriteFloat32(vector.y);
};

/**
 * Deserializes a vector from the given stream
 * @param {AxStream} sourceStream Stream with the serialized data
 * @return {AxVector2} The deserialized vector
 */
AxSerializationUtils.DeserializeVector2 = function(sourceStream)
{
    result = new AxVector2();
    result.x = sourceStream.ReadFloat32();
    result.y = sourceStream.ReadFloat32();
    return result;
};

/**
 * Serializes the given vector and writes the serialized data into the given stream
 * @param {AxStream} destinationStream Stream in which to write the serialized data
 * @param {AxVector3} vector The vector which is to be serialized
 */
AxSerializationUtils.SerializeVector3 = function(destinationStream, vector)
{
    destinationStream.WriteFloat32(vector.x);
    destinationStream.WriteFloat32(vector.y);
    destinationStream.WriteFloat32(vector.z);
};

/**
 * Deserializes a vector from the given stream
 * @param {AxStream} sourceStream Stream with the serialized data
 * @return {AxVector3} The deserialized vector
 */
AxSerializationUtils.DeserializeVector3 = function(sourceStream)
{
    result = new AxVector3();
    result.x = sourceStream.ReadFloat32();
    result.y = sourceStream.ReadFloat32();
    result.z = sourceStream.ReadFloat32();
    return result;
};

/**
 * Serializes the given vector and writes the serialized data into the given stream
 * @param {AxStream} destinationStream Stream in which to write the serialized data
 * @param {AxVector4} vector The vector which is to be serialized
 */
AxSerializationUtils.SerializeVector4 = function(destinationStream, vector)
{
    destinationStream.WriteFloat32(vector.x);
    destinationStream.WriteFloat32(vector.y);
    destinationStream.WriteFloat32(vector.z);
    destinationStream.WriteFloat32(vector.w);
};

/**
 * Deserializes a vector from the given stream
 * @param {AxStream} sourceStream Stream with the serialized data
 * @return {AxVector4} The deserialized vector
 */
AxSerializationUtils.DeserializeVector4 = function(sourceStream)
{
    result = new AxVector4();
    result.x = sourceStream.ReadFloat32();
    result.y = sourceStream.ReadFloat32();
    result.z = sourceStream.ReadFloat32();
    result.w = sourceStream.ReadFloat32();
    return result;
};

/**
 * Serializes the given matrix and writes the serialized data into the given stream
 * @param {AxStream} destinationStream Stream in which to write the serialized data
 * @param {AxMatrix} matrix The matrix which is to be serialized
 */
AxSerializationUtils.SerializeMatrix = function(destinationStream, matrix)
{
    destinationStream.WriteFloat32(matrix._11);
    destinationStream.WriteFloat32(matrix._12);
    destinationStream.WriteFloat32(matrix._13);
    destinationStream.WriteFloat32(matrix._14);
    destinationStream.WriteFloat32(matrix._21);
    destinationStream.WriteFloat32(matrix._22);
    destinationStream.WriteFloat32(matrix._23);
    destinationStream.WriteFloat32(matrix._24);
    destinationStream.WriteFloat32(matrix._31);
    destinationStream.WriteFloat32(matrix._32);
    destinationStream.WriteFloat32(matrix._33);
    destinationStream.WriteFloat32(matrix._34);
    destinationStream.WriteFloat32(matrix._41);
    destinationStream.WriteFloat32(matrix._42);
    destinationStream.WriteFloat32(matrix._43);
    destinationStream.WriteFloat32(matrix._44);
};

/**
 * Deserializes a matrix from the given stream
 * @param {AxStream} sourceStream Stream with the serialized data
 * @param {AxMatrix} result The deserialized matrix
 */
AxSerializationUtils.DeserializeMatrix = function(result, sourceStream)
{
    result._11 = sourceStream.ReadFloat32();
    result._12 = sourceStream.ReadFloat32();
    result._13 = sourceStream.ReadFloat32();
    result._14 = sourceStream.ReadFloat32();
    result._21 = sourceStream.ReadFloat32();
    result._22 = sourceStream.ReadFloat32();
    result._23 = sourceStream.ReadFloat32();
    result._24 = sourceStream.ReadFloat32();
    result._31 = sourceStream.ReadFloat32();
    result._32 = sourceStream.ReadFloat32();
    result._33 = sourceStream.ReadFloat32();
    result._34 = sourceStream.ReadFloat32();
    result._41 = sourceStream.ReadFloat32();
    result._42 = sourceStream.ReadFloat32();
    result._43 = sourceStream.ReadFloat32();
    result._44 = sourceStream.ReadFloat32();
};


/**
 * Serializes the given pixel format and writes the serialized data into the given stream
 * @param {AxStream} destinationStream Stream in which to write the serialized data
 * @param {AxPixelFormat} format The pixel format which is to be serialized
 */
AxSerializationUtils.SerializePixelFormat = function(destinationStream, format)
{
    destinationStream.WriteInt16(format.numChannels);
    destinationStream.WriteInt16(format.sizeInBytes);
    destinationStream.WriteData(format.channelId, format.numChannels);
    destinationStream.WriteData(format.channelBitCount, format.numChannels);
};

/**
 * Deserializes a pixel format from the given stream
 * @param {AxStream} sourceStream Stream with the serialized data
 * @return {AxPixelFormat} The deserialized pixel format
 */
AxSerializationUtils.DeserializePixelFormat = function(sourceStream)
{
    var result = new AxPixelFormat();

    result.numChannels = sourceStream.ReadInt16();
    result.sizeInBytes = sourceStream.ReadInt16();
    sourceStream.ReadData(result.channelId, result.numChannels);
    sourceStream.ReadData(result.channelBitCount, result.numChannels);

    return result;
};

