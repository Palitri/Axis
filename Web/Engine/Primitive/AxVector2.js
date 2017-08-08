/**
 * Creates a vector object and initializes its components
 * Sets the components of the vector to the given values. Supports multiple input formats
 * Valid input formats are
 * () - Initializes all components to zero
 * (Number) - Sets all components to the value of the given number
 * (Number, Number) - Sets the x and components to the values of the given numbers in the respective order
 * (AxVector2) - Copies the components from the given vector
 * @param {!Number|AxVector2} v1 For (number) input - the value to set to all components. For (x, y) input - the value for the X component of the vector. For (AxVector2) input - the vector to copy from.
 * @param {!Number} v2 For (x, y) input - the value for the Y component.
 * @constructor
 */
function AxVector2(v1, v2)
{
    if (AxUtils.IsUndefinedOrNull(v1))
    {
        this.x = 0.0;
        this.y = 0.0;
    }
    else
        this.Set(v1, v2);
}

/**
 * Sets the components of the vector to the given values. Supports multiple input formats
 * Valid input formats are
 * (Number) - Sets all components to the value of the given number
 * (Number, Number) - Sets the x and components to the values of the given numbers in the respective order
 * (AxVector2) - Copies the components from the given vector
 * @param {Number|AxVector2} v1 For (number) input - the value to set to all components. For (x, y) input - the value for the X component of the vector. For (AxVector2) input - the vector to copy from.
 * @param {!Number} v2 For (x, y) input - the value for the Y component.
 */
AxVector2.prototype.Set = function(v1, v2)
{
    if (AxUtils.IsNumber(v2))
    {
        this.x = v1;
        this.y = v2;
    }
    else if (AxUtils.IsNumber(v1))
    {
        this.x = v1;
        this.y = v1;
    }
    else
    {
        this.x = v1.x;
        this.y = v1.y;
    }
};

/**
 * Sets the components of the vectors to the given value
 * @param {Number} value The value to set to both component of the vector
 */
AxVector2.prototype.Set_1 = function(value)
{
    this.x = value;
    this.y = value;
};

/**
 * Sets the components of the vectors to the given values
 * @param {Number} x The value for the X component of the vector
 * @param {Number} y The value for the Y component of the vector
 */
AxVector2.prototype.Set_2 = function(x, y)
{
    this.x = x;
    this.y = y;
};

/**
 * Copies the source vector
 * @param {AxVector2} source A vector to copy from
 */
AxVector2.prototype.Set_3 = function(source)
{
    this.x = source.x;
    this.y = source.y;
};

/**
 * Compares the vector with another one and returns true if both are identical
 * @param {AxVector2} vector The vector to compare with
 * @returns {Boolean} True if identical to the given vector
 */
AxVector2.prototype.Equals = function(vector)
{
    return  (this.x === vector.x) &&
            (this.y === vector.y);
};

/**
 * Calculates the length of the vector
 * @return {Number} Returns the length of the vector
 */
AxVector2.prototype.GetLength = function()
{
    return AxMath.Sqrt(this.x * this.x + this.y * this.y);
};

/**
 * Return a vector with the same orientation, but of the given length
 * @param {Number} length Length of the returned vector
 * @returns {AxVector2} A vector with the same orientation as the original, but of the given length
 */
AxVector2.prototype.OfLength = function(length)
{
    var result = new AxVector2();
    AxVector2.SetLength(result, this, length);
    return result;
};

/**
 * Returns the vector normalized
 * @returns {AxVector2} A normalized version of the original vector
 */
AxVector2.prototype.Normalize = function()
{
    var result = new AxVector2();
    AxVector2.Normalize(result, this);
    return result;
};

/**
 * Returns the vector inverted
 * @returns {AxVector2} An inverted version of the original vector
 */
AxVector2.prototype.Invert = function()
{
    var result = new AxVector2();
    AxVector2.Invert(result, this);
    return result;
};

/**
 * Returns the distance between the point represented by the vector and a point represented by the given vector
 * @param {AxVector2} v A vector representing the point to which distance will be calculated
 * @returns {Number} The distance to the given point
 */
AxVector2.prototype.DistanceTo = function(v)
{
    return AxVector2.Distance(this, v);
};

/**
 * Returns a linearly vector interpolated between (or extrapolated outside of) the original and the given one
 * @param {AxVector2} v The vector towards which to interpolate (or extrapolate)
 * @param {Number} factor The interpolation (or extrapolation) value. Values between 0 and 1 result in interpolation, while values outside of this range result in extrapolation.
 * @returns {Number} A linearly interpolated (or extrapolated) vector
 */
AxVector2.prototype.Lerp = function(v, factor)
{
    var result = new AxVector2();
    AxVector2.Lerp(result, this, v, factor);
    return result;
};

/**
 * Returns a vetor which is the difference between the original vector and the given one
 * @param {AxVector2} v The vector, which is to be subtracted from the original one
 * @returns {AxVector2} The difference between the original vector and the given one
 */
AxVector2.prototype.Subtract = function(v)
{
    var result = new AxVector2();
    AxVector2.Subtract(result, this, v);
    return result;
};

/**
 * Returns a vetor which is the sum of the original vector and the given one
 * @param {AxVector2} v The vector, which is to be added to the original one
 * @returns {AxVector2} The sum of the original vector and the given one
 */
AxVector2.prototype.Add = function(v)
{
    var result = new AxVector2();
    AxVector2.Add(result, this, v);
    return result;
};

/**
 * Returns the dot product of the original vector and the given one
 * @param {AxVector2} v The vector to calculate dot product against
 * @returns {Number} The dot product of the original vector and the given one
 */
AxVector2.prototype.Dot = function(v)
{
    return AxVector2.Dot(this, v);
};

/**
 * Returns the cross product of the original vector and the given one
 * @returns {AxVector2} The cross product of the original vector and the given one
 */
AxVector2.prototype.Cross = function()
{
    var result = new AxVector2();
    AxVector2.Cross(result, this);
    return result;
};

/**
 * Returns the vector with each of its components multiplied by the components of the given vector
 * @param {AxVector2|Number} arg The AxVector2 vector for component-wise scaling or a scalar number by which to multiply the components of the original vector
 * @returns {AxVector2} The component-wise scaled vector
 */
AxVector2.prototype.Scale = function(arg)
{
    var result = new AxVector2();
    AxVector2.Scale(result, this, arg);
    return result;
};

/**
 * Returns a vector with each of its components being the lesser between their original value and the value of the corresponding component of the given vector.
 * In effect, the result is a component-wise minimum vector
 * @param {AxVector2} v The vector, whose components to compare against
 * @returns {AxVector2} A component-wise minimum vector
 */
AxVector2.prototype.Min = function(v)
{
    var result = new AxVector2();
    AxVector2.Min(result, this, v);
    return result;
};

/**
 * Returns a vector with each of its components being the greater between their original value and the value of the corresponding component of the given vector.
 * In effect, the result is a component-wise maximum vector
 * @param {AxVector2} v The vector, whose components to compare against
 * @returns {AxVector2} A component-wise maximum vector
 */
AxVector2.prototype.Max = function(v)
{
    var result = new AxVector2();
    AxVector2.Max(result, this, v);
    return result;
};

/**
 * Returns a vector which is the original one, transformed by the given transformation matrix
 * @param {AxMatrix} transformation The transformation matrix to apply
 * @returns {AxVector2} The transformed vector
 */
AxVector2.prototype.Transform = function(transformation)
{
    var result = new AxVector2();
    AxVector2.Transform(result, this, transformation);
    return result;
};



// Static Methods



/**
 * Copies the source vector into the result
 * @param {AxVector2} result The vector to copy to
 * @param {AxVector2} source The vector to copy from
 */
AxVector2.Copy = function(result, source)
{
    result.x = source.x;
    result.y = source.y;
};

/**
 * Produces a vector with an orientation given by another vector and a given length
 * @param {AxVector2} result The resulting vector
 * @param {AxVector2} v The vector which holds the orientation
 * @param {Number} length The length value for the new vector
 */
AxVector2.SetLength = function(result, v, length)
{
    var k = AxMath.Sqrt(v.x * v.x + v.y * v.y);

    if (k !== 0.0)
    {
        k = length / k;

        result.x = v.x * k;
        result.y = v.y * k;
    }
};

/**
 * Normalizes a vector
 * @param {AxVector2} result The resulting normalized vector
 * @param {AxVector2} v The vector to be normalized
 */
AxVector2.Normalize = function(result, v)
{
    AxVector2.SetLength(result, v, 1.0);
};

/**
 * Inverts a vector
 * @param {AxVector2} result The resulting inverted vector
 * @param {AxVector2} v The vector to be inverted
 */
AxVector2.Invert = function(result, v)
{
    result.x = -v.x;
    result.y = -v.y;
};

/**
 * Calculates the distance between two points in 3D space
 * @param {AxVector2} v1 First point
 * @param {AxVector2} v2 Second point
 * @return {Number} The distance between the two points
 */
AxVector2.Distance = function(v1, v2)
{
    var x = v1.x - v2.x;
    var y = v1.y - v2.y;

    return AxMath.Sqrt(x * x + y * y);
};
	
/**
 * Performs linear interpolation or extrapolation between two vectors
 * @param {AxVector2} result The resulting vector produced by interpolating between the given two vectors
 * @param {AxVector2} v1 First vector
 * @param {AxVector2} v2 Second vector
 * @param {Number} factor Interpolation factor. Having this factor outside of [0, 1] results in extrapolation
 */
AxVector2.Lerp = function(result, v1, v2, factor)
{
    result.x += (v2.x - v1.x) * factor;
    result.y += (v2.y - v1.y) * factor;
};

/**
 * Subtracts two vectors
 * @param {AxVector2} result The resulting vector produced by subtracting the second vector from the first
 * @param {AxVector2} v1 Vector to subtract from
 * @param {AxVector2} v2 Vector to be subtracted
 */
AxVector2.Subtract = function(result, v1, v2)
{
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
};

/**
 * Adds two vectors
 * @param {AxVector2} result The resulting vector produced by adding the two vectors
 * @param {AxVector2} v1 First vector
 * @param {AxVector2} v2 Second vector
 */
AxVector2.Add = function(result, v1, v2)
{
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
};

/**
 * Calculates the dot product (a.k.a. scalar product) of two vectors. 
 * @param {AxVector2} v1 First vector
 * @param {AxVector2} v2 Second vector
 * @return {Number} The dot product of the two vectors
 */
AxVector2.Dot = function(v1, v2)
{
    return (v1.x * v2.x +
            v1.y * v2.y);
};

/**
 * Calculates the cross product (a.k.a. vector product) of a vector
 * @param {AxVector3} result The resulting vector produced by adding the two vectors
 * @param {AxVector3} v1 Vector to calculate cross product for
 */
AxVector2.Cross = function(result, v1)
{
    result.x = -v1.y;
    result.y = v1.x;
};
    
/**
 * Scales a vector by a scalar value
 * @param {AxVector2} result The resulting vector produced by scaling the given vector
 * @param {AxVector2} v1 Original vector for scaling
 * @param {AxVector2|Number} factor An AxVector2 vector for component-wise scaling or a scalar number by which to multiply the components of the original vector
*/
AxVector2.Scale = function(result, v1, factor)
{
    if (AxUtils.IsUndefinedOrNull(factor.x))
        AxVector2.Scale_1(result, v1, factor);
    else
        AxVector2.Scale_2(result, v1, factor);
};

/**
 * Scales a vector by a scalar value
 * @param {AxVector2} result The resulting vector produced by scaling the given vector
 * @param {AxVector2} v1 Original vector for scaling
 * @param {Number} factor Scaling factor
 */
AxVector2.Scale_1 = function(result, v1, factor)
{
    result.x = v1.x * factor;
    result.y = v1.y * factor;
};

/**
 * Scales a vector's components by the components of another vector
 * @param {AxVector2} result The resulting vector produced by scaling the given vector
 * @param {AxVector2} v1 Original vector for scaling
 * @param {AxVector2} v2 Vector containing the per-component scaling factors
 */
AxVector2.Scale_2 = function(result, v1, v2)
{
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
};


/**
 * Produces a vector whose components are each the lesser corresponding component from two other vectors
 * @param {AxVector2} result The resulting vector, having the lesser components taken from the two original ones
 * @param {AxVector2} v1 Original vector to compare against
 * @param {AxVector2} v2 Original vector to compare against
 */
AxVector2.Min = function(result, v1, v2)
{
    result.x = v1.x < v2.x ? v1.x : v2.x;
    result.y = v1.y < v2.y ? v1.y : v2.y;
};
    
/**
 * Produces a vector whose components are each the greater corresponding component from two other vectors
 * @param {AxVector2} result The resulting vector, having the greater components taken from the two original ones
 * @param {AxVector2} v1 Original vector to compare against
 * @param {AxVector2} v2 Original vector to compare against
 */
AxVector2.Max = function(result, v1, v2)
{
    result.x = v1.x > v2.x ? v1.x : v2.x;
    result.y = v1.y > v2.y ? v1.y : v2.y;
};
    
/**
 * Transforms a vector by a given tranformation matrix.
 * This transformation takes into account the translation of the transformation
 * @param {AxVector2} result The resulting transformed vector
 * @param {AxVector2} v The original vector to be transformed
 * @param {AxMatrix} transformation The transformation matrix to use
 */
AxVector2.Transform = function(result, v, transformation)
{
    var x = v.x * transformation._11 +
            v.y * transformation._21 +
            transformation._31 +
            transformation._41;
    var y = v.x * transformation._12 +
            v.y * transformation._22 +
            transformation._32 +
            transformation._42;

    result.x = x;
    result.y = y;
};

/**
 * Transforms a vector by a given tranformation matrix.
 * This transformation does not take into account the translation of the transformation
 * @param {AxVector2} result The resulting transformed vector
 * @param {AxVector2} v The original vector to be transformed
 * @param {AxMatrix} transformation The transformation matrix to use
 */
AxVector2.TransformNormal = function(result, v, transformation)
{
    var x = v.x * transformation._11 +
	    v.y * transformation._21;
    var y = v.x * transformation._12 +
            v.y * transformation._22;

    result.x = x;
    result.y = y;
};