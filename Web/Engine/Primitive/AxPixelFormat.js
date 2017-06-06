/**
 * Represents a pixel color format
 * The format may have up to four channels - Red, Green, Blue and Alpha, freely arranged, each with an arbitrary number of bits
 * @param {Integer} numChannels The total number of color channels in the format
 * @param {Integer} sizeInBytes The total size in bytes for the color, including any padding bits
 * @param {Integer} channel0BitCount The number of bits used by color channel 0
 * @param {Integer} channel1BitCount The number of bits used by color channel 1
 * @param {Integer} channel2BitCount The number of bits used by color channel 2
 * @param {Integer} channel3BitCount The number of bits used by color channel 3
 * @param {Integer} channel0Id The color identifier for color channel 0
 * @param {Integer} channel1Id The color identifier for color channel 1
 * @param {Integer} channel2Id The color identifier for color channel 2
 * @param {Integer} channel3Id The color identifier for color channel 3
 * @constructor
 */
function AxPixelFormat(numChannels, sizeInBytes, channel0BitCount, channel1BitCount, channel2BitCount, channel3BitCount, channel0Id, channel1Id, channel2Id, channel3Id)
{
    this.numChannels = numChannels;
    this.sizeInBytes = sizeInBytes;
    this.channelBitCount = new Uint8Array(4);
    this.channelBitCount[0] = channel0BitCount;
    this.channelBitCount[1] = channel1BitCount;
    this.channelBitCount[2] = channel2BitCount;
    this.channelBitCount[3] = channel3BitCount;
    this.channelId = new Uint8Array(4);
    this.channelId[0] = channel0Id;
    this.channelId[1] = channel1Id;
    this.channelId[2] = channel2Id;
    this.channelId[3] = channel3Id;
}



AxPixelFormat.ChannelsMax = 4;

AxPixelFormat.ChannelIdRed = 0;
AxPixelFormat.ChannelIdGreen = 1;
AxPixelFormat.ChannelIdBlue = 2;
AxPixelFormat.ChannelIdAlpha = 3;



/**
 * Compares against another pixel color format for equality
 * @param {AxPixelFormat} pixelFormat The color formt to compare against
 * @return {Boolean} True if equal to the given color format
 */
AxPixelFormat.prototype.Equals = function(pixelFormat)
{
    if ((this.numChannels !== pixelFormat.numChannels) || (this.sizeInBytes !== pixelFormat.sizeInBytes))
        return false;
    
    for (var i = 0; i < this.numChannels; i++)
        if ((this.channelBitCount[i] !== pixelFormat.channelBitCount[i]) || (this.channelId[i] !== pixelFormat.channelId[i]))
            return false;
    
    return true;
};

/**
 * Searches for the index of the given channel id
 * @param {Integer} channelId The identifier to search for
 * @return {Integer} The index of the channel holding the given color. If there is no such channel, return -1
 */
AxPixelFormat.prototype.FindChannelIdIndex = function(channelId)
{
    for (var channelIndex = 0; channelIndex < this.numChannels; channelIndex++)
        if (this.channelId[channelIndex] === channelId)
            return channelIndex;

    return -1;
};

/**
 * Searches for the offset in bits of the given channel id
 * @param {Integer} channelId The identifier to search for
 * @return {Integer} The offset in bits of the given channel id. If there is no such channel, return -1
 */
AxPixelFormat.prototype.FindChannelIdBitOffset = function(channelId)
{
    var offset = 0;
    for (var channelIndex = 0; channelIndex < this.numChannels; channelIndex++)
    {
        if (this.channelId[channelIndex] === channelId)
            return offset;

        offset += this.channelBitCount[channelIndex];
    }

    return -1;
};

/*
 * Returns the raw values of the individual color channels of the given pixelValue in an RGBA sequence
 * The returned values are in RGBA sequence and in the original raw format - depending on the format, not all RGBA channels may be present and each channel may be of different bit count
 * @param {Uint32Array} result An Uint32Array to hold the values of the channels in RGBA order of the given pixel value
 * @param {Integer} pixelValue The value of the pixel for which to get the color channels
 */
AxPixelFormat.prototype.GetRGBAValues = function(result, pixelValue)
{
    for (var channelIndex = 0; channelIndex < this.numChannels; channelIndex++)
    {
        var channelBitCount = this.channelBitCount[channelIndex];
        var channelMaxValue = (1 << channelBitCount) - 1;

        result[this.channelId[channelIndex]] = pixelValue & channelMaxValue;

        pixelValue >>= channelBitCount;
    }
};

/*
 * Returns the unit values of the individual color channels of the given pixelValue in an RGBA sequence
 * The returned values are in RGBA sequence and in floating point unit format - that is, each channel value is normalized in the range [0, 1]
 * @param {Float32Array} result A Float32Array to hold the values of the channels in RGBA order of the given pixel value
 * @param {Integer} pixelValue The value of the pixel for which to get the color channels
 */
AxPixelFormat.prototype.GetRGBAUnitValues = function(result, pixelValue)
{
    for (var channelIndex = 0; channelIndex < this.numChannels; channelIndex++)
    {
        var channelBitCount = this.channelBitCount[channelIndex];
        var channelMaxValue = (1 << channelBitCount) - 1;

        result[this.channelId[channelIndex]] = (pixelValue & channelMaxValue) / channelMaxValue;

        pixelValue >>= channelBitCount;
    }
};

/*
 * Returns the raw values of the individual color channels of the given pixelValue in the original channel sequence of the pixel format
 * The returned values are in the format's original channels sequence and in the original raw format - depending on the format each channel may be of different bit count
 * @param {Uint32Array} result An Uint32Array to hold the values of the channels of the given pixel value
 * @param {Integer} pixelValue The value of the pixel for which to get the color channels
 */
AxPixelFormat.prototype.GetChannelsValues = function(result, pixelValue)
{
    for (var channelIndex = 0; channelIndex < this.numChannels; channelIndex++)
    {
        var channelBitCount = this.channelBitCount[channelIndex];
        var channelMaxValue = (1 << channelBitCount) - 1;

        result[channelIndex] = pixelValue & channelMaxValue;

        pixelValue >>= channelBitCount;
    }
};

/*
 * Returns the unit values of the individual color channels of the given pixelValue in the original channel sequence of the pixel format
 * The returned values are in the format's original channels sequence and in floating point unit format - that is, each value is normalized in the range [0, 1]
 * @param {Float32Array} result A Float32Array to hold the values of the channels of the given pixel value
 * @param {Integer} pixelValue The value of the pixel for which to get the color channels
 */
AxPixelFormat.prototype.GetChannelsUnitValues = function(result, pixelValue)
{
    for (var channelIndex = 0; channelIndex < this.numChannels; channelIndex++)
    {
        var channelBitCount = this.channelBitCount[channelIndex];
        var channelMaxValue = (1 << channelBitCount) - 1;

        result[channelIndex] = (pixelValue & channelMaxValue) / channelMaxValue;

        pixelValue >>= channelBitCount;
    }
};

/*
 * Returns a whole pixel value, constructed from the given raw RGBA values
 * The given values must be in an RGBA sequence and each in the original bit count of the format.
 * @param {Uint32Array} rgbaValues An Uint32Array with the color channels values in RGBA order
 * @return {Integer} The pixel value for the given color channels
*/
AxPixelFormat.prototype.SetRGBAValues = function(rgbaValues)
{
    var result = 0;

    for (var channelIndex = this.numChannels - 1; channelIndex >= 0 ; channelIndex--)
    {
        var channelBitCount = this.channelBitCount[channelIndex];

        result <<= channelBitCount;

        result |= rgbaValues[this.channelId[channelIndex]];
    }

    return result;
};

/*
 * Returns a whole pixel value, constructed from the given RGBA unit values
 * The given values must be in an RGBA sequence and each normalized in the range [0, 1]
 * @param {Float32Array} rgbaValues A Float32Array with the color channels values in RGBA order
 * @return {Integer} The pixel value for the given color channels
 */
AxPixelFormat.prototype.SetRGBAUnitValues = function(rgbaValues)
{
    var result = 0;

    for (var channelIndex = this.numChannels - 1; channelIndex >= 0 ; channelIndex--)
    {
        var channelBitCount = this.channelBitCount[channelIndex];
        var channelMaxValue = (1 << channelBitCount) - 1;

        result <<= channelBitCount;

        result |= AxMath.Trunc(rgbaValues[this.channelId[channelIndex]] * channelMaxValue);
    }

    return result;
};

/*
 * Returns a whole pixel value, constructed from the given individual color channels raw values
 * The given values must be in the original channel sequence of the format and each of the original bit count of the format
 * @param {Uint32Array} rgbaValues An Uint32Array with the color channels values, ordered in the original channel sequence of te pixel format
 * @return {Integer} The pixel value for the given color channels
 */
AxPixelFormat.prototype.SetChannelValues = function(rgbaValues)
{
    var result = 0;

    for (var channelIndex = this.numChannels - 1; channelIndex >= 0 ; channelIndex--)
    {
        var channelBitCount = this.channelBitCount[channelIndex];

        result <<= channelBitCount;

        result |= rgbaValues[channelIndex];
    }

    return result;
};

/*
 * Returns a whole pixel value, constructed from the given normalized floating point values
 * The given values must be in the original channel sequence of the format and each normalized in the range [0, 1]
 * @param {Float32Array} rgbaValues A Float32Array with the color channels values, ordered in the original channel sequence of te pixel format
 * @return {Integer} The pixel value for the given color channels
 */
AxPixelFormat.prototype.SetChannelUnitValues = function(rgbaValues)
{
    var result = 0;

    for (var channelIndex = this.numChannels - 1; channelIndex >= 0 ; channelIndex--)
    {
        var channelBitCount = this.channelBitCount[channelIndex];
        var channelMaxValue = (1 << channelBitCount) - 1;

        result <<= channelBitCount;

        result |= AxMath.Trunc(rgbaValues[channelIndex] * channelMaxValue);
    }

    return result;
};

/**
 * Converts a color in RGB format to HSL format
 * @param {Float32Array} result A Float32Array with 3 or more elements to hold the resulting hue, saturation and lightness values in that order
 * @param {Float32Array} rgbValues A Float32Array with the red, green and blue values in that order, which are to be converted
 */
AxPixelFormat.prototype.RGBToHSL = function(result, rgbValues)
{
};

/**
 * Converts a color in HSL format to RGB format
 * @param {Float32Array} result A Float32Array with 3 or more elements to hold the resulting red, green and blue values in that order
 * @param {Float32Array} hlsValues A Float32Array with the hue, saturation and lightness values in that order, which are to be converted
 */
AxPixelFormat.prototype.HSLToRGB = function(result, hlsValues)
{
    AxPixelFormat.HueToRGB(result, hlsValues[0]);

    var s = hlsValues[1];
    result[0] = 0.5 + (result[0] - 0.5) * s;
    result[1] = 0.5 + (result[1] - 0.5) * s;
    result[2] = 0.5 + (result[2] - 0.5) * s;

    var l = hlsValues[2];
    if (l > 0.5)
    {
        l = (l - 0.5) * 2.0;
        result[0] += (1.0 - result[0]) * l;
        result[1] += (1.0 - result[1]) * l;
        result[2] += (1.0 - result[2]) * l;
    }
    else
    {
        l *= 2.0;
        result[0] *= l;
        result[1] *= l;
        result[2] *= l;
    }
};

/**
 * Returns the red, green and blue values of a given hue
 * The returned RGB are pure values, unbiased by saturation and lightness. Effectively, these are the values at the hue at full saturation and neutral lightness (middle)
 * @param {Float32Array} result A Float32Array with 3 or more elements to hold the resulting red, green and blue values in that order
 * @param {Number} hue The hue to get RGB values for
 */
AxPixelFormat.prototype.HueToRGB = function(result, hue)
{
    var hueKeypoints =
    [ 
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 1.0,
            0.0, 0.0, 1.0,
            1.0, 0.0, 1.0
    ];

    var normalized = hue - AxMath.Trunc(hue);
    var keyPoint = normalized * 6.0;
    var keyPointTransition = keyPoint - AxMath.Trunc(keyPoint);

    var keyPointIndex1 = AxMath.Trunc(keyPoint);
    var keyPointIndex2 = (keyPointIndex1 + 1) % 6;

    keyPointIndex1 *= 3;
    keyPointIndex2 *= 3;

    result[0] = hueKeypoints[keyPointIndex1] + (hueKeypoints[keyPointIndex2] - hueKeypoints[keyPointIndex1]) * keyPointTransition;
    result[1] = hueKeypoints[keyPointIndex1 + 1] + (hueKeypoints[keyPointIndex2 + 1] - hueKeypoints[keyPointIndex1 + 1]) * keyPointTransition;
    result[2] = hueKeypoints[keyPointIndex1 + 2] + (hueKeypoints[keyPointIndex2 + 2] - hueKeypoints[keyPointIndex1 + 2]) * keyPointTransition;
};
