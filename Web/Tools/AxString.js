/**
 * Creates a new AxString
 * AxString is just a wrapper over javascript's reglar string and is given primarily for consistency with the C++ implementation of Axis
 * It can freely be manipulated by both the AxString functions and the regular javascript string functions of AxString.contents
 * @param {!*} source A source to use as a contents of the string
 * @param {!Integer} radix If a source representing an integer number is provided, the radix parameter is used to denote the source's base. If not provided, a value of 10 is assumed
 * @constructor
 */
function AxString(source, radix)
{
    this.contents = '';
    
    this.Set(source, radix);
}

AxString.prototype.Set= function(source, radix)
{
    if (AxUtils.IsUndefinedOrNull(source))
    {
        this.contents = '';
        this.length = 0;
        return;
    }
    
    if (AxUtils.IsUndefinedOrNull(radix))
        this.contents = AxString.GetNativeString(source).toString();
    else
        this.contents = AxString.GetNativeString(source).toStfring(radix);
    
    this.length = this.contents.length;
};


/**
 * Sets capacity exactly to the desired amount	
 * @param {Integer} newCapacity The new capacity to be set
 * @param {!Boolean} keepContents If true, contents of the string will be kept as much as the new capacity allows
 */
AxString.prototype.ResizeCapacity = function(newCapacity, keepContents)
{
    if (AxUtils.IsUndefinedOrNull(keepContents))
        keepContents = true;
};

/**
 * Makes sure there is the amount of capacity deisred. If the desired capacity already fits in the current one, no change is made
 * @param {Integer} length Length which to ensure to be available
 */
AxString.prototype.EnsureCapacity = function(length)
{
};

/**
 * Sets the length of the string. If the new length is greater than the current one, the added characters are undefined
 * @param {Integer} length Exact length to be set
 */
AxString.prototype.SetLength = function(length)
{
};

AxString.prototype.GetLength = function()
{
    return this.contents.length;
};

AxString.prototype.IsEmpty = function()
{
    return this.contents === '';
};

AxString.prototype.GetContents = function()
{
    return this.contents;
};

AxString.prototype.CopyCharacters = function(offset, source,  sourceOffset, count)
{
    this.contents = this.contents.substr(0, offset) + source.substr(sourceOffset, count) + this.contenst.substr(offset + count);
    this.length = this.contents.length;
};

/**
 * Compares against the given string
 * @param {AxString} str The string to compare against
 * @return {Integetr} 0 if the given string exactly matches, 1 if it is greater, -1 if it is lesser.
 */
AxString.prototype.Compare = function(str)
{
    str = AxString.GetNativeString(str);

    return str < this.contents ? -1 : str > this.contents ? 1 : 0;
};

AxString.prototype.Equals = function(str)
{
    str = AxString.GetNativeString(str);
    
    return this.contents === str;
};

AxString.prototype.Copy = function(source)
{
    source = AxString.GetNativeString(source);

    this.contents = source;
    this.length = this.contents.length;
};

AxString.prototype.Insert = function(source, position, count)
{
    if (AxUtils.IsUndefinedOrNull(position))
        position = this.length;

    if (AxUtils.IsUndefinedOrNull(count))
        count = AxString.GetLength(source);
    
    source = AxString.GetNativeString(source);

    this.contents = this.contents.substr(0, position) + source.substr(0, count) + this.contents.substr(position);
    this.length = this.contents.length;
};

AxString.prototype.IndexOf = function(substr, start)
{
    substr = AxString.GetNativeString(substr);

    return this.contents.indexOf(substr, start);
};

AxString.prototype.LastIndexOf = function(substr, start)
{
    substr = AxString.GetNativeString(substr);

    return this.contents.lastIndexOf(substr, start);
};

AxString.prototype.SubString = function(start, count)
{
    return new AxString(this.contents.substr(start, count));
};

AxString.prototype.Remove = function(str)
{
    str = AxString.GetNativeString(str);

    return new AxString(this.contents.replace(str, ''));
};

AxString.prototype.Remove_2 = function(start, count)
{
    return new AxString(this.contents.substr(0, start) + this.contents.substr(start + count));
};

AxString.prototype.Replace = function(str, substitute)
{
    str = AxString.GetNativeString(str);
    substitute = AxString.GetNativeString(substitute);

    return new AxString(this.contents.split(str).join(substitute));// this.contents.replace() replaces only the first occurence
};

AxString.prototype.StartsWith = function(str)
{
    str = AxString.GetNativeString(str);

    return this.contents.indexOf(str) === 0;
};

AxString.prototype.EndsWith = function(str)
{
    str = AxString.GetNativeString(str);

    return this.contents.indexOf(str, this.contents.length - str.length) !== -1;
};
	
AxString.prototype.Trim = function(str, left, right)
{
    str = AxString.GetNativeString(str);
   
    var leftIndex = 0;
    if (AxUtils.IsUndefinedOrNull(left) || left)
        while (this.contents.indexOf(str, leftIndex) === leftIndex)
            leftIndex += str.length;

    var rightIndex = this.contents.length;
    if (AxUtils.IsUndefinedOrNull(right) || right)
        while (this.contents.indexOf(str, rightIndex - str.length) === (rightIndex - str.length))
            rightIndex -= str.length;
    
    return new AxString(this.contents.substr(leftIndex, rightIndex - leftIndex));
};
	
AxString.prototype.IsInt = function()
{
    var n = Number(this.contents);
    return !isNaN(parseInt(this.contents)) && (n === this.contents) && (n % 1 === 0);
};

AxString.prototype.ToInt = function(radix)
{
    if (AxUtils.IsUndefinedOrNull(radix))
        radix = 10;
    
    return parseInt(this.contents, radix);
};

AxString.prototype.IsFloat = function()
{
    return !isNaN(parseFloat(this.contents));
};

AxString.prototype.ToFloat = function()
{
    return parseFloat(this.contents);
};

AxString.prototype.ToLower = function()
{
    return new AxString(this.contents.toLowerCase());
};

AxString.prototype.ToUpper = function()
{
    return new AxString(this.contents.toUpperCase());
};



// Static methods



AxString.GetNativeString = function(str)
{
    return str.constructor === AxString ? str.contents : str.toString();
};

AxString.GetAxString = function(str)
{
    return str.constructor === AxString ? str : new AxString(str);
};

AxString.GetLength = function(str)
{
    return AxString.GetNativeString(str).length;
};

AxString.IsNullOrEmpty = function(str)
{
    return AxUtils.IsUndefinedOrNull(str) || str.IsEmpty();
};

/*
 * Encodes a character into UTF8 and returns the size of the encoded data
 * @param {Uint8Array} result The destination of the encoded UTF-8 data. If a null value is passed, no data is encoded
 * @param {Number} resultOffset The offset in bytes, at which the encoded data should be written
 * @param {Number} codePoint The code point of the character to encode
 * @return {Number} The number of bytes in the encoded result
 */
AxString.EncodeUtf8Char = function(result, resultOffset, codePoint)
{
    var trailingByteHeader = 0x80;
    var trailingByteBits = 0x3f;

    if (codePoint >> 7 === 0)
    {
        if (result !== null)
        {
            result[resultOffset] = codePoint;
        }
        
        return 1;
    }

    if (codePoint >> 11 === 0)
    {
        if (result !== null)
        {
            result[resultOffset++] = trailingByteHeader | (codePoint & trailingByteBits);
            result[resultOffset] = (0xff << (5 + 1)) | (codePoint >> 6);
        }

        return 2;
    }

    if (codePoint >> 16 === 0)
    {
        if (result !== null)
        {
            result[resultOffset++] = trailingByteHeader | (codePoint & trailingByteBits);
            result[resultOffset++] = trailingByteHeader | ((codePoint >> 6) & trailingByteBits);
            result[resultOffset] = (0xff << (4 + 1)) | (codePoint >> 12);
        }

        return 3;
    }

    if (codePoint >> 21 === 0)
    {
        if (result !== null)
        {
            result[resultOffset++] = trailingByteHeader | (codePoint & trailingByteBits);
            result[resultOffset++] = trailingByteHeader | ((codePoint >> 6) & trailingByteBits);
            result[resultOffset++] = trailingByteHeader | ((codePoint >> 12) & trailingByteBits);
            result[resultOffset] = (0xff << (3 + 1)) | (codePoint >> 18);
        }
    
        return 4;
    }

    return 0;
};

/**
 * Decodes UTF-8 data into a string and returns the number of decoded bytes
 * @param {Uint8Array} utf The source UTF-8 encoded data of the character
 * @param {Number} utfOffset The offset in bytes, at which the the character is located
 * @return {Object} An object containing the decoded character code point and the number of decoded bytes
 */
AxString.DecodeUtf8Char = function(utf, utfOffset)
{
    var trailingByteHeader = 0x80;
    var trailingByteBitMask = 0x3f;
    
    var result;

    if ((utf[utfOffset] & trailingByteHeader) === 0)
    {
        result = utf[utfOffset];
        
        return { bytesDecoded: 1, codePoint: result };
    }

    result = 0;
    var bytesDecoded = 0;
    var readBits = 0;
    while (utf[utfOffset] >> 6 === 2)
    {
        result |= (utf[utfOffset] & trailingByteBitMask) << readBits;
        readBits += 6;
        utfOffset++;
        bytesDecoded++;
    }

    var headerByteBitMask = (1 << (7 - bytesDecoded)) - 1;
    result |= (utf[utfOffset] & headerByteBitMask) << readBits;
    bytesDecoded++;

    return { bytesDecoded: bytesDecoded, codePoint: result };
};


/**
 * Encodes a string in UTF-8
 * @param {ArrayBuffer} result The destination buffer where the encoded result will be stored
 * @param {Number} resultOffset The offset in bytes, at which the encoded data should be written
 * @param {AxString} str The source string to be encoded
 * @return {Number} The number of bytes in the encoded result
 */
AxString.EncodeUtf8String = function(result, resultOffset, str)
{
    var bytesEncoded = 0;
    for (var i = 0; i < str.length; i++)
    {
        bytesEncoded += AxString.EncodeUtf8Char(result, resultOffset + bytesEncoded, str.charCodeAt(i));
    }
    return bytesEncoded;
};

/**
 * Decodes a string from UTF-8
 * @param {AxString} result The string where the result will be stored
 * @param {ArrayBuffer} utfData The source of the utf-encoded string
 * @param {Number} utfDataLength The length in bytes of the utf data
 * @param {Number} utfDataOffset The offset in bytes, at which the the string is located
 * @return {Number} The number of bytes decoded from the utfData
 */
AxString.DecodeUtf8String = function(result, utfData, utfDataLength, utfDataOffset)
{
    result.SetLength(AxString.GetUTF8AsStringLength(utfData, utfDataLength, utfDataOffset));

    var charIndex = 0;
    var bytesDecoded = 0;
    while (bytesDecoded < utfDataLength)
    {
        var decodedInfo = AxString.DecodeUtf8Char(utfData, utfDataOffset + bytesDecoded);
        if (decodedInfo.bytesDecoded === 0)
            break;
        result.contents += String.fromCharCode(decodedInfo.codePoint);
        bytesDecoded += decodedInfo.bytesDecoded;
        charIndex++;
    }

    return bytesDecoded;
};


/**
 * Determines the number of bytes, required to encode a string in UTF-8 format
 * @param {AxString} str The string for which to determine 
 * @return {Number} The number of bytes decoded from the utfData
 */
AxString.GetStringAsUTF8Length = function(str)
{
    var bytesEncoded = 0;
    for (var i = 0; i < str.length; i++)
    {
        bytesEncoded += AxString.EncodeUtf8Char(null, 0, str.contents[i]);
    }
    return bytesEncoded;
};

/**
 * Determines the number of characters, encoded in a UTF-8 data sequence
 * @param {ArrayBuffer} utfData The source of the utf-encoded string
 * @param {Number} utfDataOffset The offset in bytes, at which the the string is located
 * @param {Number} utfDataLength The size in bytes of the utf data to be decoded
 * @return {Number} The number of bytes decoded from the utfData
 */
AxString.GetUTF8AsStringLength = function(utfData, utfDataOffset, utfDataLength)
{
    var charIndex = 0;
    var bytesDecoded = 0;
    while (bytesDecoded < utfDataLength)
    {
        var decodedInfo = AxString.DecodeUtf8Char(utfData, utfDataOffset + bytesDecoded);
        if (decodedInfo.bytesDecoded === 0)
            break;
        bytesDecoded += decodedInfo.bytesDecoded;
        charIndex++;
    }

    return charIndex;
};
