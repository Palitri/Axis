/**
 * Creates a vector object and initializes its components
 * Sets the components of the vector to the given values. Supports multiple input formats
 * Valid input formats are
 * () - Initializes all components to zero
 * (Number) - Sets all components to the value of the given number
 * (Number, Number, Number, Number) - Sets the x, y, z and w components to the values of the given numbers in the respective order
 * (AxVector2, Number, Number) - Sets the x and y components to the respective values from the given vector and the z and w components to the values of the given numbers in the respective order
 * (AxVector3, Number) - Sets the x, y and z components to the respective values from the given vector and the w component to the value of the given number
 * (AxVector4) - Copies the components from the given vector
 * @param {!Number|AxVector2|AxVector3|AxVector4} v1 For (number) input - the value to set to all components. For (x, y, z, w) input - the value for the X component of the vector. For (AxVector2, z, w) input - the AxVector2 which contains the x and y components. For (AxVector3, w) input - the AxVector3 which contains the x, y and z components. For (AxVector4) input - the vector to copy from
 * @param {!Number} v2 For (x, y, z, w) input - the value for the Y component. For (AxVector2, z, w) input - the value for the Z component. For (AxVector3, w) input - the value for the W component.
 * @param {!Number} v3 For (x, y, z, w) input - the value for the Z component. For (AxVector2, z, w) input - the value for the W component.
 * @param {!Number} v4 For (x, y, z, w) input - the value for the W component.
 * @constructor
 */
function AxVector4(v1, v2, v3, v4)
{
    if (AxUtils.IsUndefinedOrNull(v1))
    {
        this.x = 0.0;
        this.y = 0.0;
        this.z = 0.0;
        this.w = 0.0;
    }
    else
        this.Set(v1, v2, v3, v4);
}

/**
 * Sets the components of the vector to the given values. Supports multiple input formats
 * Valid input formats are
 * (Number) - Sets all components to the value of the given number
 * (Number, Number, Number, Number) - Sets the x, y, z and w components to the values of the given numbers in the respective order
 * (AxVector2, Number, Number) - Sets the x and y components to the respective values from the given vector and the z and w components to the values of the given numbers in the respective order
 * (AxVector3, Number) - Sets the x, y and z components to the respective values from the given vector and the w component to the value of the given number
 * (AxVector4) - Copies the components from the given vector
 * @param {Number|AxVector2|AxVector3|AxVector4} v1 For (number) input - the value to set to all components. For (x, y, z, w) input - the value for the X component of the vector. For (AxVector2, z, w) input - the AxVector2 which contains the x and y components. For (AxVector3, w) input - the AxVector3 which contains the x, y and z components. For (AxVector4) input - the vector to copy from
 * @param {!Number} v2 For (x, y, z, w) input - the value for the Y component. For (AxVector2, z, w) input - the value for the Z component. For (AxVector3, w) input - the value for the W component.
 * @param {!Number} v3 For (x, y, z, w) input - the value for the Z component. For (AxVector2, z, w) input - the value for the W component.
 * @param {!Number} v4 For (x, y, z, w) input - the value for the W component.
 */
AxVector4.prototype.Set = function(v1, v2, v3, v4)
{
    if (AxUtils.IsNumber(v4))
    {
        this.x = v1;
        this.y = v2;
        this.z = v3;
        this.w = v4;
    }
    else if (AxUtils.IsNumber(v3))
    {
        this.x = v1.x;
        this.y = v1.y;
        this.z = v2;
        this.w = v3;
    }
    else if (AxUtils.IsNumber(v2))
    {
        this.x = v1.x;
        this.y = v1.y;
        this.z = v1.z;
        this.w = v2;
    }
    else if (AxUtils.IsNumber(v1))
    {
        this.x = v1;
        this.y = v1;
        this.z = v1;
        this.w = v1;
    }
    else
    {
        this.x = v1.x;
        this.y = v1.y;
        this.z = v1.z;
        this.w = v1.w;
    }
};

/**
 * Sets the components of the vectors to the given value
 * @param {Number} value The value to set all the four component of the vector to
 */
AxVector4.prototype.Set_1 = function(value)
{
    this.x = value;
    this.y = value;
    this.z = value;
    this.w = value;
};

/**
 * Sets the components of the vectors to the given values
 * @param {Number} x The value for the X component of the vector
 * @param {Number} y The value for the Y component of the vector
 * @param {Number} z The value for the Z component of the vector
 * @param {Number} w The value for the W component of the vector
 */
AxVector4.prototype.Set_2 = function(x, y, z, w)
{
    this.x = x;
    this.y = y;
    this.z = z;
    this.w = w;
};

/**
 * Constructs the vector's X and Y components from a 2D vector and the Z and W components from values
 * @param {AxVector2} source A 2D vector to use as source for the X and Y components
 * @param {Number} z A value to use as source for the Z component
 * @param {Number} w A value to use as source for the W component
 */
AxVector4.prototype.Set_3 = function(source, z, w)
{
    this.x = source.x;
    this.y = source.y;
    this.z = z;
    this.w = w;
};

/**
 * Constructs the vector's X and Y components from a 3D vector and the W component from a value
 * @param {AxVector3} source A 3D vector to use as source for the X, Y and Z components
 * @param {Number} w A value to use as source for the W component
 */
AxVector4.prototype.Set_4 = function(source, w)
{
    this.x = source.x;
    this.y = source.y;
    this.z = source.z;
    this.w = w;
};

/**
 * Copies the source vector
 * @param {AxVector4} source A vector to copy from
 */
AxVector4.prototype.Set_5 = function(source)
{
    this.x = source.x;
    this.y = source.y;
    this.z = source.z;
    this.w = source.w;
};

/**
 * Compares the vector with another one and returns true if both are identical
 * @param {AxVector4} vector The vector to compare with
 * @returns {Boolean} True if identical to the given vector
 */
AxVector4.prototype.Equals = function(vector)
{
    return  (this.x === vector.x) &&
            (this.y === vector.y) &&
            (this.z === vector.z) &&
            (this.w === vector.w);
};

/**
 * Calculates the length of the vector
 * @return {Number} Returns the length of the vector
 */
AxVector4.prototype.GetLength = function()
{
    return AxMath.Sqrt(this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w);
};

/**
 * Return a vector with the same orientation, but of the given length
 * @param {Number} length Length of the returned vector
 * @returns {AxVector4} A vector with the same orientation as the original, but of the given length
 */
AxVector4.prototype.OfLength = function(length)
{
    var result = new AxVector4();
    AxVector4.SetLength(result, this, length);
    return result;
};

/**
 * Returns the vector normalized
 * @returns {AxVector4} A normalized version of the original vector
 */
AxVector4.prototype.Normalize = function()
{
    var result = new AxVector4();
    AxVector4.Normalize(result, this);
    return result;
};

/**
 * Returns the vector inverted
 * @returns {AxVector4} An inverted version of the original vector
 */
AxVector4.prototype.Invert = function()
{
    var result = new AxVector4();
    AxVector4.Invert(result, this);
    return result;
};

/**
 * Returns the distance between the point represented by the vector and a point represented by the given vector
 * @param {AxVector4} v A vector representing the point to which distance will be calculated
 * @returns {Number} The distance to the given point
 */
AxVector4.prototype.DistanceTo = function(v)
{
    return AxVector4.Distance(this, v);
};

/**
 * Returns a linearly vector interpolated between (or extrapolated outside of) the original and the given one
 * @param {AxVector4} v The vector towards which to interpolate (or extrapolate)
 * @param {Number} factor The interpolation (or extrapolation) value. Values between 0 and 1 result in interpolation, while values outside of this range result in extrapolation.
 * @returns {Number} A linearly interpolated (or extrapolated) vector
 */
AxVector4.prototype.Lerp = function(v, factor)
{
    var result = new AxVector4();
    AxVector4.Lerp(result, this, v, factor);
    return result;
};

/**
 * Returns a vetor which is the difference between the original vector and the given one
 * @param {AxVector4} v The vector, which is to be subtracted from the original one
 * @returns {AxVector4} The difference between the original vector and the given one
 */
AxVector4.prototype.Subtract = function(v)
{
    var result = new AxVector4();
    AxVector4.Subtract(result, this, v);
    return result;
};

/**
 * Returns a vetor which is the sum of the original vector and the given one
 * @param {AxVector4} v The vector, which is to be added to the original one
 * @returns {AxVector4} The sum of the original vector and the given one
 */
AxVector4.prototype.Add = function(v)
{
    var result = new AxVector4();
    AxVector4.Add(result, this, v);
    return result;
};

/**
 * Returns the dot product of the original vector and the given one
 * @param {AxVector4} v The vector to calculate dot product against
 * @returns {Number} The dot product of the original vector and the given one
 */
AxVector4.prototype.Dot = function(v)
{
    return AxVector4.Dot(this, v);
};

/**
 * Returns the cross product of the original vector and the given one
 * @param {AxVector4} v1 The first vector to calculate cross product against
 * @param {AxVector4} v2 The second vector to calculate cross product against
 * @returns {AxVector4} The cross product of the original vector and the given one
 */
AxVector4.prototype.Cross = function(v1, v2)
{
    var result = new AxVector4();
    AxVector4.Cross(result, this, v1, v2);
    return result;
};

/**
 * Returns the vector with each of its components multiplied by the components of the given vector
 * @param {AxVector4} v The vector by which compoenents to multiply the components of the original one
 * @returns {AxVector4} The component-wise scaled vector
 */
AxVector4.prototype.Scale = function(v)
{
    var result = new AxVector4();
    AxVector4.Scale(result, this, v);
    return result;
};

/**
 * Returns the vector with each of its components multiplied by the given value
 * @param {Number} factor The value by which to multiply the components of the original vector
 * @returns {AxVector4} The component-wise scaled vector
 */
AxVector4.prototype.Scale = function(factor)
{
    var result = new AxVector4();
    AxVector4.Scale(result, this, factor);
    return result;
};

/**
 * Returns a vector with each of its components being the lesser between their original value and the value of the corresponding component of the given vector.
 * In effect, the result is a component-wise minimum vector
 * @param {AxVector4} v The vector, whose components to compare against
 * @returns {AxVector4} A component-wise minimum vector
 */
AxVector4.prototype.Min = function(v)
{
    var result = new AxVector4();
    AxVector4.Min(result, this, v);
    return result;
};

/**
 * Returns a vector with each of its components being the greater between their original value and the value of the corresponding component of the given vector.
 * In effect, the result is a component-wise maximum vector
 * @param {AxVector4} v The vector, whose components to compare against
 * @returns {AxVector4} A component-wise maximum vector
 */
AxVector4.prototype.Max = function(v)
{
    var result = new AxVector4();
    AxVector4.Max(result, this, v);
    return result;
};

/**
 * Returns a vector which is the original one, transformed by the given transformation matrix
 * @param {AxMatrix} transformation The transformation matrix to apply
 * @returns {AxVector4} The transformed vector
 */
AxVector4.prototype.Transform = function(transformation)
{
    var result = new AxVector4();
    AxVector4.Transform(result, this, transformation);
    return result;
};



// Static Methods



/**
 * Copies the source vector into the result
 * @param {AxVector4} result The vector to copy to
 * @param {AxVector4} source The vector to copy from
 */
AxVector4.Copy = function(result, source)
{
    result.x = source.x;
    result.y = source.y;
    result.z = source.z;
    result.w = source.w;
};

/**
 * Produces a vector with an orientation given by another vector and a given length
 * @param {AxVector4} result The resulting vector
 * @param {AxVector4} v The vector which holds the orientation
 * @param {Number} length The length value for the new vector
 */
AxVector4.SetLength = function(result, v, length)
{
    var k = AxMath.Sqrt(v.x * v.x + v.y * v.y +	v.z * v.z + v.w * v.w);
    if (k !== 0.0)
    {
        k = length / k;

        result.x = v.x * k;
        result.y = v.y * k;
        result.z = v.z * k;
        result.w = v.w * k;
    }
};

/**
 * Normalizes a vector
 * @param {AxVector4} result The resulting normalized vector
 * @param {AxVector4} v The vector to be normalized
 */
AxVector4.Normalize = function(result, v)
{
    AxVector4.SetLength(result, v, 1.0);
};

/**
 * Inverts a vector
 * @param {AxVector4} result The resulting inverted vector
 * @param {AxVector4} v The vector to be inverted
 */
AxVector4.Invert = function(result, v)
{
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    result.w = -v.w;
};
	
/**
 * Calculates the distance between two points in 3D space
 * @param {AxVector4} v1 First point
 * @param {AxVector4} v2 Second point
 * @return {Number} The distance between the two points
 */
AxVector4.Distance = function(v1, v2)
{
    var x = v1.x - v2.x;
    var y = v1.y - v2.y;
    var z = v1.z - v2.z;
    var w = v1.w - v2.w;

    return AxMath.Sqrt(x * x + y * y + z * z + w * w);
};
	
/**
 * Performs linear interpolation or extrapolation between two vectors
 * @param {AxVector4} result The resulting vector produced by interpolating between the given two vectors
 * @param {AxVector4} v1 First vector
 * @param {AxVector4} v2 Second vector
 * @param {Number} factor Interpolation factor. Having this factor outside of [0, 1] results in extrapolation
 */
AxVector4.Lerp = function(result, v1, v2, factor)
{
    result.x = v1.x + (v2.x - v1.x) * factor;
    result.y = v1.y + (v2.y - v1.y) * factor;
    result.z = v1.z + (v2.z - v1.z) * factor;
    result.w = v1.w + (v2.w - v1.w) * factor;
};

/**
 * Subtracts two vectors
 * @param {AxVector4} result The resulting vector produced by subtracting the second vector from the first
 * @param {AxVector4} v1 Vector to subtract from
 * @param {AxVector4} v2 Vector to be subtracted
 */
AxVector4.Subtract = function(result, v1, v2)
{
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    result.w = v1.w - v2.w;
};

/**
 * Adds two vectors
 * @param {AxVector4} result The resulting vector produced by adding the two vectors
 * @param {AxVector4} v1 First vector
 * @param {AxVector4} v2 Second vector
 */
AxVector4.Add = function(result, v1, v2)
{
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    result.w = v1.w + v2.w;
};

/**
 * Calculates the dot product (a.k.a. scalar product) of two vectors. 
 * @param {AxVector4} v1 First vector
 * @param {AxVector4} v2 Second vector
 * @return {Number} The dot product of the two vectors
 */
AxVector4.Dot = function(v1, v2)
{
    return (v1.x * v2.x +
            v1.y * v2.y +
            v1.z * v2.z +
            v1.w * v2.w);
};

/**
 * Calculates the cross product (a.k.a. vector product) of two vectors
 * @param {AxVector4} result The resulting vector produced by adding the two vectors
 * @param {AxVector4} v1 First vector
 * @param {AxVector4} v2 Second vector
 */
AxVector4.Cross = function(result, v1, v2)
{
    // not implemented
};
    
/**
 * Scales a vector by a scalar value
 * @param {AxVector4} result The resulting vector produced by scaling the given vector
 * @param {AxVector4} v1 Original vector for scaling
 * @param {Number} factor Scaling factor
 */
AxVector4.Scale = function(result, v1, factor)
{
    result.x = v1.x * factor;
    result.y = v1.y * factor;
    result.z = v1.z * factor;
    result.w = v1.w * factor;
};

/**
 * Scales a vector's components by the components of another vector
 * @param {AxVector4} result The resulting vector produced by scaling the given vector
 * @param {AxVector4} v1 Original vector for scaling
 * @param {AxVector4} v2 Vector containing the per-component scaling factors
 */
AxVector4.Scale_2 = function(result, v1, v2)
{
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
    result.w = v1.w * v2.w;
};

/**
 * Produces a vector whose components are each the lesser corresponding component from two other vectors
 * @param {AxVector4} result The resulting vector, having the lesser components taken from the two original ones
 * @param {AxVector4} v1 Original vector to compare against
 * @param {AxVector4} v2 Original vector to compare against
 */
AxVector4.Min = function(result, v1, v2)
{
    result.x = v1.x < v2.x ? v1.x : v2.x;
    result.y = v1.y < v2.y ? v1.y : v2.y;
    result.z = v1.z < v2.z ? v1.z : v2.z;
    result.w = v1.w < v2.w ? v1.w : v2.w;
};
    
/**
 * Produces a vector whose components are each the greater corresponding component from two other vectors
 * @param {AxVector4} result The resulting vector, having the greater components taken from the two original ones
 * @param {AxVector4} v1 Original vector to compare against
 * @param {AxVector4} v2 Original vector to compare against
 */
AxVector4.Max = function(result, v1, v2)
{
    result.x = v1.x > v2.x ? v1.x : v2.x;
    result.y = v1.y > v2.y ? v1.y : v2.y;
    result.z = v1.z > v2.z ? v1.z : v2.z;
    result.w = v1.w > v2.w ? v1.w : v2.w;
};
    
/**
 * Transforms a vector by a given tranformation matrix.
 * This transformation takes into account the translation of the transformation
 * @param {AxVector4} result The resulting transformed vector
 * @param {AxVector4} v The original vector to be transformed
 * @param {AxMatrix} transformation The transformation matrix to use
 */
AxVector4.Transform = function(result, v, transformation)
{
    if (AxUtils.IsUndefined(v.w))
    {
        AxVector4.Transform_2(result, v, transformation);
        return;
    }
    
    var x = 	v.x * transformation._11 +
                v.y * transformation._21 +
                v.z * transformation._31 +
                v.w * transformation._41;
    var y = 	v.x * transformation._12 +
                v.y * transformation._22 +
                v.z * transformation._32 +
                v.w * transformation._42;
    var z = 	v.x * transformation._13 +
                v.y * transformation._23 +
                v.z * transformation._33 +
                v.w * transformation._43;
        
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
};

/**
 * Transforms a 3D vector by a given tranformation matrix and returns a 4D vector.
 * This transformation does not take into account the translation of the transformation
 * @param {AxVector4} result The resulting transformed vector
 * @param {AxVector3} v The original vector to be transformed
 * @param {AxMatrix} transformation The transformation matrix to use
 */
AxVector4.Transform_2 = function(result, v, transformation)
{
    var x = v.x * transformation._11 +
            v.y * transformation._21 +
            v.z * transformation._31 +
            transformation._41;
    var y = v.x * transformation._12 +
            v.y * transformation._22 +
            v.z * transformation._32 +
            transformation._42;
    var z = v.x * transformation._13 +
            v.y * transformation._23 +
            v.z * transformation._33 +
            transformation._43;
    var w = v.x * transformation._14 +
            v.y * transformation._24 +
            v.z * transformation._34 +
            transformation._44;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
};