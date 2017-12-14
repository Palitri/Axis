/**
 * Creates a vector object and initializes its components
 * Sets the components of the vector to the given values. Supports multiple input formats
 * Valid input formats are
 * () - Initializes all components to zero
 * (Number) - Sets all components to the value of the given number
 * (Number, Number, Number) - Sets the x, y and z components to the values of the given numbers in the respective order
 * (AxVector2, Number) - Sets the x and y components to the respective values from the given vector and the z component to the values of the given number
 * (AxVector3) - Copies the components from the given vector
 * @param {!Number|AxVector2|AxVector3} v1 For (number) input - the value to set to all components. For (x, y, z) input - the value for the X component of the vector. For (AxVector2, z) input - the AxVector2 which contains the x and y components. For (AxVector3) input - the vector to copy from.
 * @param {!Number} v2 For (x, y, z) input - the value for the Y component. For (AxVector2, z) input - the value for the Z component.
 * @param {!Number} v3 For (x, y, z) input - the value for the Z component.
 * @constructor
 */
function AxVector3(v1, v2, v3)
{
    if (AxUtils.IsUndefinedOrNull(v1))
    {
        this.x = 0.0;
        this.y = 0.0;
        this.z = 0.0;
    }
    else
        this.Set(v1, v2, v3);
}

/**
 * Sets the components of the vector to the given values. Supports multiple input formats
 * Valid input formats are
 * (Number) - Sets all components to the value of the given number
 * (Number, Number, Number) - Sets the x, y and z components to the values of the given numbers in the respective order
 * (AxVector2, Number) - Sets the x and y components to the respective values from the given vector and the z component to the values of the given number
 * (AxVector3) - Copies the components from the given vector
 * @param {Number|AxVector2|AxVector3} v1 For (number) input - the value to set to all components. For (x, y, z) input - the value for the X component of the vector. For (AxVector2, z) input - the AxVector2 which contains the x and y components. For (AxVector3) input - the vector to copy from.
 * @param {!Number} v2 For (x, y, z) input - the value for the Y component. For (AxVector2, z) input - the value for the Z component.
 * @param {!Number} v3 For (x, y, z) input - the value for the Z component.
 */
AxVector3.prototype.Set = function(v1, v2, v3)
{
    if (AxUtils.IsNumber(v3))
    {
        this.x = v1;
        this.y = v2;
        this.z = v3;
    }
    else if (AxUtils.IsNumber(v2))
    {
        this.x = v1.x;
        this.y = v1.y;
        this.z = v2;
    }
    else if (AxUtils.IsNumber(v1))
    {
        this.x = v1;
        this.y = v1;
        this.z = v1;
    }
    else
    {
        this.x = v1.x;
        this.y = v1.y;
        this.z = v1.z;
    }
};

/**
 * Sets the components of the vectors to the given value
 * @param {Number} value The value to set all the three component of the vector to
 */
AxVector3.prototype.Set_1 = function(value)
{
    this.x = value;
    this.y = value;
    this.z = value;
};

/**
 * Sets the components of the vectors to the given values
 * @param {Number} x The value for the X component of the vector
 * @param {Number} y The value for the Y component of the vector
 * @param {Number} z The value for the Z component of the vector
 */
AxVector3.prototype.Set_2 = function(x, y, z)
{
    this.x = x;
    this.y = y;
    this.z = z;
};

/**
 * Constructs the vector's X and Y components from a 2D vector and the Z component from a value
 * @param {AxVector2} source A 2D vector to use as source for the X and Y components
 * @param {Number} z A value to use as source for the Z component
 */
AxVector3.prototype.Set_3 = function(source, z)
{
    this.x = source.x;
    this.y = source.y;
    this.z = z;
};

/**
 * Copies the source vector
 * @param {AxVector3} source A vector to copy from
 */
AxVector3.prototype.Set_4 = function(source)
{
    this.x = source.x;
    this.y = source.y;
    this.z = source.z;
};

/**
 * Compares the vector with another one and returns true if both are identical
 * @param {AxVector3} vector The vector to compare with
 * @returns {Boolean} True if identical to the given vector
 */
AxVector3.prototype.Equals = function(vector)
{
    return  (this.x === vector.x) &&
            (this.y === vector.y) &&
            (this.z === vector.z);
};

/**
 * Calculates the length of the vector
 * @return {Number} Returns the length of the vector
 */
AxVector3.prototype.GetLength = function()
{
    return AxMath.Sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
};

/**
 * Return a vector with the same orientation, but of the given length
 * @param {Number} length Length of the returned vector
 * @returns {AxVector3} A vector with the same orientation as the original, but of the given length
 */
AxVector3.prototype.OfLength = function(length)
{
    var result = new AxVector3();
    AxVector3.SetLength(result, this, length);
    return result;
};

/**
 * Returns the vector normalized
 * @returns {AxVector3} A normalized version of the original vector
 */
AxVector3.prototype.Normalize = function()
{
    var result = new AxVector3();
    AxVector3.Normalize(result, this);
    return result;
};

/**
 * Returns the vector inverted
 * @returns {AxVector3} An inverted version of the original vector
 */
AxVector3.prototype.Invert = function()
{
    var result = new AxVector3();
    AxVector3.Invert(result, this);
    return result;
};
/**
 * Returns the distance between the point represented by the vector and a point represented by the given vector
 * @param {AxVector3} v A vector representing the point to which distance will be calculated
 * @returns {Number} The distance to the given point
 */
AxVector3.prototype.DistanceTo = function(v)
{
    return AxVector3.Distance(this, v);
};

/**
 * Returns a linearly vector interpolated between (or extrapolated outside of) the original and the given one
 * @param {AxVector3} v The vector towards which to interpolate (or extrapolate)
 * @param {Number} factor The interpolation (or extrapolation) value. Values between 0 and 1 result in interpolation, while values outside of this range result in extrapolation.
 * @returns {Number} A linearly interpolated (or extrapolated) vector
 */
AxVector3.prototype.Lerp = function(v, factor)
{
    var result = new AxVector3();
    AxVector3.Lerp(result, this, v, factor);
    return result;
};

/**
 * Interpolates between the components of the original vector and the given one, treating them as angles, thus performing the interpolation between the angular smallest interval
 * @param {AxVector3} v The vector towards which to interpolate
 * @param {Number} factor The interpolation value
 * @returns {Number} The interpolation result
 */
AxVector3.prototype.LerpAngles = function(v, factor)
{
    var result = new AxVector3();
    AxVector3.LerpAngles(result, this, v, factor);
    return result;
};

/**
 * Returns a vetor which is the difference between the original vector and the given one
 * @param {AxVector3} v The vector, which is to be subtracted from the original one
 * @returns {AxVector3} The difference between the original vector and the given one
 */
AxVector3.prototype.Subtract = function(v)
{
    var result = new AxVector3();
    AxVector3.Subtract(result, this, v);
    return result;
};

/**
 * Returns a vetor which is the sum of the original vector and the given one
 * @param {AxVector3} v The vector, which is to be added to the original one
 * @returns {AxVector3} The sum of the original vector and the given one
 */
AxVector3.prototype.Add = function(v)
{
    var result = new AxVector3();
    AxVector3.Add(result, this, v);
    return result;
};

/**
 * Returns the dot product of the original vector and the given one
 * @param {AxVector3} v The vector to calculate dot product against
 * @returns {Number} The dot product of the original vector and the given one
 */
AxVector3.prototype.Dot = function(v)
{
    return AxVector3.Dot(this, v);
};

/**
 * Returns the cross product of the original vector and the given one
 * @param {AxVector3} v The vector to calculate cross product against
 * @returns {AxVector3} The cross product of the original vector and the given one
 */
AxVector3.prototype.Cross = function(v)
{
    var result = new AxVector3();
    AxVector3.Cross(result, this, v);
    return result;
};

/**
 * Returns the vector with each of its components multiplied by the components of the given vector
 * @param {AxVector3} v The vector by which compoenents to multiply the components of the original one
 * @returns {AxVector3} The component-wise scaled vector
 */
AxVector3.prototype.Scale = function(v)
{
    var result = new AxVector3();
    AxVector3.Scale(result, this, v);
    return result;
};

/**
 * Returns the vector with each of its components multiplied by the given value
 * @param {Number} factor The value by which to multiply the components of the original vector
 * @returns {AxVector3} The component-wise scaled vector
 */
AxVector3.prototype.Scale = function(factor)
{
    var result = new AxVector3();
    AxVector3.Scale(result, this, factor);
    return result;
};

/**
 * Returns a vector with each of its components being the lesser between their original value and the value of the corresponding component of the given vector.
 * In effect, the result is a component-wise minimum vector
 * @param {AxVector3} v The vector, whose components to compare against
 * @returns {AxVector3} A component-wise minimum vector
 */
AxVector3.prototype.Min = function(v)
{
    var result = new AxVector3();
    AxVector3.Min(result, this, v);
    return result;
};

/**
 * Returns a vector with each of its components being the greater between their original value and the value of the corresponding component of the given vector.
 * In effect, the result is a component-wise maximum vector
 * @param {AxVector3} v The vector, whose components to compare against
 * @returns {AxVector3} A component-wise maximum vector
 */
AxVector3.prototype.Max = function(v)
{
    var result = new AxVector3();
    AxVector3.Max(result, this, v);
    return result;
};

/**
 * Returns a vector which is the original one, transformed by the given transformation matrix
 * @param {AxMatrix} transformation The transformation matrix to apply
 * @returns {AxVector3} The transformed vector
 */
AxVector3.prototype.Transform = function(transformation)
{
    var result = new AxVector3();
    AxVector3.Transform(result, this, transformation);
    return result;
};

/**
 * Given the original vector contains Cartesian space coordinates (x, y and z), returns the spatially equivalent vector in spherical space coordinates, such that X corresponds to azimuth, Y corresponds to elevation and Z corresponds to radius
 * @returns {AxVector3} The spatially equivalent vector in spherical space coordinates, where X is azimuth, Y is elevation and Z is radius
 */
AxVector3.prototype.CartesianToSpherical = function()
{
    var result = new AxVector3();
    AxVector3.CartesianToSpherical(result, this);
    return result;
};

/**
 * Given the original vector contains spherical space coordinates (X as azimuth, Y as elevation and Z as radius), returns a spatially equivalent vector in Cartesian space coordinates.
 * @returns {AxVector3} The spatially equivalent vector in Cartesian space coordinates, given the original vector is in spherical coordinates
 */
AxVector3.prototype.SphericalToCartesian = function()
{
    var result = new AxVector3();
    AxVector3.SphericalToCartesian(result, this);
    return result;
};

/**
 * Returns a vector in spherical coordinates, which is a linear interpolation between (or extrapolation outside of) the original vector and the provided one, given both are also in spherical coordinates
 * Spherical coordinates are interpreted as X being azimuth, Y being elevation and Z being radius
 * @param {AxVector3} s The vector in spherical coordinates, towards which to interpolate (or extrapolate)
 * @param {Number} factor The interpolation (or extrapolation) value. Values between 0 and 1 result in interpolation, while values outside of this range result in extrapolation.
 * @returns {AxVector3} A linearly interpolated (or extrapolated) vector in spherical coordinates, given the original vector and the provided one are also in spherical coordinates
 */
AxVector3.prototype.LerpSpherical = function(s, factor)
{
    var result = new AxVector3();
    AxVector3.LerpSpherical(result, this, s, factor);
    return result;
};



// Static Methods



/**
 * Copies the source vector into the result
 * @param {AxVector3} result The vector to copy to
 * @param {AxVector3} source The vector to copy from
 */
AxVector3.Copy = function(result, source)
{
    result.x = source.x;
    result.y = source.y;
    result.z = source.z;
};

/**
 * Produces a vector with an orientation given by another vector and a given length
 * @param {AxVector3} result The resulting vector
 * @param {AxVector3} v The vector which holds the orientation
 * @param {Number} length The length value for the new vector
 */
AxVector3.SetLength = function(result, v, length)
{
    var k = AxMath.Sqrt(v.x * v.x + v.y * v.y +	v.z * v.z);
    if (k !== 0.0)
    {
        k = length / k;

        result.x = v.x * k;
        result.y = v.y * k;
        result.z = v.z * k;
    }
};

/**
 * Normalizes a vector
 * @param {AxVector3} result The resulting normalized vector
 * @param {AxVector3} v The vector to be normalized
 */
AxVector3.Normalize = function(result, v)
{
    AxVector3.SetLength(result, v, 1.0);
};
	
/**
 * Inverts a vector
 * @param {AxVector3} result The resulting inverted vector
 * @param {AxVector3} v The vector to be inverted
 */
AxVector3.Invert = function(result, v)
{
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
};

/**
 * Calculates the distance between two points in 3D space
 * @param {AxVector3} v1 First point
 * @param {AxVector3} v2 Second point
 * @return {Number} The distance between the two points
 */
AxVector3.Distance = function(v1, v2)
{
    var x = v1.x - v2.x;
    var y = v1.y - v2.y;
    var z = v1.z - v2.z;

    return AxMath.Sqrt(x * x + y * y + z * z);
};

/**
 * Performs linear interpolation or extrapolation between two vectors
 * @param {AxVector3} result The resulting vector produced by interpolating between the given two vectors
 * @param {AxVector3} v1 First vector
 * @param {AxVector3} v2 Second vector
 * @param {Number} factor Interpolation factor. Having this factor outside of [0, 1] results in extrapolation
 */
AxVector3.Lerp = function(result, v1, v2, factor)
{
    result.x = v1.x + (v2.x - v1.x) * factor;
    result.y = v1.y + (v2.y - v1.y) * factor;
    result.z = v1.z + (v2.z - v1.z) * factor;
};

/**
 * Subtracts two vectors
 * @param {AxVector3} result The resulting vector produced by subtracting the second vector from the first
 * @param {AxVector3} v1 Vector to subtract from
 * @param {AxVector3} v2 Vector to be subtracted
 */
AxVector3.Subtract = function(result, v1, v2)
{
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
};

/**
 * Adds two vectors
 * @param {AxVector3} result The resulting vector produced by adding the two vectors
 * @param {AxVector3} v1 First vector
 * @param {AxVector3} v2 Second vector
 */
AxVector3.Add = function(result, v1, v2)
{
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
};

/**
 * Calculates the dot product (a.k.a. scalar product) of two vectors. 
 * @param {AxVector3} v1 First vector
 * @param {AxVector3} v2 Second vector
 * @return {Number} The dot product of the two vectors
 */
AxVector3.Dot = function(v1, v2)
{
    return (v1.x * v2.x +
            v1.y * v2.y +
            v1.z * v2.z);
};

/**
 * Calculates the cross product (a.k.a. vector product) of two vectors
 * @param {AxVector3} result The resulting vector produced by adding the two vectors
 * @param {AxVector3} v1 First vector
 * @param {AxVector3} v2 Second vector
 */
AxVector3.Cross = function(result, v1, v2)
{
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
};
    
/**
 * Scales a vector by a scalar value
 * @param {AxVector3} result The resulting vector produced by scaling the given vector
 * @param {AxVector3} v1 Original vector for scaling
 * @param {Number} factor Scaling factor
 */
AxVector3.Scale = function(result, v1, factor)
{
    result.x = v1.x * factor;
    result.y = v1.y * factor;
    result.z = v1.z * factor;
};

/**
 * Scales a vector's components by the components of another vector
 * @param {AxVector3} result The resulting vector produced by scaling the given vector
 * @param {AxVector3} v1 Original vector for scaling
 * @param {AxVector3} v2 Vector containing the per-component scaling factors
 */
AxVector3.Scale_2 = function(result, v1, v2)
{
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
};

/**
 * Produces a vector whose components are each the lesser corresponding component from two other vectors
 * @param {AxVector3} result The resulting vector, having the lesser components taken from the two original ones
 * @param {AxVector3} v1 Original vector to compare against
 * @param {AxVector3} v2 Original vector to compare against
 */
AxVector3.Min = function(result, v1, v2)
{
    result.x = v1.x < v2.x ? v1.x : v2.x;
    result.y = v1.y < v2.y ? v1.y : v2.y;
    result.z = v1.z < v2.z ? v1.z : v2.z;
};
    
/**
 * Produces a vector whose components are each the greater corresponding component from two other vectors
 * @param {AxVector3} result The resulting vector, having the greater components taken from the two original ones
 * @param {AxVector3} v1 Original vector to compare against
 * @param {AxVector3} v2 Original vector to compare against
 */
AxVector3.Max = function(result, v1, v2)
{
    result.x = v1.x > v2.x ? v1.x : v2.x;
    result.y = v1.y > v2.y ? v1.y : v2.y;
    result.z = v1.z > v2.z ? v1.z : v2.z;
};
    
/**
 * Transforms a vector by a given tranformation matrix.
 * This transformation takes into account the translation of the transformation
 * @param {AxVector3} result The resulting transformed vector
 * @param {AxVector3} v The original vector to be transformed
 * @param {AxMatrix} transformation The transformation matrix to use
 */
AxVector3.Transform = function(result, v, transformation)
{
    var x = 	v.x * transformation._11 +
                v.y * transformation._21 +
                v.z * transformation._31 +
                transformation._41;
    var y = 	v.x * transformation._12 +
                v.y * transformation._22 +
                v.z * transformation._32 +
                transformation._42;
    var z = 	v.x * transformation._13 +
                v.y * transformation._23 +
                v.z * transformation._33 +
                transformation._43;
        
    result.x = x;
    result.y = y;
    result.z = z;
};

/**
 * Transforms a vector by a given tranformation matrix.
 * This transformation does not take into account the translation of the transformation
 * @param {AxVector3} result The resulting transformed vector
 * @param {AxVector3} v The original vector to be transformed
 * @param {AxMatrix} transformation The transformation matrix to use
 */
AxVector3.TransformNormal = function(result, v, transformation)
{
    var x = v.x * transformation._11 +
            v.y * transformation._21 +
            v.z * transformation._31;
    var y = v.x * transformation._12 +
            v.y * transformation._22 +
            v.z * transformation._32;
    var z = v.x * transformation._13 +
            v.y * transformation._23 +
            v.z * transformation._33;

    result.x = x;
    result.y = y;
    result.z = z;
};

/**
 * Converts Cartesian coordinates into spherical coordinates
 * Accepts 2 sets of input parameters:
 * A result vector and an input vector
 * A result vector and the three coordinates of the input vector
 * The result coordinates are as follow:
 * result.x - azimuth: range 0 to 2Pi, initial direction is negative Z (east), increasing towards full revolution around positive Y
 * result.y - elevation: range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
 * result.z - distance
 * @param {AxVector3} result The resulting vector containing the spherical coordinates.
 * @param {Number|AxVector3} x The X cartesian coordinate or a whole vector containing the three cartesian coordinates
 * @param {!Number} y The Y cartesian coordinate
 * @param {!Number} z The Z cartesian coordinate
 */
AxVector3.CartesianToSpherical = function(result, x, y, z)
{
    if (AxUtils.IsUndefinedOrNull(y))
        AxVector3.CartesianToSpherical_2(result, x);
    else
        AxVector3.CartesianToSpherical_1(result, x, y, z);
};

/**
 * Converts Cartesian coordinates into spherical coordinates
 * The result coordinates are as follow:
 * result.x - azimuth: range 0 to 2Pi, initial direction is negative Z (east), increasing towards full revolution around positive Y
 * result.y - elevation: range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
 * result.z - distance
 * @param {AxVector3} result The resulting vector containing the spherical coordinates.
 * @param {Number} x The X cartesian coordinate
 * @param {Number} y The Y cartesian coordinate
 * @param {Number} z The Z cartesian coordinate
 */
AxVector3.CartesianToSpherical_1 = function(result, x, y, z)
{
    result.z = AxMath.Sqrt(x * x + y * y + z * z);
    if (result.z === 0.0)
        return;

    result.y = AxMath.ArcSin(y / result.z);

    if (x === 0)
        if (z > 0)
            result.x = AxMath.Pi / 2.0;
        else
            result.x = -AxMath.Pi / 2.0;
    else
        result.x = AxMath.ArcTan(z / x);

    if (x < 0)
        result.x += AxMath.Pi;

    if (result.x < 0)
        result.x += AxMath.Pi * 2.0;
};

/**
 * Converts Cartesian coordinates into spherical coordinates
 * The result coordinates are as follow:
 * result.x - azimuth: range 0 to 2Pi, initial direction is negative Z (east), increasing towards full revolution around positive Y
 * result.y - elevation: range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
 * result.z - distance
 * @param {AxVector3} result The resulting vector containing the spherical coordinates.
 * @param {AxVector3} cartesian The vector containing the cartesian coordinates
 */
AxVector3.CartesianToSpherical_2 = function(result, cartesian)
{
    AxVector3.CartesianToSpherical_1(result, cartesian.x, cartesian.y, cartesian.z);
};

/**
 * Converts spherical coordinates into Cartesian coordinates
 * Accepts 2 sets of input parameters:
 * A result vector and a 
 * The spherical coordinates are as follow:
 * @param {AxVector3} result The resulting vector containing the spherical coordinates.
 * @param {Number|AxVector3} arg1 A vector containing spherical coordinates or the azimuth component, in range 0 to 2Pi, initial direction being negative Z (east), increasing towards full revolution around positive Y
 * @param {Number} arg2 The elevation component. Range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
 * @param {Number} arg3 The radial distance component
 */
AxVector3.SphericalToCartesian = function(result, arg1, arg2, arg3)
{
    if (AxUtils.IsUndefinedOrNull(arg2))
        AxVector3.SphericalToCartesian_2(result, arg1);
    else
        AxVector3.SphericalToCartesian_1(result, arg1, arg2, arg3);
};
/**
 * Converts spherical coordinates into Cartesian coordinates
 * The spherical coordinates are as follow:
 * @param {AxVector3} result The resulting vector containing the spherical coordinates.
 * @param {Number} azimuth The azimuth component. Range 0 to 2Pi, initial direction is negative Z (east), increasing towards full revolution around positive Y
 * @param {Number} elevation The elevation component. Range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
 * @param {Number} radius The radial distance component
 */
AxVector3.SphericalToCartesian_1 = function(result, azimuth, elevation, radius)
{
    var eCos = AxMath.Cos(elevation) * radius;
    result.x = AxMath.Cos(azimuth) * eCos;
    result.y = AxMath.Sin(elevation) * radius;
    result.z = AxMath.Sin(azimuth) * eCos;
};

/**
 * Converts spherical coordinates into Cartesian coordinates
 * The spherical coordinates are as follow:
 * spherical.x - azimuth: range 0 to 2Pi, initial direction is negative Z (east), increasing towards full revolution around positive Y
 * spherical.y - elevation: range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
 * spherical.z - distance
 * @param {AxVector3} result The resulting vector containing the Cartesian coordinates.
 * @param {AxVector3} spherical The vector containing the spherical coordinates
 */
AxVector3.SphericalToCartesian_2 = function(result, spherical)
{
    AxVector3.SphericalToCartesian(result, spherical.x, spherical.y, spherical.z);
};
    
/**
 * Interpolates between spherical coordinates, producing a gradual change of a vector's orientation
 * The spherical coordinates are as follow:
 * x - azimuth: range 0 to 2Pi, initial direction is negative Z (east), increasing towards full revolution around positive Y
 * y - elevation: range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
 * z - distance
 * @param {AxVector3} result The resulting vector
 * @param {AxVector3} v1 The first vector of spherical coordinates
 * @param {AxVector3} v2 The second vector of spherical coordinates
 * @param {Number} factor The factor by which to interpolate
 */
AxVector3.LerpSpherical = function(result, v1, v2, factor)
{
    var delta = v2.x - v1.x;
    if (AxMath.Abs(delta) < AxMath.Pi)
        result.x = v1.x + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath.Pi2;
            result.x = v1.x + delta * factor;
            if (result.x < 0)
                result.x += AxMath.Pi2;
        }
        else
        {
            delta += AxMath.Pi * 2;
            result.x = v1.x + delta * factor;
            if (result.x > AxMath.Pi2)
                result.x -= AxMath.Pi2;
        }
    }

    delta = v2.y - v1.y;
    if (AxMath.Abs(delta) < AxMath.Pi)
        result.y = v1.y + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath.Pi2;
            result.y = v1.y + delta * factor;
            if (result.y < 0)
                result.y += AxMath.Pi2;
        }
        else
        {
            delta += AxMath.Pi2;
            result.y = v1.y + delta * factor;
            if (result.y > AxMath.Pi2)
                result.y -= AxMath.Pi2;
        }
    }

    result.z = v1.z + (v2.z - v1.z) * factor;
};

/**
 * Interpolates between the components of the vector, treating them as angles, thus performing the interpolation between the angular smallest interval
 * @param {AxVector3} result The resulting vector
 * @param {AxVector3} v1 The first vector of angular components
 * @param {AxVector3} v2 The second vector of angular comonents
 * @param {Number} factor The factor by which to interpolate
 */
AxVector3.LerpAngles = function(result, v1, v2, factor)
{
    var delta = v2.x - v1.x;
    if (AxMath.Abs(delta) < AxMath.Pi)
        result.x = v1.x + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath.Pi2;
            result.x = v1.x + delta * factor;
            if (result.x < 0)
                result.x += AxMath.Pi2;
        }
        else
        {
            delta += AxMath.Pi * 2;
            result.x = v1.x + delta * factor;
            if (result.x > AxMath.Pi2)
                result.x -= AxMath.Pi2;
        }
    }

    delta = v2.y - v1.y;
    if (AxMath.Abs(delta) < AxMath.Pi)
        result.y = v1.y + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath.Pi2;
            result.y = v1.y + delta * factor;
            if (result.y < 0)
                result.y += AxMath.Pi2;
        }
        else
        {
            delta += AxMath.Pi2;
            result.y = v1.y + delta * factor;
            if (result.y > AxMath.Pi2)
                result.y -= AxMath.Pi2;
        }
    }

    delta = v2.z - v1.z;
    if (AxMath.Abs(delta) < AxMath.Pi)
        result.z = v1.z + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath.Pi2;
            result.z = v1.z + delta * factor;
            if (result.z < 0)
                result.z += AxMath.Pi2;
        }
        else
        {
            delta += AxMath.Pi2;
            result.z = v1.z + delta * factor;
            if (result.z > AxMath.Pi2)
                result.z -= AxMath.Pi2;
        }
    }
};
