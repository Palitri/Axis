AxMath.Pi = 3.14159265358979;
AxMath.Pi_d = 3.141592653589793238462;

// a.k.a. Tau
AxMath.Pi2 = AxMath.Pi * 2.0;
AxMath.Pi2_d = AxMath.Pi_d * 2.0;

AxMath.PiHalf = AxMath.Pi / 2.0;
AxMath.PiHalf_d = AxMath.Pi_d / 2.0;

// a.k.a The golden ratio
AxMath.Phi = 1.61803398874989;
AxMath.Phi_d = 1.6180339887498948482;

// Euler's number
AxMath.E = 2.71828182845904;
AxMath.E_d = 2.7182818284590452353;

AxMath.IntMin = Math.trunc(Number.MIN_VALUE);
AxMath.IntMax = Math.trunc(Number.MIN_VALUE);

AxMath.FloatMin = Number.MIN_VALUE;
AxMath.FloatMax = Number.MAX_VALUE;

AxMath.crcPolynomial_Normal = 0x04C11DB7;
AxMath.crcPolynomial_Reversed = 0xEDB88320;
AxMath.crcPolynomial_ReversedReciprocal = 0x82608EDB;

AxMath.crcTable = [];

/**
 * @constructor
 */
function AxMath()
{
}



// Static methods



/**
 * Returns the truncated integer part of the given argument
 * @param {Number} a The argument for which to get truncated value for
 * @return {Number} The truncated integer part of the argument
 */
AxMath.Trunc = function(a)
{
    return Math.trunc(a);
    //return a - a % 1;
    //return parseInt(a);
};

/**
 * Returns the nearest integer value which is not less than the argument
 * @param {Number} a The argument for which to get integer for
 * @return {Number} The nearest integral value which is not less than the argument
 */
AxMath.Ceil = function(a)
{
    return Math.ceil(a);
};

/**
 * Returns the nearest integer value which is not greater than the argument
 * @param {Number} a The argument for which to get integer for
 * @return {Number} The nearest integral value which is not greater than the argument
 */
AxMath.Floor = function(a)
{
    return Math.floor(a);
};

/**
 * Rounds a value to an integer number
 * @param {Number} x The number which to round
 * @return {Number} Returns an integer rounded value of the given number
 */
AxMath.Round = function(x)
{
    return Math.round(x);
};

/**
 * Returns the lesser of two values
 * @param {Number} a A value to compare against
 * @param {Number} b A value to compare against
 * @return {Number} The lesser of the two given values
 */
AxMath.Min = function(a, b)
{
    if (a < b)
        return a;

    return b;
};

/**
 * Returns the greater of two values
 * @param {Number} a A value to compare against
 * @param {Number} b A value to compare against
 * @return {Number} The greater of the two given values
 */
AxMath.Max = function(a, b)
{
    if (a > b)
        return a;

    return b;
};

/**
 * Returns a number limited by a minimum and a maximum boundaries
 * @param {Number} x The number to trim
 * @param {Number} min Minmimum boundary to trim against
 * @param {Number} max Maximum boundary to trim agains
 * @return {Number} The number, trimmed if necessary, by the given boundaries
 */
AxMath.Trim = function(x, min, max)
{
    if (x < min)
        return min;

    if (x > max)
        return max;

    return x;
};

/**
 * Returns a value of the high 16 bits of a given number
 * @param {Number} value The value to get the high 16 bits from
 * @return {Number} The high 16 bits of the given number
 */
AxMath.High = function(value)
{
    return (value >>> 16) & 0xffff;
};

/**
 * Returns a value of the low 16 bits of a given number
 * @param {Number} value The value to get the low 16 bits from
 * @return {Number} The low 16 bits of the given number
 */
AxMath.Low = function(value)
{
    return value & 0xffff;
};

/**
 * Returns a value whose high and low 16 bit parts are constructed by the low 16 bits of two given values
 * @param {Number} high The value from which to form the high 16 bit part of the result
 * @param {Number} low The value from which to form the low 16 bit part of the result
 * @return {Number} A number whose high and low 16 bit parts are constructed by the low 16 bits of two given values
 */
AxMath.HighLow = function(high, low)
{
    return (high << 16) | (low & 0xffff);
};

/**
 * Returns the sign value of a given number
 * @param {Number} x The number whose sign to get
 * @return {Number} Returns 1 if the number is positive, -1 if negative and 0 if it's zero
 */
AxMath.Sign = function(x)
{
    return x > 0 ? 1 : x < 0 ? -1 : 0;
};

/**
 * Returns the absolute value of a given number
 * @param {Number} x The number whose absolute value to get
 * @return {Number} Returns the absolute value of the given number
 */
AxMath.Abs = function(x)
{
    return x >= 0 ? x : -x;
};

/**
 * Raises a number to a given exponent
 * @param {Number} base The base which to raise
 * @param {Number} exponent The power to which the base will be raised
 * @return {Number} Retrns the base raised to the given exponent
 */
AxMath.Power = function(base, exponent)
{
    return Math.pow(base, exponent);
};

/**
 * Calculates the natural exponent, that is the Euler's number, e, to the power of a given exponent
 * @param {Number} exponent The power to which e will be raised
 * @returns {Number} The natural exponent of the given value
 */
AxMath.Exp = function(exponent)
{
    return Math.exp(exponent);
};

/**
 * Calculates the square root of a number
 * @param {Number} x The radicant number whose squre root to calculate 
 * @return {Number} Returns the square root of the given number
 */
AxMath.Sqrt = function(x)
{
    return Math.sqrt(x);
};

/**
 * Calculates the inverse square root of a number, i.e. 1 / Sqrt(x)
 * @param {Number} x The number whose inverse square root to calculate
 * @return {Number} Returns the inverse square root of the given number
 */
AxMath.InvSqrt = function(x)
{
    return 1 / Math.sqrt(x);
};

/**
 * Calculates the logarithm of x to a given base
 * @param {Number} base The base of the logarithm
 * @param {Number} x The argument of the logarithm
 * @returns {Number} The logarithm of x to the given base
 */
AxMath.Log = function(base, x)
{
    return Math.log(x) / Math.log(b);
};

/**
 * Calculates the natural logarithm of x, that is, the logarithm of x to the Euler number, e
 * @param {Number} x The argument of the logarithm
 * @returns {Number} The natural logarithm of x
 */
AxMath.Ln = function(x)
{
    return Math.log(x);
};

/**
 * Calculates the common logarithm of x, that is, the logarithm of x to base 10
 * @param {Number} x The argument of the logarithm
 * @returns {Number} The common logarithm of x
 */
AxMath.Lg = function(x)
{
    return Math.log10(x);
};

/**
 * Calculates the sine of a number
 * @param {Number} x The number in radians whose sine to calculate
 * @return {Number} Returns the sine of the given number
 */
AxMath.Sin = function(x)
{
    return Math.sin(x);
};

/**
 * Calculates the cosine of a number
 * @param {Number} x The number in radians whose cosine to calculate
 * @return {Number} Returns the cosine of the given number
 */
AxMath.Cos = function(x)
{
    return Math.cos(x);
};

/**
 * Calculates the tangent of a number
 * @param {Number} x The number in radians whose tangent to calculate
 * @return {Number} Returns the tangent of the given number
 */
AxMath.Tan = function(x)
{
    return Math.tan(x);
};

/**
 * Calculates the cotangent of a number
 * @param {Number} x The number in radians whose cotangent to calculate
 * @return {Number} Returns the cotangent of the given number
 */
AxMath.Cot = function(x)
{
    return 1.0 / Math.tan(x);
};


/**
 * Calculates the inverse sine of a number
 * @param {Number} x The number in whose arcsine to calculate
 * @return {Number} Returns the arcsine of the given number
 */
AxMath.ArcSin = function(x)
{
    return Math.asin(x);
};

/**
 * Calculates the inverse cosine of a number
 * @param {Number} x The number in whose arccosine to calculate
 * @return {Number} Returns the arccosine of the given number
 */
AxMath.ArcCos = function(x)
{
    return Math.acos(x);
};

/**
 * Calculates the inverse tangent of a number
 * @param {Number} x The number in whose arctangent to calculate
 * @return {Number} Returns the arctangent of the given number
 */
AxMath.ArcTan = function(x)
{
    return Math.atan(x);
};

/**
 * Calculates the inverse cotangent of a number
 * @param {Number} x The number in whose arccotangent to calculate
 * @return {Number} Returns the arccotangent of the given number
 */
AxMath.ArcCot = function(x)
{
    return AxMath.Pi / 2.0 - Math.atan(x);
};

/**
 * Calculates the inverse of the tangent, obtained by two cartesian coordinates
 * Produces a the quadrant-correct angle between the positive x-axis and the vector with the given coordinates
 * @param {Number} x The X coordinate of the vector whose angle to calculate
 * @param {Number} y The Y coordinate of the vector whose angle to calculate
 * @return {Number} Returns the arctangent of the vector with the given coordinates
 */
AxMath.ArcTan2 = function(y, x)
{
    return Math.atan2(y, x);
};



/**
 * Produces a random integer number ranging from zero (including) to a given limit (excluding), i.e. in [0, n)
 * @param {Number} n The maximum limit, between zero and which to generate the random number. The result can not include the limit itself
 * @return {Number} Returns a random number between (and including) zero and the given limit (excluding)
 */
AxMath.RandomInt = function(n)
{
    return Math.trunc(Math.random() * n);
};

/**
 * Produces a random floating point number between zero (including) and 1 (excluding), i.e. in [0, 1)
 * @return {Number} Returns a random number between (and including) zero and 1 (excluding)
 */
AxMath.Random = function()
{
    return Math.random();
};

/**
 * Generates the table used to calculate CRC sums.
 * @param {Number} crcPolynomial The the polynomial used for generating the table
 */
AxMath.CRC32GenerateTable = function(crcPolynomial)
{
    AxMath.crcTable = [];
    
    for (var i = 0; i < 256; i++)
    {
        var remainder = i;
        for(var j = 0; j < 8; j++)
        {
            if (remainder & 1)
            {
                remainder = (remainder >>> 1) ^ crcPolynomial;
            }
            else
            {
                remainder >>>= 1;
            }
        }

        AxMath.crcTable.push(remainder);
    }
};

/**
 * Calculates the CRC sum for a given source, based on a CRC table and a seed
 * @param {Number} source The source for which to calculate a CRC sum
 * @param {Number} size Size of the source in bytes
 * @param {Number} seed The initial seed for calculating the CRC sum
 * @return {Number} Returns a CRC sum for the given source
 */
AxMath.CRC32 = function(source,  size, seed)
{
    var result = ~seed;

    for (var i = 0; i < size; i++)
        result = AxMath.crcTable[((result & 0xFF) ^ source[i])] ^ (result >>> 8);
            
    return ~result;
};



AxMath.CRC32GenerateTable(AxMath.crcPolynomial_Reversed);