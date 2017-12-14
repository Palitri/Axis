/**
 * Creates a new polynomial mechanism.
 * This mechanism yields the result of a polynomial function
 * @param {Integer} degree The degree of the polynomial
 * @constructor
 */
function AxPolynomialMechanism(degree)
{
    AxMechanism.call(this);

    this.typeId = AxPolynomialMechanism.classId;

    this.parameter.SetFloat(0.0);

    this.properties.Add(new AxProperty(new AxString("x"), 0.0));

    this.degree = -1;
    this.SetDegree(degree);

    if (this.degree >= 1)
        this.properties.Get(AxPolynomialMechanism.GetDegreePropertyIndex(1)).SetFloat(1.0);
}

AxPolynomialMechanism.prototype = Object.create(AxMechanism.prototype);


AxPolynomialMechanism.classId = AxMechanism.classId | 102;


AxPolynomialMechanism.SerializationId_Degree	= 0x21111028;


AxPolynomialMechanism.propertyIndex_X       = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxPolynomialMechanism.propertyIndex_Degree0 = AxResource.propertyIndex_ChildPropertiesIndex + 1;




AxPolynomialMechanism.prototype.Dispose = function()
{
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxPolynomialMechanism.prototype.Process = function(deltaTime)
{
    var variable =  this.properties.Get(AxPolynomialMechanism.propertyIndex_X).GetEffectiveParameter().value;
    var result = 0.0;

    for (var i = 0; i <= this.degree; i++)
        result += this.properties.Get(AxPolynomialMechanism.GetDegreePropertyIndex(i)).GetEffectiveValue() * AxMath.Power(variable, i);

    this.parameter.value = result;

    return true;
};

/**
 * Sets the degree of the polynomial
 * @param {Integer} degree the degree of the polynomial
 */
AxPolynomialMechanism.prototype.SetDegree = function(degree)
{
    // Reduce degree
    for (var i = degree + 1; i <= this.degree; i++)
    {
        var index = AxPolynomialMechanism.GetDegreePropertyIndex(i);
        delete this.properties.Get(index);
        this.properties.RemoveAt(index);
    }

    // Raise degree
    for (var i = this.degree + 1; i <= degree; i++)
        this.properties.Insert(AxPolynomialMechanism.GetDegreePropertyIndex(i), new AxProperty(new AxString("x" + i), 0.0));

    this.degree = degree;
};

/**
 * Writes chunks for all the data which is needed to serialize the resource.
 * Can be overridden by child resources to serialize their implementation-specific data
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxPolynomialMechanism.prototype.SerializeChunks = function(writer)
{
    writer.BeginChunk(AxPolynomialMechanism.SerializationId_Degree);
    writer.stream.WriteInt32(this.degree);
    writer.EndChunk();

    AxMechanism.prototype.SerializeChunks.call(this, writer);
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the resource's chunks. Override to read chunks of resource-specific data
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxPolynomialMechanism.prototype.DeserializeChunk = function(reader)
{
    if (AxMechanism.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxPolynomialMechanism.SerializationId_Degree:
        {
            this.SetDegree(reader.stream.ReadInt32());
            break;
        }
    }
};



// Static methods



/**
 * Gets the index of the property for the factor of the respective degree in the polynomial
 * @param {Integer} degree The degree of the polynomial for whose factor to get the property for
 * @returns {Integer} The index of the property for the factor of the respective degree in the polynomial
 */
AxPolynomialMechanism.GetDegreePropertyIndex = function(degree)
{
    return AxPolynomialMechanism.propertyIndex_Degree0 + degree;
};
