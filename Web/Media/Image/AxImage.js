/**
 * Creates an instance of AxImage
 * @param {!AxImage} source Source image to copy
 * @param {!AxPixelFormat} pixelFormat Pixel format for the new image
 * @constructor
 */
function AxImage(source, pixelFormat)
{
    AxMedia.call(this);
    
    this.Init();
    
    if (!AxUtils.IsUndefinedOrNull(source))
        this.Copy(source);
    
    if (!AxUtils.IsUndefinedOrNull(pixelFormat))
	this.SetPixelFormat(pixelFormat);
}

AxImage.prototype = Object.create(AxMedia.prototype);



/**
 * Gets fired when an image gets loaded
 * @callback AxImageLoadedCallback
 * @param {*} caller The image object, which was loaded
 * @param {*} context An object holding the context for the callback. It is the object passed to the Load method
 * @param {Boolean} succeeded True if the loading was successful, false otherwise
 */


/**
 * Disposes resources
 */
AxImage.prototype.Dispose = function()
{
    this.SetSize(0, 0, 0);
};

AxImage.prototype.Init = function()
{
    this.width = 0;
    this.height = 0;
    this.depth = 0;

    this.pixelData = null;
    this.pixelFormat = new AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat.ChannelIdRed, AxPixelFormat.ChannelIdGreen, AxPixelFormat.ChannelIdBlue, AxPixelFormat.ChannelIdAlpha);
};

/**
 * Sets the pixel format of the image data and performs dithering if required
 * @param {AxPixelFormat} pixelFormat If true, dithering is performed on matching channels which have their bit count reduced
 * @param {!Boolean} dither If true, dithering is performed on matching channels which have their bit count reduced. If omitted, a value of true is assumed
 */
AxImage.prototype.SetPixelFormat = function(pixelFormat, dither)
{
    /*TODO
    if (this.pixelFormat.Equals(pixelFormat))
        return;
    
    if (AxUtils.IsUndefinedOrNull(dither))
        dither = true;

    // perform dithering on matching channels, if required
    if (dither)
    {
        var lineSize = this.width * this.pixelFormat.sizeInBytes;
        var bitOffset = 0;
        for (var channelIndex = 0; channelIndex < this.pixelFormat.numChannels; channelIndex++)
        {
            var newChannelIndex = pixelFormat.FindChannelIdIndex(this.pixelFormat.channelId[channelIndex]);
            if (newChannelIndex !== -1)
            {
                var channelBitDifference = this.pixelFormat.channelBitCount[channelIndex] - pixelFormat.channelBitCount[newChannelIndex];
                if (channelBitDifference > 0)
                {
                    var channelMax = ((1 << this.pixelFormat.channelBitCount[channelIndex]) - 1);
                    var mask = channelMax << bitOffset;
                    var pixelIndex = 0;

                    for (var z = 0; z < this.depth; z++)
                    {
                        for (var y = 0; y < this.height - 1; y++)
                        {
                            pixelIndex += this.pixelFormat.sizeInBytes;
                            for (var x = 1; x < this.width - 1; x++)
                            {
                                var channelOldValue = (*(unsigned int*)pixel & mask) >> bitOffset;
                                var channelNewValue = (channelOldValue >> channelBitDifference) << channelBitDifference;
                                var error = channelOldValue - channelNewValue;

                                *(unsigned int*)pixel = ((*(unsigned int*)pixel) & (~mask)) | (channelNewValue << bitOffset);

                                unsigned int *neighbour = (unsigned int*)(pixel + this.pixelFormat.sizeInBytes);
                                *neighbour = ((*neighbour) & (~mask)) | (AxMath.Min(((*neighbour & mask) >> bitOffset) + 7 * error / 16, channelMax) << bitOffset);
                                neighbour = (unsigned int*)(pixel + lineSize - this.pixelFormat.sizeInBytes);
                                *neighbour = ((*neighbour) & (~mask)) | (AxMath.Min(((*neighbour & mask) >> bitOffset) + 3 * error / 16, channelMax) << bitOffset);
                                neighbour = (unsigned int*)(pixel + lineSize);
                                *neighbour = ((*neighbour) & (~mask)) | (AxMath.Min(((*neighbour & mask) >> bitOffset) + 5 * error / 16, channelMax) << bitOffset);
                                neighbour = (unsigned int*)(pixel + lineSize + this.pixelFormat.sizeInBytes);
                                *neighbour = ((*neighbour) & (~mask)) | (AxMath.Min(((*neighbour & mask) >> bitOffset) + error / 16, channelMax) << bitOffset);

                                pixel += this.pixelFormat.sizeInBytes;
                            }
                            pixel += this.pixelFormat.sizeInBytes;
                        }
                        pixel += lineSize;
                    }
                }
            }

            bitOffset += this.pixelFormat.channelBitCount[channelIndex];
        }
    }


	// create and fill the new channels
	unsigned char *newData = 0;
	int newDataSize = this.width * this.height * this.depth * pixelFormat.sizeInBytes;
	if (newDataSize != 0)
	{
		newData = new unsigned char[newDataSize];

		int bitOffset = 0;
		for (int channelIndex = 0; channelIndex < pixelFormat.numChannels; channelIndex++)
		{
			int channelBitDifference = 0;

			//int oldChannelIndex = pixelFormat.FindChannelIdIndex(this.pixelFormat.channelId[channelIndex]);
			int oldChannelIndex = this.pixelFormat.FindChannelIdIndex(pixelFormat.channelId[channelIndex]);
			int oldChannelBitOffset;
			unsigned int channelOldValue = 0;
			unsigned int oldChannelMax;
			unsigned int oldChannelMask;
			unsigned char *oldChannelPixel = this.pixelData;
			if (oldChannelIndex != -1)
			{
				channelBitDifference = this.pixelFormat.channelBitCount[oldChannelIndex] - pixelFormat.channelBitCount[channelIndex];
				oldChannelBitOffset = this.pixelFormat.FindChannelIdBitOffset(this.pixelFormat.channelId[oldChannelIndex]);
				oldChannelMax = (1 << this.pixelFormat.channelBitCount[oldChannelIndex]) - 1;
				oldChannelMask = oldChannelMax << oldChannelBitOffset;
			}

			
			unsigned int channelNewValue = 0;
			unsigned int newChannelMax = (1 << pixelFormat.channelBitCount[channelIndex]) - 1;
			unsigned int mask = newChannelMax << bitOffset;
			unsigned char *pixel = newData;

			for (int z = 0; z < this.depth; z++)
			{
				for (int y = 0; y < this.height; y++)
				{
					for (int x = 0; x < this.width; x++)
					{
						if ((oldChannelIndex != -1) && (oldChannelMax > 0))
						{
							channelOldValue = (*(unsigned int*)oldChannelPixel & oldChannelMask) >> oldChannelBitOffset;
							channelNewValue = channelBitDifference >= 0 ? channelOldValue >> channelBitDifference : (channelOldValue  * newChannelMax) / oldChannelMax;
						}

						*(unsigned int*)pixel = ((*(unsigned int*)pixel) & (~mask)) | (channelNewValue << bitOffset);

						pixel += pixelFormat.sizeInBytes;
						oldChannelPixel += this.pixelFormat.sizeInBytes;
					}
				}
			}

			bitOffset += pixelFormat.channelBitCount[channelIndex];
		}
	}

	if (this.pixelData != 0)
		delete[] this.pixelData;

	this.pixelData = newData;

	this.pixelFormat = pixelFormat;
        */
};

/**
 * Gets the pixel format of the image
 * @return {AxPixelFormat} The pixel format of the image
 */
AxImage.prototype.GetPixelFormat = function()
{
    return this.pixelFormat;
};

/**
 * Sets the size of the image, allocating the required amount of memory according to the given size and the pixel format of the image
 * @param {Integer} width The width in pixels for the new size of the image
 * @param {Integer} height The height in pixels for the new size of the image
 * @param {Integer} depth The depth in pixels for the new size of the image
 */
AxImage.prototype.SetSize = function(width, height, depth)
{
    this.width = width;
    this.height = height;
    this.depth = depth;

    this.pixelData = null;

    var dataSize = this.width * this.height * this.depth * this.pixelFormat.sizeInBytes;
    if (dataSize !== 0)
        this.pixelData = new Uint8Array(dataSize);
};

/**
 * Copies pixels from a specified region of another image to a specified region.
 * Image size and pixel format are not changed and no pixel format conversion is performed
 * @param {AxImage} source The source image to copy pixels from
 * @param {Integer} srcX The X coordinate of the starting point of the region to copy from the source
 * @param {Integer} srcY The Y coordinate of the starting point of the region to copy from the source
 * @param {Integer} srcZ The Z coordinate of the starting point of the region to copy from the source
 * @param {Integer} destX The X coordinate of the starting point of the region to copy to
 * @param {Integer} destY The Y coordinate of the starting point of the region to copy to
 * @param {Integer} destZ The Z coordinate of the starting point of the region to copy to
 * @param {Integer} width The width of the region to copy
 * @param {Integer} height The height of the region to copy
 * @param {Integer} depth The depth of the region to copy
 */
AxImage.prototype.CopyPixels = function(source, srcX, srcY, srcZ, destX, destY, destZ, width, height, depth)
{
    /*TODO
    if ((source.height == height) && (this.height == height) && (source.width == width) && (this.width == width))
    {
        if ((source.depth == depth) && (this.depth == depth))
        {
            AxMem.Copy(this.pixelData, source.pixelData, width * height * depth * source.pixelFormat.sizeInBytes);
        }
        else
        {
            int planeSize = width * height * 4;
            int dest = *(int*)this.pixelData + destZ * planeSize;
            int src = *(int*)source.pixelData + srcZ * planeSize;
            for (int z = 0; z < depth; z++)
            {
                AxMem.Copy((void*)dest, (void*)src, planeSize); 
                dest += planeSize;
                src += planeSize;
            }
        }
    }
    else
    {
        int lineSize = width * 4;
        int destLineSize = this.width * 4;
        int destPlaneSize = destLineSize * this.height;
        int srcLineSize = source.width * 4;
        int srcPlaneSize = srcLineSize * source.height;
        int dest = *(int*)this.pixelData + destZ * destPlaneSize + destY * destLineSize + destX * 4;
        int src = *(int*)source.pixelData + srcZ * srcPlaneSize + srcY * srcLineSize + srcX * 4;
        for (int z = 0; z < depth; z++)
        {
            int d = dest;
            int s = src;
            for (int y = 0; y < height; y++)
            {
                AxMem.Copy((void*)d, (void*)s, lineSize); 
                d += destLineSize;
                s += srcLineSize;
            }

            dest += destPlaneSize;
            src += srcPlaneSize;
        }
    }
    */
};

/**
 * Copies pixels from a specified region of another image
 * Image size and pixel format are not changed and no pixel format conversion is performed
 * @param {AxImage} source The source image to copy pixels from
 * @param {Integer} srcX The X coordinate of the starting point of the region to copy from the source
 * @param {Integer} srcY The Y coordinate of the starting point of the region to copy from the source
 * @param {Integer} srcZ The Z coordinate of the starting point of the region to copy from the source
 * @param {Integer} width The width of the region to copy
 * @param {Integer} height The height of the region to copy
 * @param {Integer} depth The depth of the region to copy
 */
AxImage.prototype.CopyPixels_2 = function(source, srcX, srcY, srcZ, width, height, depth)
{
    this.CopyPixels(source, srcX, srcY, srcZ, 0, 0, 0, width, height, depth);
};

/**
 * Copies pixels from another image
 * Image size and pixel format are not changed and no pixel format conversion is performed
 * @param {AxImage} source The source image to copy pixels from
 */
AxImage.prototype.CopyPixels_3 = function(source)
{
    this.CopyPixels(source, 0, 0, 0, 0, 0, 0, AxMath.Min(source.width, this.width), AxMath.Min(source.height, this.height), AxMath.Min(source.depth, this.depth));
};

/**
 * Copies another image and sets the pixel format and size accordingly
 * @param {AxImage} source The image to copy from
 */
AxImage.prototype.Copy = function(source)
{
    this.pixelFormat = source.pixelFormat;
    this.SetSize(source.width, source.height, source.depth);
    this.CopyPixels(source);
};

/**
 * Sets a pixel to the given coordinates of the image
 * No pixel format conversion is performed
 * @param {Integer} x The X coordinate of the pixel to set
 * @param {Integer} y The Y coordinate of the pixel to set
 * @param {Integer} z The Z coordinate of the pixel to set
 * @param {Integer} value The color value of the pixel to be set. Contains all the channels, encoded in the pixel format of the image
 */
AxImage.prototype.SetPixel = function(x, y, z, value)
{
    /*TODO
    AxMem.Copy(&this.pixelData[(z * this.width * this.height + y * this.width + x) * this.pixelFormat.sizeInBytes], &value, this.pixelFormat.sizeInBytes);
    */
};

/**
 * Gets the pixel at the given coordinates of the image
 * No pixel format conversion is performed
 * @param {Integer} x The X coordinate of the pixel to get
 * @param {Integer} y The Y coordinate of the pixel to get
 * @param {Integer} z The Z coordinate of the pixel to get
 * @return {Integer} The color value of the pixel at the given coordinates. Contains all the channels, encoded in the pixel format of the image
 */
AxImage.prototype.GetPixel = function(x, y, z)
{
    /*TODO
    unsigned int result = 0;
    AxMem.Copy(&result, &this.pixelData[z * this.width * this.height + y * this.width + x], this.pixelFormat.sizeInBytes);
    return result;
    */
};

/**
 * Gets a sample color from a given Z-layer at given coordinates ranging from 0 to 1
 * The sampling uses linear interpolation
 * @param {Number} u The X coordinate to get a sample at
 * @param {Number} v The Y coordinate to get a sample at
 * @param {Integer} z The Z coordinate of the layer to get the sample from
 * @return {Integer} A color value sampled at the the given coordinates. Contains all the channels, encoded in the pixel format of the image
 */
AxImage.prototype.GetSample = function(u, v, z)
{
    var x = u * this.width;
    var y = v * this.height;

    var x1 = AxMath.Trunc(x);
    var y1 = AxMath.Trunc(y);

    x -= x1;
    y -= y1;

    x1 %= this.width;
    y1 %= this.height;

    var x2 = (x1 + 1) % this.width;
    var y2 = (y1 + 1) % this.height;

    var planeOffset = z * this.width * this.height;

    var intPixelData = new Uint32Array(this.pixelData.buffer);
    
    var p11 = intPixelData[((planeOffset + y1 * this.width + x1) * 4) / this.pixelFormat.sizeInBytes];
    var p12 = intPixelData[((planeOffset + y1 * this.width + x2) * 4) / this.pixelFormat.sizeInBytes];
    var p21 = intPixelData[((planeOffset + y2 * this.width + x1) * 4) / this.pixelFormat.sizeInBytes];
    var p22 = intPixelData[((planeOffset + y2 * this.width + x2) * 4) / this.pixelFormat.sizeInBytes];

    var result = 0;
    var bitShift = 0;

    for (var channelIndex = 0; channelIndex < this.pixelFormat.numChannels; channelIndex++)
    {
        var channelBitCount = this.pixelFormat.channelBitCount[channelIndex];
        var channelLimit = 1 << channelBitCount;
        var channelMax = channelLimit - 1;
        var fx = AxMath.Trunc(x * channelLimit);
        var fy = AxMath.Trunc(y * channelLimit);

        var c11 = p11 & channelMax;
        var c12 = p12 & channelMax;
        var c21 = p21 & channelMax;
        var c22 = p22 & channelMax;

        var c1 = c11 + (((c12 - c11) * fx) >> channelBitCount);
        var c2 = c21 + (((c22 - c21) * fx) >> channelBitCount);
        var c = c1 + (((c2 - c1) * fy) >> channelBitCount);

        result |= c << bitShift;
        bitShift += channelBitCount;

        p11 = p11 >> channelBitCount;
        p12 = p12 >> channelBitCount;
        p21 = p21 >> channelBitCount;
        p22 = p22 >> channelBitCount;
    }

    return result;
};

/**
 * Gets a sample color at a given coordinates ranging from 0 to 1
 * The sampling uses linear interpolation
 * @param {Number} u The X coordinate to get a sample at
 * @param {Number} v The Y coordinate to get a sample at
 * @param {Number} w The Z coordinate to get a sample at
 * @return {Integer} A color value sampled at the the given coordinates. Contains all the channels, encoded in the pixel format of the image
 */
AxImage.prototype.GetSample_2 = function(u, v, w)
{
    /*TODO
	float x = u * this.width;
	float y = v * this.height;
	float z = w * this.depth;

	int x1 = (int)x;
	int y1 = (int)y;
	int z1 = (int)z;

	x -= x1;
	y -= y1;
	z -= z1;

	x1 %= this.width;
	y1 %= this.height;
	z1 %= this.depth;

	int x2 = (x1 + 1) % this.width;
	int y2 = (y1 + 1) % this.height;
	int z2 = (z1 + 1) % this.depth;

	int planeSize = this.width * this.height;
	int plane1Offset = z1 * planeSize;
	int plane2Offset = z2 * planeSize;

	unsigned int p111 = *(unsigned int*)&this.pixelData[(plane1Offset + y1 * this.width + x1) * this.pixelFormat.sizeInBytes];
	unsigned int p112 = *(unsigned int*)&this.pixelData[(plane1Offset + y1 * this.width + x2) * this.pixelFormat.sizeInBytes];
	unsigned int p121 = *(unsigned int*)&this.pixelData[(plane1Offset + y2 * this.width + x1) * this.pixelFormat.sizeInBytes];
	unsigned int p122 = *(unsigned int*)&this.pixelData[(plane1Offset + y2 * this.width + x2) * this.pixelFormat.sizeInBytes];
	unsigned int p211 = *(unsigned int*)&this.pixelData[(plane2Offset + y1 * this.width + x1) * this.pixelFormat.sizeInBytes];
	unsigned int p212 = *(unsigned int*)&this.pixelData[(plane2Offset + y1 * this.width + x2) * this.pixelFormat.sizeInBytes];
	unsigned int p221 = *(unsigned int*)&this.pixelData[(plane2Offset + y2 * this.width + x1) * this.pixelFormat.sizeInBytes];
	unsigned int p222 = *(unsigned int*)&this.pixelData[(plane2Offset + y2 * this.width + x2) * this.pixelFormat.sizeInBytes];

	unsigned int result = 0;
	int bitShift = 0;

	for (int channelIndex = 0; channelIndex < this.pixelFormat.numChannels; channelIndex++)
	{
		int channelBitCount = this.pixelFormat.channelBitCount[channelIndex];
		int channelLimit = 1 << channelBitCount;
		int channelMax = channelLimit - 1;
		int fx = (int)(x * channelLimit);
		int fy = (int)(y * channelLimit);
		int fz = (int)(z * channelLimit);

		int c11 = p111 & channelMax;
		int c12 = p112 & channelMax;
		int c21 = p121 & channelMax;
		int c22 = p122 & channelMax;

		int c1 = c11 + (((c12 - c11) * fx) >> channelBitCount);
		int c2 = c21 + (((c22 - c21) * fx) >> channelBitCount);
		int p1c = c1 + (((c2 - c1) * fy) >> channelBitCount);

		c11 = p211 & channelMax;
		c12 = p212 & channelMax;
		c21 = p221 & channelMax;
		c22 = p222 & channelMax;

		c1 = c11 + (((c12 - c11) * fx) >> channelBitCount);
		c2 = c21 + (((c22 - c21) * fx) >> channelBitCount);
		int p2c = c1 + (((c2 - c1) * fy) >> channelBitCount);

		int pc = p1c + (((p2c - p1c) * fy) >> 8);

		result |= pc << bitShift;
		bitShift += channelBitCount;

		p111 = p111 >> channelBitCount;
		p112 = p112 >> channelBitCount;
		p121 = p121 >> channelBitCount;
		p122 = p122 >> channelBitCount;

		p211 = p211 >> channelBitCount;
		p212 = p212 >> channelBitCount;
		p221 = p221 >> channelBitCount;
		p222 = p222 >> channelBitCount;
	}

	return result;
        */
};

/**
 * Loads the image from a given stream
 * If the particular implementation of AxImage does not support the format in which the source data is encoded, it should return false
 * Different implementations of AxImage can load differently encoded images
 * @param {AxStream} source Stream containing the image encoded in its particular format
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxImageLoadedCallback} callback A callback method which is to be called when the loading has finished
 */
AxImage.prototype.Load = function(source, callbackContext, callback) { callback(this, callbackContext, false); };

/**
 * Saves the image to the given stream in the given encoding format
 * If the particular implementation of AxImage does not support the requested encoding format, it should return false and shouldn't write anything to the stream
 * Different implementations of AxImage can save images in different encodings
 * @param {AxStream} destination Stream to which the image will be written in the requested format
 * @param {String} format Format in which to encode the image. For example, such as 'jpg', 'png' and etc. 
 * @return {Boolean} True if the image was successfully saved to the given stream in the given encoding format
 */
AxImage.prototype.Save = function(destination, format) { return false; };
